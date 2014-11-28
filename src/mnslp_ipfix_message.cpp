/// ----------------------------------------*- mode: C++; -*--
/// @file mnslp_libipfix.cpp
/// Tools for processing IPFIX messages in NSIS metering.
/// ----------------------------------------------------------
/// $Id: mnslp_libipfix.cpp 2558 2014-11-14 14:11:00 amarentes $
/// $HeadURL: https://./include/mnslp_libipfix.c $
// ===========================================================
//                      
// Copyright (C) 2012-2014, all rights reserved by
// - System and Computing Engineering, Universidad de los Andes
//
// More information and contact:
// https://www.uniandes.edu.co/
//                      
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 of the License
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
// ===========================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <limits.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <netinet/in.h>

#include <fcntl.h>
#include <netdb.h>

#include "mnslp_ipfix_fields.h"
#ifdef SSLSUPPORT
#include "ipfix_ssl.h"
#endif


#include "mnslp_ipfix_message.h"
#include "mnslp_ipfix_exception.h"


namespace mnslp_ipfix
{


/*----- defines ----------------------------------------------------------*/

#define NODEBUG

#ifndef NTOHLL
uint8_t g_isLittleEndian = 0;
void testEndianness() {
	uint32_t tmp = 0x0a0b0c0d;
	g_isLittleEndian = (tmp != ntohl(tmp));
}
#endif

#define INSERTU16(b,l,val) \
        { uint16_t _t=htons((val)); memcpy((b),&_t,2); (l)+=2; }
#define INSERTU32(b,l,val) \
        { uint32_t _t=htonl((val)); memcpy((b),&_t,4); (l)+=4; }


mnslp_ipfix_message::mnslp_ipfix_message( int sourceid, int ipfix_version ):
		message(NULL), g_tstart(0), g_buflist(NULL)
{

    ipfix_t       *i;

	g_data = { NULL, NULL, 0 };

    switch( ipfix_version ) {
      case IPFIX_VERSION_NF9:
          break;
      case IPFIX_VERSION:
          break;
      default:
          errno = ENOTSUP;
          throw mnslp_ipfix_bad_argument("Invalid IP Fix Version");
    }

	try
	{
		i = new ipfix_t;
		i->buffer = new char[IPFIX_DEFAULT_BUFLEN];
	}
	catch(std::bad_alloc& exc)
	{
		if ( i != NULL )
			delete( i );
        throw std::bad_alloc(exc);
    }

    i->sourceid  = sourceid;
    i->offset    = 0;
    i->version   = ipfix_version;
    i->seqno     = 0;

    message = i;
}


mnslp_ipfix_message::~mnslp_ipfix_message( void )
{

    close( );

    g_tstart = 0;
    
    if ( g_data.lens ) delete( g_data.lens );
    if ( g_data.addrs ) delete( g_data.addrs );
    g_data.maxfields = 0;
    g_data.lens  = NULL;
    g_data.addrs = NULL;
}


/*
 * name:        ipfix_add_vendor_information_elements()
 * parameters:  > fields - array of fields of size nfields+1
 *                         the last field has ftype = 0
 * description: add information elements to global list of field types
 * remarks:
 */
int mnslp_ipfix_message::add_vendor_information_elements( ipfix_field_type_t *fields )
{
    ipfix_field_type_t *ft;

    if ( ! g_tstart ) {          /* hack: change this! */
        init();
    }

    /** add to list of field types
     */
    ft = fields;
    while ( ft->ftype !=0 ) {
        /** create new node
         */
        g_ipfix_fields.AddFieldType(*ft);

        ft++;
        continue;
    }

    return 0;
}

/**
 * name:        init()
 * parameters:
 * remarks:     init module, read field type info.
 */
void mnslp_ipfix_message::init( void )
{
    /* check and store in global flag, 
     * whether we are on a Small or BigEndian machine */
    testEndianness(); 

    g_tstart = time(NULL);
    signal( SIGPIPE, SIG_IGN );
    g_lasttid = 255;

    /** alloc iobufs, todo!
     ** for ( i=0; i< niobufs; i++ ) ....
     */
    g_buflist = &(g_iobuf[0]);
    g_iobuf[0].next = &(g_iobuf[1]);
    g_iobuf[1].next = NULL;

    /** init list of field types
     ** - from field_types.h
     ** - in future from config files
     */

    g_ipfix_fields.initialize_forward();

    g_ipfix_fields.initialize_reverse();

}


/*
 * name:        new_data_template()
 * parameters:
 * return:      0 = ok, -1 = error
 */
void mnslp_ipfix_message::new_data_template( mnslp_ipfix_template **templ, int nfields )
{
    new_template( templ, nfields );
    (*templ)->type = DATA_TEMPLATE;
}

/*
 * name:        ipfix_new_option_template()
 * parameters:
 * return:      0 = ok, -1 = error
 */
int mnslp_ipfix_message::new_option_template( ipfix_template_t **templ,
                         int              nfields )
{
    if ( new_template( templ, nfields ) <0 )
        return -1;

    (*templ)->type = OPTION_TEMPLATE;
    return 0;
}


/*
 * name:        new_template()
 * parameters:
 * return:      0 = ok, -1 = error
 */
void mnslp_ipfix_message::new_template( mnslp_ipfix_template **templ, int nfields )
{
    mnslp_ipfix_template  *t;

    if ( !message || !templ  ) {
        errno = EINVAL;
        throw mnslp_ipfix_bad_argument("Template parameter invalid");
    }
    
    if ( (nfields<1))
    {
		throw mnslp_ipfix_bad_argument("Invalid number of fields");
	}

    /** alloc mem
     */
    try
    {
		t = new mnslp_ipfix_template;
		
		/** generate template id, todo!
		 */
		g_lasttid++;
		t->tid       = g_lasttid;
		t->nfields   = 0;
		t->maxfields = nfields;
		*templ       = t;

		/** add template to template container
		 */
		(ifh->templates).add_template(*t);
	
	}
	catch (std::bad_alloc& exc)
	{
		if (t != NULL)
			delete t;
		throw std::bad_alloc(exc);
	}

}


/*
 * name:        add_field()
 * parameters:
 * return:      0 = ok, -1 = error
 */
int mnslp_ipfix_message::add_field( mnslp_ipfix_template *templ,
               uint32_t         eno,
               uint16_t         type,
               uint16_t         length )
{
    if ( (templ->nfields < templ->maxfields)
         && (type < IPFIX_EFT_VENDOR_BIT) ) {
        /** set template field
         */

        mnslp_ipfix_field * field = g_ipfix_fields.get_field(eno, type);
        if (field==NULL) {
            errno = EINVAL;
            return -1;
        }
        templ->add_field(length,0,0,1,field);
    }
    else {
        errno = EINVAL;
        return -1;
    }

    return 0;
}

/*
 * name:        add_scope_field()
 * parameters:
 * return:      0 = ok, -1 = error
 */
int mnslp_ipfix_message::add_scope_field( mnslp_ipfix_template *templ,
                     uint32_t         eno,
                     uint16_t         type,
                     uint16_t         length )
{
    int i;

    if ( templ->type != OPTION_TEMPLATE ) {
        errno = EINVAL;
        return -1;
    }

    if ( templ->nfields < templ->maxfields ) {

        mnslp_ipfix_field * field = g_ipfix_fields.get_field(eno, type);
        if (field==NULL) {
            errno = EINVAL;
            return -1;
        }
        templ->add_field(length,0,0,0,field);
    }

    return 0;
}

/*
 * name:        delete_template()
 * parameters:
 * return:
 */
void mnslp_ipfix_message::delete_template( mnslp_ipfix_template *templ )
{

    if ( ! templ )
        return;

    /** remove template from list
     */
    
    templates.delete_template(templ);

    delete( templ );
}

/*
 * name:        ipfix_make_template()
 * parameters:
 * return:      generates a new template and stores a pointer to it into the templ parameter
 */
int mnslp_ipfix_message::make_template( ipfix_template_t **templ,
				    export_fields_t *fields, 
					int nfields )
{
    ipfix_template_t *t;
    int i;

    if ( new_data_template( &t, nfields ) <0 ) {
        mlogf( 0, "new_template() failed: %s\n", strerror(errno) );
        return -1;
    }

    for ( i=0; i<nfields; i++ ) {
        if ( add_field( handle, t, fields[i].eno, 
			fields[i].ienum, fields[i].length ) <0 ) {
            mlogf( 0, "add_field() failed: %s\n", strerror(errno) );
            ipfix_delete_template( handle, t );
            return -1;
        }
    }

    *templ = t;
    return 0;

}

void mnslp_ipfix_message::finish_cs( void )
{
    size_t   buflen;
    uint8_t  *buf;

    /* finish current dataset */
    if ( (buf = ifh->cs_header) ==NULL )
        return;
    buflen = 0;
    INSERTU16( buf+buflen, buflen, ifh->cs_tid );
    INSERTU16( buf+buflen, buflen, ifh->cs_bytes );
    ifh->cs_bytes = 0;
    ifh->cs_header = NULL;
    ifh->cs_tid = 0;
}

/*
 * name:        _write_hdr()
 * parameters:
 * return:      0/-1
 */
int mnslp_ipfix_message::_write_hdr( iobuf_t *buf )
{
    time_t      now = time(NULL);

    /** fill ipfix header
     */
    if ( ifh->version == IPFIX_VERSION_NF9 ) {
        buf->buflen = 0;
        ifh->seqno++;
        INSERTU16( buf->buffer+buf->buflen, buf->buflen, ifh->version );
        INSERTU16( buf->buffer+buf->buflen, buf->buflen, ifh->nrecords );
        INSERTU32( buf->buffer+buf->buflen, buf->buflen, ((now-g_tstart)*1000));
        INSERTU32( buf->buffer+buf->buflen, buf->buflen, now );
        INSERTU32( buf->buffer+buf->buflen, buf->buflen, ifh->seqno );
        INSERTU32( buf->buffer+buf->buflen, buf->buflen, ifh->sourceid );
    }
    else {
        buf->buflen = 0;
        INSERTU16( buf->buffer+buf->buflen, buf->buflen, ifh->version );
        INSERTU16( buf->buffer+buf->buflen, buf->buflen,
                   ifh->offset + IPFIX_HDR_BYTES );
        INSERTU32( buf->buffer+buf->buflen, buf->buflen, now );
        INSERTU32( buf->buffer+buf->buflen, buf->buflen, ifh->seqno - ifh->nrecords );
        INSERTU32( buf->buffer+buf->buflen, buf->buflen, ifh->sourceid );
    }

    return 0;
}

void mnslp_ipfix_message::_freebuf( iobuf_t *b )
{
    if ( b ) {
        b->next = g_buflist;
        g_buflist = b;
    }
}

iobuf_t *mnslp_ipfix_message::_getbuf ( void )
{
    iobuf_t *b = g_buflist;

    if ( b ) {
        g_buflist = b->next;
        b->next = NULL;
    }

    return b;
}


/* name:        _output_flush()
 * parameters:
 * remarks:    
 */
int mnslp_ipfix_message::_output_flush( void )
{
    iobuf_t           *buf;
    int               ret;

    if ( (message==NULL) || (message->offset==0) )
        return 0;

    if ( message->cs_tid > 0 ) {
        /* finish current dataset */
        finish_cs( );
    }
    
    if ( (buf=_getbuf()) ==NULL )
        return -1;

    _write_hdr( buf );
    memcpy( buf->buffer+buf->buflen, message->buffer, ifh->offset );
    buf->buflen += message->offset;

    /*
    col = ifh->collectors;
    ret = 0;
    while ( col ) {
        if ( _ipfix_send_msg( ifh, col, buf ) <0 ) {
            ret = -1;
        }
        else {
            ifh->offset = 0;   
            ifh->nrecords = 0;
        }

        col = col->next;
    }
    */

    _freebuf( buf );
    return ret;
}

/*
 * name:        ipfix_close()
 * parameters:
 * return:      0 = ok, -1 = error
 */
void mnslp_ipfix_message::close( void )
{
    if ( message )
    {
        _output_flush( );
		delete (message->buffer);
        delete(message)
    }
}



/*
 * name:        _write_template()
 * parameters:
 * return:      0/-1
 */
int mnslp_ipfix_message::_write_template( ipfix_template_t  *templ )
{
    size_t            buflen, tsize=0, ssize=0, osize=0;
    char              *buf;
    uint16_t          tmp16;
    int               i, n;

    /** calc template size
     */
    if ( templ->type == OPTION_TEMPLATE ) {
        for ( i=0, ssize=0; i<templ->get_number_scopefields(); i++ ) {
            ssize += 4;
            if (templ->get_field(i).elem->ft->eno != IPFIX_FT_NOENO)
                ssize += 4;
        }
        for ( osize=0; i<templ->get_numfields(); i++ ) {
            osize += 4;
            if (templ->get_field(i).elem->ft->eno != IPFIX_FT_NOENO)
                osize += 4;
        }
        tsize = 10 + osize + ssize;
    } else {
        for ( tsize=8,i=0; i<templ->get_numfields(); i++ ) {
            tsize += 4;
            if (templ->get_field(i).elem->ft->eno != IPFIX_FT_NOENO)
                tsize += 4;
        }
    }

    /* check space */
    if ( tsize + message->offset > IPFIX_DEFAULT_BUFLEN ) 
    {
         if ( _output_flush(  ) < 0 )
             return -1;
         if ( tsize + message->offset > IPFIX_DEFAULT_BUFLEN )
             return -1;
    }

    /* write template prior to data */
    if ( message->offset > 0 ) 
    {
         memmove( message->buffer + tsize, message->buffer, message->offset );
         if ( message->cs_tid )
             message->cs_header += tsize;          
    }

    buf = messsage->buffer;
    buflen = 0;

    /** insert template set into buffer
     */
    if ( message->version == IPFIX_VERSION_NF9 ) {
        if ( templ->type == OPTION_TEMPLATE ) {
            INSERTU16( buf+buflen, buflen, IPFIX_SETID_OPTTEMPLATE_NF9);
            INSERTU16( buf+buflen, buflen, tsize );
            INSERTU16( buf+buflen, buflen, templ->tid );
            INSERTU16( buf+buflen, buflen, ssize );
            INSERTU16( buf+buflen, buflen, osize );
        } else {
            INSERTU16( buf+buflen, buflen, IPFIX_SETID_TEMPLATE_NF9);
            INSERTU16( buf+buflen, buflen, tsize );
            INSERTU16( buf+buflen, buflen, templ->tid );
            INSERTU16( buf+buflen, buflen, templ->nfields );
        }
    } else {
        if ( templ->type == OPTION_TEMPLATE ) {
            INSERTU16( buf+buflen, buflen, IPFIX_SETID_OPTTEMPLATE );
            INSERTU16( buf+buflen, buflen, tsize );
            INSERTU16( buf+buflen, buflen, templ->tid );
            INSERTU16( buf+buflen, buflen, templ->nfields );
            INSERTU16( buf+buflen, buflen, templ->nscopefields );
        } else {
            INSERTU16( buf+buflen, buflen, IPFIX_SETID_TEMPLATE);
            INSERTU16( buf+buflen, buflen, tsize );
            INSERTU16( buf+buflen, buflen, templ->tid );
            INSERTU16( buf+buflen, buflen, templ->nfields );
        }
    }

    if ( templ->type == OPTION_TEMPLATE ) {
        n = templ->get_numfields();
        for ( i=0; i<templ->get_number_scopefields(); i++ ) {
            if ( templ->get_field(i).elem->ft->eno == IPFIX_FT_NOENO ) {
                INSERTU16( buf+buflen, buflen, templ->get_field(i).elem->ft->ftype );
                INSERTU16( buf+buflen, buflen, templ->get_field(i).flength );
            } else {
                tmp16 = templ->get_field(i).elem->ft->ftype|IPFIX_EFT_VENDOR_BIT;
                INSERTU16( buf+buflen, buflen, tmp16 );
                INSERTU16( buf+buflen, buflen, templ->get_field(i).flength );
                INSERTU32( buf+buflen, buflen, templ->get_field(i).elem->ft->eno );
            }
        }
    } else {
        i = 0;
        n = templ->get_numfields();
    }

    for ( ; i<templ->get_numfields(); i++ )
    {
        if ( templ->get_field(i).elem->ft->eno == IPFIX_FT_NOENO ) {
            INSERTU16( buf+buflen, buflen, templ->get_field(i).elem->ft->ftype );
            INSERTU16( buf+buflen, buflen, templ->get_field(i).flength );
        } else {
            tmp16 = templ->get_field(i).elem->ft->ftype|IPFIX_EFT_VENDOR_BIT;
            INSERTU16( buf+buflen, buflen, tmp16 );
            INSERTU16( buf+buflen, buflen, templ->get_field(i).flength );
            INSERTU32( buf+buflen, buflen, templ->get_field(i).elem->ft->eno );
        }
    }
    templ->tsend = time(NULL);


    message->offset += buflen;
    if ( message->version == IPFIX_VERSION_NF9 )
         message->nrecords++;

    return 0;
}

int mnslp_ipfix_message::_output_array( ipfix_template_t *templ,
                         int              nfields,
                         void             **fields,
                         uint16_t         *lengths )
{
    int               i, newset_f=0;
    size_t            buflen, datasetlen;
    uint8_t           *p, *buf;

    /** parameter check
     */
    if ( (templ==NULL) || (nfields!=templ->get_numfields()) ) {
        errno = EINVAL;
        return -1;
    }

    if ( templ->get_tsend() == 0 ) {

         if ( _write_template( templ ) <0 )
                return -1;
    }


    /** get size of data set, check space
     */
    if ( templ->get_template_id() == message->cs_tid ) {
        newset_f = 0;
        datasetlen = 0;
    }
    else {
        if ( message->cs_tid > 0 ) {
            finish_cs( );
        }
        newset_f = 1;
        datasetlen = 4;
    }
    
    for ( i=0; i<nfields; i++ ) {
        if ( templ->get_field(i).flength == IPFIX_FT_VARLEN ) {
            if ( lengths[i]>254 )
                datasetlen += 3;
            else
                datasetlen += 1;
        } else {
            if ( lengths[i] > templ->get_field(i).flength ) {
                errno = EINVAL;
                return -1;
            }
        }
        datasetlen += lengths[i];
    }

    if ( (message->offset + datasetlen) > IPFIX_DEFAULT_BUFLEN ) {
        if ( message->cs_tid )
            finish_cs( );
        newset_f = 1;

        if ( _output_flush( ) <0 )
            return -1;
    }

    /* fill buffer */
    buf    = (uint8_t*)(message->buffer) + message->offset;
    buflen = 0;

    if ( newset_f ) {
        /* insert data set
         */
        message->cs_bytes = 0;
        message->cs_header = buf;
        message->cs_tid = templ->tid;
        INSERTU16( buf+buflen, buflen, templ->tid );
        INSERTU16( buf+buflen, buflen, datasetlen );
    }
    /* csc: to be checked with Lutz whether the usage of "datasetlen" 
     * in the last 30 lines of code is correct */

    /* insert data record
     */
    for ( i=0; i<nfields; i++ ) {
        if ( templ->get_field(i).flength == IPFIX_FT_VARLEN ) {
            if ( lengths[i]>254 ) {
                *(buf+buflen) = 0xFF;
                buflen++;
                INSERTU16( buf+buflen, buflen, lengths[i] );
            }
            else {
                *(buf+buflen) = lengths[i];
                buflen++;
            }
        }
        p = fields[i];
        if ( templ->get_field(i).relay_f ) {
            ipfix_encode_bytes( p, buf+buflen, lengths[i] ); /* no encoding */
        }
        else {
            templ->get_field(i).elem->encode( p, buf+buflen, lengths[i] );
        }
        buflen += lengths[i];
    }

    message->nrecords ++;
    message->offset += buflen;
    message->cs_bytes += buflen;
    if ( message->version == IPFIX_VERSION ) {
        message->seqno ++;
    }
    return 0;
}

int mnslp_ipfix_message::output_array( ipfix_template_t *templ,
                  int              nfields,
                  void             **fields,
                  uint16_t         *lengths )
{
    int ret;
    ret = _output_array( templ, nfields, fields, lengths );
    return ret;
}



int mnslp_ipfix_message::output( ipfix_template_t *templ, ... )
{
    int       i;
    va_list   args;

    if ( !templ ) {
        errno = EINVAL;
        return -1;
    }

    if ( templ->get_numfields() > g_data.maxfields ) {
        if ( g_data.addrs ) delete( g_data.addrs );
        if ( g_data.lens ) delete( g_data.lens );
        if ( (g_data.lens= new uint16_t[get_numfields()] ) ==NULL) {
            g_data.maxfields = 0;
            return -1;
        }
        if ( (g_data.addrs=calloc( templ->get_numfields(), sizeof(void*))) ==NULL) {
            delete( g_data.lens );
            g_data.lens = NULL;
            g_data.maxfields = 0;
            return -1;
        }
        g_data.maxfields = templ->get_numfields();
    }

    /** collect pointers
     */
    va_start(args, templ);
    for ( i=0; i<templ->get_numfields(); i++ )
    {
        g_data.addrs[i] = va_arg(args, char*);          /* todo: change this! */
        if ( templ->fields[i].flength == IPFIX_FT_VARLEN )
            g_data.lens[i] = va_arg(args, int);
        else
            g_data.lens[i] = templ->fields[i].flength;
    }
    va_end(args);

    return output_array( templ, templ->get_numfields(), g_data.addrs, g_data.lens );
}


int mnslp_ipfix_message::output_flush(  )
{
    int ret;
    ret = _output_flush( );
    return ret;
}


}
