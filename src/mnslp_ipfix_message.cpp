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
#include <iostream>
#include <stdexcept>      // std::out_of_range

#include <fcntl.h>
#include <netdb.h>

#include "mnslp_ipfix_fields.h"
#ifdef SSLSUPPORT
#include "ipfix_ssl.h"
#endif


#include "mnslp_ipfix_message.h"
#include "mnslp_ipfix_data_record.h"
#include "mnslp_ipfix_exception.h"


namespace mnslp_ipfix
{


/*----- defines ----------------------------------------------------------*/

#define NODEBUG


#define INSERTU16(b,l,val) \
        { uint16_t _t=htons((val)); memcpy((b),&_t,2); (l)+=2; }
#define INSERTU32(b,l,val) \
        { uint32_t _t=htonl((val)); memcpy((b),&_t,4); (l)+=4; }

#define INSERT_U16_NOENCODE(b,l,val) \
		{ uint16_t _t = val; memcpy((b),&_t,2); (l)+=2; }

#define INSERT_U32_NOENCODE(b,l,val) \
        { uint32_t _t=val; memcpy((b),&_t,4); (l)+=4; }

#define READ16_NOENCODE(val,b) \
        { memcpy((&val),b,2); }

#define READ32_NOENCODE(val,b) \
	    { memcpy((&val),b,4); } 


#define READ16(b) ((*(b)<<8)|*((b)+1))
#define READ32(b) ((((((*(b)<<8)|*(b+1))<<8)|(*(b+2)))<<8)|*(b+3))


mnslp_ipfix_message::mnslp_ipfix_message( int sourceid, int ipfix_version, bool _encode_network):
		message(NULL), g_tstart(0), encode_network(_encode_network)
{

    switch( ipfix_version ) 
    {
      case IPFIX_VERSION_NF9:
          break;
      case IPFIX_VERSION:
          break;
      default:
          throw mnslp_ipfix_bad_argument("Invalid IP Fix Version");
    }
    
    init( sourceid, ipfix_version );

}


mnslp_ipfix_message::~mnslp_ipfix_message( void )
{

    close( );
    g_tstart = 0;
    
}


/*
 * name:        ipfix_add_vendor_information_elements()
 * parameters:  > fields - array of fields of size nfields+1
 *                         the last field has ftype = 0
 * description: add information elements to global list of field types
 * remarks:
 */
void mnslp_ipfix_message::add_vendor_information_elements( ipfix_field_type_t *fields )
{
    ipfix_field_type_t *ft;

    if ( ! g_tstart ) {  
        throw mnslp_ipfix_bad_argument("Message not initialized");
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

}

void mnslp_ipfix_message::allocate_additional_memory(size_t additional)
{
	if (message)
	{
		if ((message->offset + additional) > message->buffer_lenght)
		{
			message->buffer=(char *)realloc(message->buffer, message->offset + additional + 1);
			if (message->buffer == NULL)
				message->buffer_lenght = message->offset + additional + 1;
			else
				throw mnslp_ipfix_bad_argument("Could not increse the memory of the final char pointer");
		}
	}
	else
	{
		throw mnslp_ipfix_bad_argument("Not initializated message");
	}
}



/**
 * name:        init()
 * parameters:
 * remarks:     init module, read field type info.
 */
void mnslp_ipfix_message::init( int sourceid, int ipfix_version )
{

    ipfix_t       *i;

	try
	{
		i = new ipfix_t;
		i->buffer = new char[IPFIX_DEFAULT_BUFLEN];
		i->buffer_lenght = IPFIX_DEFAULT_BUFLEN;
		i->cs_tid = 0;
		i->cs_bytes = 0;
		i->cs_header = NULL;
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
    i->nrecords  = 0;
    message = i;


    g_tstart = time(NULL);
    signal( SIGPIPE, SIG_IGN );
    g_lasttid = 255;

    g_ipfix_fields.initialize_forward();

    g_ipfix_fields.initialize_reverse();

}


/*
 * name:        new_data_template()
 * parameters:
 * return:      
 */
uint16_t 
mnslp_ipfix_message::new_data_template( int nfields )
{
    uint16_t templid;
    mnslp_ipfix_template  *templ;

    templid = new_template( nfields );
    templ = message->templates.get_template(templid);
    templ->set_type(DATA_TEMPLATE);
    return templid;
    
}

/*
 * name:        ipfix_new_option_template()
 * parameters:
 * return:      
 */
uint16_t 
mnslp_ipfix_message::new_option_template( int nfields )
{
    uint16_t templid;
    mnslp_ipfix_template  *templ;

    templid =  new_template( nfields );
    templ = message->templates.get_template(templid);
    templ->set_type(OPTION_TEMPLATE);
    return templid;
}


/*
 * name:        new_template()
 * parameters:
 * return:     
 */
uint16_t  
mnslp_ipfix_message::new_template( int nfields )
{
    mnslp_ipfix_template  *t;

    if ( ! message  ) 
        throw mnslp_ipfix_bad_argument("Message is not initialized");
    
    if ( nfields<1 )
		throw mnslp_ipfix_bad_argument("Invalid number of fields");

    /** alloc mem
     */
    try
    {
		
		t = new mnslp_ipfix_template();
		
		/** generate template id, todo!
		 */
		g_lasttid++;
		t->set_id( g_lasttid );
		t->set_maxfields( nfields );

		/** add template to template container
		 */
		(message->templates).add_template(t);
		
		return t->get_template_id();
	
	}
	catch (std::bad_alloc& exc)
	{
		if (t != NULL)
			delete t;
		throw std::bad_alloc(exc);
	}

}

mnslp_ipfix_field
mnslp_ipfix_message::get_field_definition( int eno, int type )
{
	mnslp_ipfix_field *field = g_ipfix_fields.get_field(eno, type);
	
	if (field == NULL)
		throw mnslp_ipfix_bad_argument("Field not found in the container");
	else
		return *field;
}


/*
 * name:        add_field()
 * parameters:
 * return:      
 */
void 
mnslp_ipfix_message::add_field(  uint16_t templid,
               uint32_t         eno,
               uint16_t         type,
               uint16_t         length )
{
    
    mnslp_ipfix_template *templ;     
    templ = message->templates.get_template(templid);
    
    if (templ== NULL)
		throw mnslp_ipfix_bad_argument("template not included in the message");

    if ( templ->get_type() == OPTION_TEMPLATE )
		throw mnslp_ipfix_bad_argument("The template is not a data template");    

    
    if ( ( templ->get_numfields() < templ->get_maxfields() )
         && (type < IPFIX_EFT_VENDOR_BIT) ) {
        /** set template field
         */

        mnslp_ipfix_field * field = g_ipfix_fields.get_field(eno, type);
        if (field==NULL)
            throw mnslp_ipfix_bad_argument("Field not found in the container");
        
        templ->add_field(length,0,0,field);
    }
    else 
        throw mnslp_ipfix_bad_argument("Maximum number of field reach");
    
}

/*
 * name:        add_scope_field()
 * parameters:
 * return:      
 */
void 
mnslp_ipfix_message::add_scope_field( uint16_t templid,
                     uint32_t         eno,
                     uint16_t         type,
                     uint16_t         length )
{
    int i;
    mnslp_ipfix_template *templ; 
    
    templ = message->templates.get_template(templid);

    if (templ == NULL)
		throw mnslp_ipfix_bad_argument("template not included in the message");

    if ( templ->get_type() != OPTION_TEMPLATE )
		throw mnslp_ipfix_bad_argument("The template is not an option template");    

    if ( templ->get_numfields() < templ->get_maxfields() ) {

        mnslp_ipfix_field * field = g_ipfix_fields.get_field(eno, type);
        if (field==NULL) 
            throw mnslp_ipfix_bad_argument("Field not found in the collection"); 
        
        templ->add_field(length,0,0,field);
    }
    else 
        throw mnslp_ipfix_bad_argument("Maximum number of field reach");
}

/*
 * name:        delete_template()
 * parameters:
 * return:
 */
void mnslp_ipfix_message::delete_template( uint16_t templid )
{

    /** remove template from list
     */
    
    message->templates.delete_template(templid);

}

/*
 * name:        delete_all_templates()
 * parameters:
 * return:
 */
void mnslp_ipfix_message::delete_all_templates( void )
{
    /** remove all templates from list
     */
    
    message->templates.delete_all_templates();

}


/*
 * name:        ipfix_make_template()
 * parameters:
 * return:      generates a new template and stores a pointer to it into the templ parameter
 */
uint16_t 
mnslp_ipfix_message::make_template( export_fields_t *fields, 
									    int nfields )
{
    mnslp_ipfix_template *t;
    uint16_t templid;
    int i;

    templid = new_data_template( nfields );

	try
	{
		for ( i=0; i<nfields; i++ ){
			std::cout << "i:" << i << std::endl;
			add_field( templid, fields[i].eno, fields[i].ienum, fields[i].length);
		}
				
	}
	catch(...)
	{
		delete_template( templid );
		throw mnslp_ipfix_bad_argument("Could not insert all fields"); 
	}

    return templid;
}

void mnslp_ipfix_message::finish_cs( void )
{
    size_t   buflen;
    uint8_t  *buf;
	std::string 	func = "finish_cs";

	std::cout << "In function:" << func << std::endl;

    /* finish current dataset */
    if ( (buf = message->cs_header) ==NULL )
        return;
    buflen = 0;
    
    std::cout << "In function:" << func << "num_bytes:" << message->cs_bytes << std::endl;
    
    if (encode_network == true){
		INSERTU16( buf+buflen, buflen, message->cs_tid );
		INSERTU16( buf+buflen, buflen, message->cs_bytes );
	}
	else{
		INSERT_U16_NOENCODE( buf+buflen, buflen, message->cs_tid );
		INSERT_U16_NOENCODE( buf+buflen, buflen, message->cs_bytes );		
	}
    message->cs_bytes = 0;
    message->cs_header = NULL;
    message->cs_tid = 0;
}

/*
 * name:        _write_hdr()
 * parameters:
 * return:      0/-1
 */
void mnslp_ipfix_message::_write_hdr( void )
{
    int hsize, buflen = 0;
    time_t      now = time(NULL);

    /** fill ipfix header
     */
    char *buf; 
    if ( message->version == IPFIX_VERSION_NF9 ) 
    {
        hsize = 20;
        
        if ( hsize + message->offset > message->buffer_lenght ) 
			allocate_additional_memory(hsize + message->offset - message->buffer_lenght );
        
        /* write header before any other data */
		if ( message->offset > 0 ) 
			memmove( message->buffer + hsize, message->buffer, message->offset );

        message->seqno++;
        if (encode_network == true){
			INSERTU16( message->buffer+buflen, buflen, message->version );
			INSERTU16( message->buffer+buflen, buflen, message->nrecords );
			INSERTU32( message->buffer+buflen, buflen, ((now-g_tstart)*1000));
			INSERTU32( message->buffer+buflen, buflen, now );
			INSERTU32( message->buffer+buflen, buflen, message->seqno );
			INSERTU32( message->buffer+buflen, buflen, message->sourceid );
		}
		else{
			INSERT_U16_NOENCODE( message->buffer+buflen, buflen, message->version );
			INSERT_U16_NOENCODE( message->buffer+buflen, buflen, message->nrecords );
			INSERT_U32_NOENCODE( message->buffer+buflen, buflen, ((now-g_tstart)*1000));
			INSERT_U32_NOENCODE( message->buffer+buflen, buflen, now );
			INSERT_U32_NOENCODE( message->buffer+buflen, buflen, message->seqno );
			INSERT_U32_NOENCODE( message->buffer+buflen, buflen, message->sourceid );		
		}
		message->offset += hsize;
    }
    else 
    {
        hsize = IPFIX_HDR_BYTES;
        
        if ( hsize + message->offset > message->buffer_lenght ) 
			allocate_additional_memory(hsize + message->offset - message->buffer_lenght );

        /* write header before any other data */
		if ( message->offset > 0 )
			memmove( message->buffer + hsize, message->buffer, message->offset );
		
        if (encode_network == true){
			INSERTU16( message->buffer+buflen, buflen, message->version );
			INSERTU16( message->buffer+buflen, buflen, message->offset + IPFIX_HDR_BYTES );
			INSERTU32( message->buffer+buflen, buflen, now );
			INSERTU32( message->buffer+buflen, buflen, message->nrecords );
			INSERTU32( message->buffer+buflen, buflen, message->sourceid );
		}
		else{
			INSERT_U16_NOENCODE( message->buffer+buflen, buflen, message->version );
			INSERT_U16_NOENCODE( message->buffer+buflen, buflen, message->offset + IPFIX_HDR_BYTES );
			INSERT_U32_NOENCODE( message->buffer+buflen, buflen, now );
			INSERT_U32_NOENCODE( message->buffer+buflen, buflen, message->nrecords );
			INSERT_U32_NOENCODE( message->buffer+buflen, buflen, message->sourceid );		
		}
		message->length = message->offset + IPFIX_HDR_BYTES;
		message->exporttime = now;
		message->offset += hsize;
		
    }

}



/* name:        _output_flush()
 * parameters:
 * remarks:    
 */
void mnslp_ipfix_message::_output_flush( void )
{
    int               ret;
    std::string 	  func = "_output_flush";

	std::cout << "In function:" << func << std::endl; 

    if ( (message==NULL) || (message->offset==0) )
        return;
    
    if ( message->cs_tid > 0 ) {
        /* finish current dataset */
        finish_cs( );
    }
    
    _write_hdr( );
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
		delete (message->buffer);
        delete (message);
    }
}



/*
 * name:        _write_template()
 * parameters:
 * return:      0/-1
 */
void 
mnslp_ipfix_message::_write_template( mnslp_ipfix_template  *templ )
{
    size_t            buflen, tsize=0, ssize=0, osize=0;
    char              *buf;
    uint16_t          tmp16;
    int               i, n;

    /** calc template size
     */
    if ( templ->get_type() == OPTION_TEMPLATE ) {
        for ( i=0, ssize=0; i<templ->get_number_scopefields(); i++ ) {
            ssize += 4;
            if (templ->get_field(i).elem->get_field_type().eno != IPFIX_FT_NOENO)
                ssize += 4;
        }
        for ( osize=0; i<templ->get_numfields(); i++ ) {
            osize += 4;
            if (templ->get_field(i).elem->get_field_type().eno != IPFIX_FT_NOENO)
                osize += 4;
        }
        tsize = 10 + osize + ssize;
    } 
    else {
        for ( tsize=8,i=0; i < templ->get_numfields(); i++ ) {
            tsize += 4;
            if (templ->get_field(i).elem->get_field_type().eno != IPFIX_FT_NOENO)
                tsize += 4;
        }
    }

    std::cout << "template length:" << tsize << "offset:" << message->offset << std::endl;

    /* check space */
    if ( tsize + message->offset > message->buffer_lenght ) 
    {
         allocate_additional_memory(tsize + message->offset - message->buffer_lenght );
    }

    /* write template prior to data */
    if ( message->offset > 0 ) 
    {
         memmove( message->buffer + tsize, message->buffer, message->offset );
         if ( message->cs_tid )
             message->cs_header += tsize;          
    }

    buf = message->buffer;
    buflen = 0;

    /** insert template set into buffer
     */
    if ( message->version == IPFIX_VERSION_NF9 ) {
        if ( templ->get_type() == OPTION_TEMPLATE ) {
			if (encode_network == true){
				INSERTU16( buf+buflen, buflen, IPFIX_SETID_OPTTEMPLATE_NF9);
				INSERTU16( buf+buflen, buflen, tsize );
				INSERTU16( buf+buflen, buflen, templ->get_template_id() );
				INSERTU16( buf+buflen, buflen, ssize );
				INSERTU16( buf+buflen, buflen, osize );
			}
			else{
				INSERT_U16_NOENCODE( buf+buflen, buflen, IPFIX_SETID_OPTTEMPLATE_NF9);
				INSERT_U16_NOENCODE( buf+buflen, buflen, tsize );
				INSERT_U16_NOENCODE( buf+buflen, buflen, templ->get_template_id() );
				INSERT_U16_NOENCODE( buf+buflen, buflen, ssize );
				INSERT_U16_NOENCODE( buf+buflen, buflen, osize );				
			}
        } 
        else {
            if (encode_network == true){
				INSERTU16( buf+buflen, buflen, IPFIX_SETID_TEMPLATE_NF9);
				INSERTU16( buf+buflen, buflen, tsize );
				INSERTU16( buf+buflen, buflen, templ->get_template_id() );
				INSERTU16( buf+buflen, buflen, templ->get_numfields() );
			}
			else{
				INSERT_U16_NOENCODE( buf+buflen, buflen, IPFIX_SETID_TEMPLATE_NF9);
				INSERT_U16_NOENCODE( buf+buflen, buflen, tsize );
				INSERT_U16_NOENCODE( buf+buflen, buflen, templ->get_template_id() );
				INSERT_U16_NOENCODE( buf+buflen, buflen, templ->get_numfields() );
			}
        }
    } 
    else 
    {
        if ( templ->get_type() == OPTION_TEMPLATE ) {
			if (encode_network == true){
				INSERTU16( buf+buflen, buflen, IPFIX_SETID_OPTTEMPLATE );
				INSERTU16( buf+buflen, buflen, tsize );
				INSERTU16( buf+buflen, buflen, templ->get_template_id() );
				INSERTU16( buf+buflen, buflen, templ->get_numfields() );
				INSERTU16( buf+buflen, buflen, templ->get_number_scopefields() );
			}
			else{
				INSERT_U16_NOENCODE( buf+buflen, buflen, IPFIX_SETID_OPTTEMPLATE );
				INSERT_U16_NOENCODE( buf+buflen, buflen, tsize );
				INSERT_U16_NOENCODE( buf+buflen, buflen, templ->get_template_id() );
				INSERT_U16_NOENCODE( buf+buflen, buflen, templ->get_numfields() );
				INSERT_U16_NOENCODE( buf+buflen, buflen, templ->get_number_scopefields() );				
			}
        } 
        else {
            if (encode_network == true){
				INSERTU16( buf+buflen, buflen, IPFIX_SETID_TEMPLATE);
				INSERTU16( buf+buflen, buflen, tsize );
				INSERTU16( buf+buflen, buflen, templ->get_template_id() );
				INSERTU16( buf+buflen, buflen, templ->get_numfields() );
			}
			else{
				INSERT_U16_NOENCODE( buf+buflen, buflen, IPFIX_SETID_TEMPLATE);
				INSERT_U16_NOENCODE( buf+buflen, buflen, tsize );
				INSERT_U16_NOENCODE( buf+buflen, buflen, templ->get_template_id() );
				INSERT_U16_NOENCODE( buf+buflen, buflen, templ->get_numfields() );				
			}
        }
    }

    if ( templ->get_type() == OPTION_TEMPLATE ) 
    {
        n = templ->get_numfields();
        for ( i=0; i<templ->get_number_scopefields(); i++ ) {
            if ( templ->get_field(i).elem->get_field_type().eno == IPFIX_FT_NOENO ) {
				if (encode_network == true){
					INSERTU16( buf+buflen, buflen, templ->get_field(i).elem->get_field_type().ftype );
					INSERTU16( buf+buflen, buflen, templ->get_field(i).flength );
				}
				else{
					INSERT_U16_NOENCODE( buf+buflen, buflen, templ->get_field(i).elem->get_field_type().ftype );
					INSERT_U16_NOENCODE( buf+buflen, buflen, templ->get_field(i).flength );
				}
            } 
            else {
                tmp16 = templ->get_field(i).elem->get_field_type().ftype|IPFIX_EFT_VENDOR_BIT;
                if (encode_network == true){
					INSERTU16( buf+buflen, buflen, tmp16 );
					INSERTU16( buf+buflen, buflen, templ->get_field(i).flength );
					INSERTU32( buf+buflen, buflen, templ->get_field(i).elem->get_field_type().eno );
				}
				else{
					INSERT_U16_NOENCODE( buf+buflen, buflen, tmp16 );
					INSERT_U16_NOENCODE( buf+buflen, buflen, templ->get_field(i).flength );
					INSERT_U32_NOENCODE( buf+buflen, buflen, templ->get_field(i).elem->get_field_type().eno );					
				}
            }
        }
    } 
    else {
        i = 0;
        n = templ->get_numfields();
    }

    for ( ; i<templ->get_numfields(); i++ )
    {
        std::cout << "Field Num:" << i << std::endl;
        if ( templ->get_field(i).elem->get_field_type().eno == IPFIX_FT_NOENO ) {
			if (encode_network == true){
				INSERTU16( buf+buflen, buflen, templ->get_field(i).elem->get_field_type().ftype );
				INSERTU16( buf+buflen, buflen, templ->get_field(i).flength );
			}
			else{
				INSERT_U16_NOENCODE( buf+buflen, buflen, templ->get_field(i).elem->get_field_type().ftype );
				INSERT_U16_NOENCODE( buf+buflen, buflen, templ->get_field(i).flength );				
			}
        } 
        else {
            tmp16 = templ->get_field(i).elem->get_field_type().ftype|IPFIX_EFT_VENDOR_BIT;
            if (encode_network == true){
				INSERTU16( buf+buflen, buflen, tmp16 );
				INSERTU16( buf+buflen, buflen, templ->get_field(i).flength );
				INSERTU32( buf+buflen, buflen, templ->get_field(i).elem->get_field_type().eno );
			}
			else{
				INSERT_U16_NOENCODE( buf+buflen, buflen, tmp16 );
				INSERT_U16_NOENCODE( buf+buflen, buflen, templ->get_field(i).flength );
				INSERT_U32_NOENCODE( buf+buflen, buflen, templ->get_field(i).elem->get_field_type().eno );				
			}
        }
    }
    templ->set_time_send( time(NULL) );

	std::cout << "buffer len:" << buflen << std::endl;
	
    message->offset += buflen;
    if ( message->version == IPFIX_VERSION_NF9 )
         message->nrecords++;

}

void 
mnslp_ipfix_message::output( uint16_t templid )
{
    int               i, newset_f=0;
    size_t            buflen, datasetlen;
    uint8_t           *p, *buf;
    mnslp_ipfix_template * templ; 
    
    templ = message->templates.get_template(templid);

    /** parameter check
     */
    if (templ==NULL)
        throw mnslp_ipfix_bad_argument("template not included in the message");
    	
    /** writes the templates if it was not done before
     */
    if ( templ->get_tsend() == 0 )
        _write_template( templ ); 
    

    /** get size of data set, check space
    */ 
    if ( templ->get_template_id() == message->cs_tid ) 
    {
        std::cout << "No new set" << std::endl;
        newset_f = 0;
        datasetlen = 0;
    }
    else 
    {
        std::cout << "New set" << message->cs_tid << std::endl;
        if ( message->cs_tid > 0 ) {
            finish_cs( );
        }
        newset_f = 1;
        datasetlen = 4;
    }
        	
    // insert the data records associated with the template.
    for ( int data_index= 0; data_index < data_list.size(); data_index++){
		
		if (data_list[data_index].get_template_id() == templ->get_template_id()){
			
			mnslp_ipfix_data_record g_data = data_list[data_index];
			
			for ( i=0; i < templ->get_numfields(); i++ ){
				mnslp_ipfix_field_key field_key = mnslp_ipfix_field_key(templ->get_field(i).elem->get_field_type().eno, 
																		templ->get_field(i).elem->get_field_type().ftype);
				if ( templ->get_field(i).flength == IPFIX_FT_VARLEN ) {
					
					if ( g_data.get_length(field_key) > 254 )
						datasetlen += 3;
					else
						datasetlen += 1;
				} 
				else 
					if ( g_data.get_length(field_key) > templ->get_field(i).flength )
						throw mnslp_ipfix_bad_argument("Data length greater than field definition lenght");
						
				datasetlen += g_data.get_length(field_key);
			}

			std::cout << "data set length" << datasetlen << std::endl;


			if ( (message->offset + datasetlen) > message->buffer_lenght )
				allocate_additional_memory(datasetlen + message->offset - message->buffer_lenght );
			
			std::cout << "Message offset before inserting data:" << message->offset << std::endl;

			// fill buffer 
			buf    = (uint8_t*)(message->buffer) + message->offset;
			buflen = 0;

			if ( newset_f ) {
				
				std::cout << "Inside new set - Copying data" << std::endl;
				// insert data set 
				message->cs_bytes = 0;
				message->cs_header = buf;
				message->cs_tid = templ->get_template_id();
				
				
				if (encode_network == true){
					INSERTU16( buf+buflen, buflen, templ->get_template_id() );
					INSERTU16( buf+buflen, buflen, datasetlen );
				}
				else{
					INSERT_U16_NOENCODE( buf+buflen, buflen, templ->get_template_id() );
					INSERT_U16_NOENCODE( buf+buflen, buflen, datasetlen );			
				}
			}
			
			for ( i=0; i < templ->get_numfields(); i++ ) {
				
				std::cout << "writing field:" << i << std::endl;
				
				mnslp_ipfix_field_key field_key = mnslp_ipfix_field_key(templ->get_field(i).elem->get_field_type().eno, 
																		templ->get_field(i).elem->get_field_type().ftype);
				
				if ( templ->get_field(i).flength == IPFIX_FT_VARLEN ) {
					if ( g_data.get_length(field_key) > 254 ) {
						*(buf+buflen) = 0xFF;
						buflen++;
						INSERTU16( buf+buflen, buflen, g_data.get_length(field_key) );
					}
					else {
						*(buf+buflen) = g_data.get_length(field_key);
						buflen++;
					}
				}
				
				if ( templ->get_field(i).relay_f ) {
					templ->get_field(i).elem->ipfix_encode_bytes( g_data.get_field(field_key), 
																  buf+buflen, (size_t) 
																  templ->get_field(i).relay_f ); // no encoding 
				}
				else {
					templ->get_field(i).elem->encode( g_data.get_field(field_key), 
													  buf+buflen, 
													  templ->get_field(i).relay_f );
				}
				buflen += g_data.get_length(field_key);
			}
			message->nrecords ++;
			message->offset += buflen;
			message->cs_bytes += buflen;
			finish_cs( );
		    if ( message->version == IPFIX_VERSION )
				message->seqno ++;
		}
	}
		
	std::cout << "End of the data encode:" << message->offset << std::endl;
    
    _output_flush( );
    
}


void 
mnslp_ipfix_message::include_data( uint16_t templid, 
								   mnslp_ipfix_data_record &data )
{
    int i;
	mnslp_ipfix_template *templ;
	
	templ = message->templates.get_template(templid);
	
    if ( !templ ) {
        throw mnslp_ipfix_bad_argument("Parameter template not defined");
    }
       
    if ( ( templ->get_numfields() != data.get_num_fields())  ){
        throw mnslp_ipfix_bad_argument("The number of field values is diferent from template's fields");
	}
				
    data_list.push_back(data);
}

char * 
mnslp_ipfix_message::get_message(void)
{
	return message->buffer;
}

int 
mnslp_ipfix_message::get_offset(void)
{
	return message->offset;
}

mnslp_ipfix_message::mnslp_ipfix_message(char * param, size_t message_length, bool _encode_network):
	message(NULL), g_tstart(0), encode_network(_encode_network)
{
	int nrecords;
	nrecords = mnslp_ipfix_import(param, message_length );
	
	std::cout << "bytes read:" << nrecords << std::endl;
}


/*
 * name:        mnslp_ipfix_parse_hdr()
 * parameters:
 */
void 
mnslp_ipfix_message::mnslp_ipfix_parse_hdr( char *mes, int offset )
{
    uint16_t _count, _length, _version;
    uint32_t _sysuptime, _unixtime, _exporttime, _seqno, _sourceid; 
        
    std::string func = "mnslp_ipfix_parse_hdr";
    if (encode_network == true)
		_version = READ16(mes);
	else
		READ16_NOENCODE(_version,mes);

	std::cout << " In function" << func << std::endl;
	
    switch ( _version ) {
      case IPFIX_VERSION_NF9:

		  std::cout << " Arrive 1.3" << std::endl;
		  fflush( stdout) ;

          if ( (offset) < IPFIX_HDR_BYTES_NF9 )
              throw mnslp_ipfix_bad_argument("Invalid message given");
          
          if (encode_network == true)
          {
			_count = READ16(mes+2);
			_sysuptime = READ32(mes+4);
			_unixtime = READ32(mes+8);
			_seqno = READ32(mes+12);
			_sourceid = READ32(mes+16);
		  }
		  else
		  {
			 READ16_NOENCODE(_count, mes+2);
			 READ32_NOENCODE(_sysuptime,mes+4);
			 READ32_NOENCODE(_unixtime, mes+8);
			 READ32_NOENCODE(_seqno, mes+12);
			 READ32_NOENCODE(_sourceid, mes+16);			 
		  }
          
          /* Initialize the message object */
		  init(_sourceid, _version); 
		  message->version = _version;
		  message->count = _count;
          message->sysuptime = _sysuptime;
          message->unixtime = _unixtime;
          message->seqno = _seqno;
          message->sourceid = _sourceid;
          break;

      case IPFIX_VERSION:

		  std::cout << " Arrive 1.4" << std::endl;

          if ( (offset) < IPFIX_HDR_BYTES )
              throw mnslp_ipfix_bad_argument("Length of the message header is less than required");

		  if (encode_network == true){
		      _length = READ16(mes+2);
		      _exporttime = READ32(mes+4);
		      _seqno = READ32(mes+8);
		      _sourceid = READ32(mes+12);
		  }
		  else{
			  READ16_NOENCODE(_length,mes+2);
			  READ32_NOENCODE(_exporttime,mes+4);
			  READ32_NOENCODE(_seqno,mes+8);
			  READ32_NOENCODE(_sourceid,mes+12);
		  }
          std::cout << "header received" << _version 
					<< "length:"         << _length
					<< "exporttime:"	 << _exporttime
					<< "seqno:"			 << _seqno
					<< "sourceid:"		 << _sourceid << std::endl;

		  /* Initialize the message object */
		  init(_sourceid, _version); 
          message->version = _version;
          message->length = _length;
          message->exporttime = _exporttime;
          message->seqno = _seqno;
          message->sourceid = _sourceid;
          break;

      default:
		  std::cout << " Arrive 1.5" << std::endl;
          message->version = -1;
          throw mnslp_ipfix_bad_argument("Invalid Message Version");
    }
	std::cout << " Arrive 1.2" << std::endl;
	
}


/*
 * name:        mnslp_ipfix_decode_trecord()
 * func:        create or update template inside ifh. 
 * return:      0/-1
 */
void
mnslp_ipfix_message::mnslp_ipfix_decode_trecord( int setid,
								const char     *buf2,
								size_t         len,
								int            *nread )
{
    mnslp_ipfix_template *templ;
    uint16_t          templid, nfields, nscopefields, ndatafields;
    int               i, newnode =0;
    size_t            offset;
    std::string       func = "ipfix_decode_trecord";

    errno = EIO;
    const uint8_t * buf = reinterpret_cast<const uint8_t*>(buf2);

    /** read template header
     */
    switch( setid ) {
      case IPFIX_SETID_OPTTEMPLATE:
          
          std::cout << "Reading option template" << std::endl;
          
          if ( len<6 )
              throw mnslp_ipfix_bad_argument("invalid message lenght");
          if (encode_network == true){    
		      templid      = READ16(buf);
			  nfields      = READ16(buf+2);
              nscopefields = READ16(buf+4);
		  }
		  else{
		      READ16_NOENCODE(templid,buf);
			  READ16_NOENCODE(nfields,buf+2);
              READ16_NOENCODE(nscopefields,buf+4);			  
		  }
          offset = 6;
          ndatafields = nfields - nscopefields;
          break;
      case IPFIX_SETID_OPTTEMPLATE_NF9:
      {
          std::cout << "Reading option template NF9" << std::endl;
          
          size_t scopelen, optionlen;
          if ( len<6 )
              throw mnslp_ipfix_bad_argument("invalid message lenght");
          
          if (encode_network == true){
			  templid   = READ16(buf);
			  scopelen  = READ16(buf+2);
			  optionlen = READ16(buf+4);
		  }
		  else{
			  READ16_NOENCODE(templid, buf);
			  READ16_NOENCODE(scopelen,buf+2);
			  READ16_NOENCODE(optionlen, buf+4);			  
	      }
		  
          offset = 6;
          if ( (scopelen+optionlen) < len ) {
              throw mnslp_ipfix_bad_argument("Invalid template data");
          } 
          nscopefields = scopelen / 4;
          ndatafields  = optionlen / 4;
          nfields   = nscopefields + ndatafields;
          break;
      }
      case IPFIX_SETID_TEMPLATE:
      case IPFIX_SETID_TEMPLATE_NF9:
          
          std::cout << "Reading data template" << std::endl;
          
          if ( len<4 )
             throw mnslp_ipfix_bad_argument("invalid message lenght");
          if (encode_network == true){
			  templid   = READ16(buf);
              nfields   = READ16(buf+2);
          }
          else{
			  READ16_NOENCODE(templid,buf);
              READ16_NOENCODE(nfields,buf+2);
		  }
		  std::cout << "template information:" << "templid:" << templid
					<< "nfields:" << nfields << std::endl;
					
          offset = 4;
          ndatafields  = nfields;
          nscopefields = 0;
          break;
      default:
          throw mnslp_ipfix_bad_argument("Invalid template the setid is wrong");
    }

    if ( nfields == 0 ) {
        /** 
         * template withdrawal message
         */
        if ( templid == setid ) {
            delete_all_templates();
        }
        else{
			message->templates.delete_template(templid);
		}

        *nread = offset;
    }


    /** 
     * gets template node ( create or obtain it from the message).
     */
    mnslp_ipfix_template * t = message->templates.get_template(templid);
    if ( (t == NULL)
         || (nfields > t->get_numfields() ) ) {
		
		
		
        if ( t != NULL ){
           // Replace the template.
           message->templates.delete_template( templid );
           delete(t);
        }
		/** alloc mem
		*/
		switch( setid ) {
			case IPFIX_SETID_OPTTEMPLATE:
			case IPFIX_SETID_OPTTEMPLATE_NF9:
				std::cout << "creating a new option template" << std::endl;
				templid = new_option_template(nfields);
				break;
			case IPFIX_SETID_TEMPLATE:
			case IPFIX_SETID_TEMPLATE_NF9:
				std::cout << "creating a new data template" << std::endl;
				templid = new_data_template( nfields );
				break;
		}
		t = message->templates.get_template(templid);	
		newnode =1;
    }
    else {
        newnode =0;
        t->remove_unknown_fields();
    }

    t->set_id(templid);
    t->set_maxfields(nfields);
    
    std::cout << "type of template:" << t->get_type() << std::endl;

    /** read field definitions
     */
    try
    {
		for( i=0;  i < nfields; i++ ) {
			if (offset >= len)
				throw mnslp_ipfix_bad_argument("Field in template has a longer length than message"); 
			else
				read_field(t, buf+offset, len-offset, &offset);
		}
		*nread = offset;		
	}
	catch (mnslp_ipfix_bad_argument bad)
	{
		if ( newnode ) {
			delete(t);
		}
		else {
			message->templates.delete_template( t->get_template_id() );
		}
		throw mnslp_ipfix_bad_argument("Could not read the template information"); 
	}

}


int
mnslp_ipfix_message::get_num_templates(void)
{
	if (message == NULL)
		return 0;
	else
		return (message->templates).get_num_templates();
}

void 
mnslp_ipfix_message::read_field(mnslp_ipfix_template *templ, 
							    const uint8_t  *buf, 
							    size_t   buflen, 
							    size_t   *nread)
{
    int ftype, eno;
    uint16_t length;
    int unknown_f;
    int relay_f;
    
    if (encode_network == true)
		relay_f = 1;
	else 
		relay_f = 0;    

    std::cout << "In read field - Template Id:" << templ->get_template_id() << std::endl;

    if ( buflen<4 ) 
    {
        throw mnslp_ipfix_bad_argument("Buffer has not enough information to read a field");
    }
    
    if (templ== NULL){
		throw mnslp_ipfix_bad_argument("Invalid template definition for reading a field");
	}
    
    // Verifies that the field can be added
    if ( ( templ->get_numfields() >= templ->get_maxfields() )){
		throw mnslp_ipfix_bad_argument("It cannot insert the new field, max field reached");	
	}
    
    // Reads the field from the buffer.
    if (encode_network == true){
		ftype          = READ16(buf);
		length 		   = READ16(buf+2);
	}
	else{
		READ16_NOENCODE(ftype, buf);
		READ16_NOENCODE(length, buf+2);		
	}
    (*nread) += 4;

    if ( ftype & IPFIX_EFT_VENDOR_BIT ) {
        if ( buflen<8 ) {
            throw mnslp_ipfix_bad_argument("Invalid buffer len for reading a vendor field");
        }
        ftype &= (~IPFIX_EFT_VENDOR_BIT);
        if (encode_network == true)
			eno = READ32(buf+4);
		else
			READ32_NOENCODE(eno,buf+4);
        (*nread) += 4;
    } 
    else {
        eno = IPFIX_FT_NOENO;
    }
	
	std::cout << "template field read-" << "ftype:" << ftype
			  << "eno:" << eno << "length" << length << std::endl;
	
    try
    {
        mnslp_ipfix_field * field = g_ipfix_fields.get_field(eno, ftype);
        unknown_f =0;
        templ->add_field(length, unknown_f, relay_f, field);
		
		
		std::cout << "found the field:" << templ->get_numfields() 
				  << "Nbr scope fields" << templ->get_number_scopefields() 
				  << "Nbr data fields" << templ->get_number_datafields() 
				  << std::endl;
		
	}
	catch(mnslp_ipfix_bad_argument e)
	{	
        /** unknown field -> generate node
         */
        unknown_f =1;  /* mark node, so we can drop it later */
        mnslp_ipfix_field * field = g_ipfix_fields.get_field(0, 0);
        templ->add_field(length,unknown_f,relay_f,field);
        std::cout << "No found the field" << std::endl;
	}
	
	std::cout << "number of fields after insert" << templ->get_numfields() << std::endl;
	
}


/*
 * name:        mnslp_ipfix_decode_datarecord()
 * parameters:
 * desc:        this func parses and exports the ipfix data set
 * todo:        parse message before calling this func
 */
void mnslp_ipfix_message::mnslp_ipfix_decode_datarecord( mnslp_ipfix_template *templ,
								   char      		  *buf, 
								   int                buflen,
								   int                *nread )
{
    uint8_t       *p;
    int           i, len, bytesleft;
    std::string   func = "mnslp_ipfix_decode_datarecord";
    mnslp_ipfix_data_record g_data(templ->get_template_id());
    char salida[30];
    
    std::cout << "In function:" << func << "buflen" << buflen << std::endl;

    /** parse message
     */
    bytesleft = buflen;
    *nread    = 0;
    p         = reinterpret_cast<uint8_t*>(buf);
    
    std::cout << "number of fields:" << templ->get_numfields() << std::endl;
    
    for ( i=0; i < templ->get_numfields(); i++ ) {

        len = templ->get_field(i).flength;
        if ( len == IPFIX_FT_VARLEN ) {
            len =*p;
            p++;
            (*nread) ++;
            if ( len == 255 ) {
				if (encode_network == true)
					len = READ16(p);
				else
					READ16_NOENCODE(len,p);
                p += 2;
                (*nread) +=2;
            }
        }
		std::cout << "in field to read" << "len:" << len << std::endl;
        bytesleft -= len;
        if ( bytesleft < 0 ) {
            throw mnslp_ipfix_bad_argument("Invalid buffer len for reading the data");
        }

        mnslp_ipfix_value_field value = templ->get_field(i).elem->decode(p,len, encode_network);

		templ->get_field(i).elem->snprint(salida, 30, value);
		
		std::cout << "value:" << salida << std::endl;
		
        g_data.insert_field(templ->get_field(i).elem->get_field_type().eno, 
						    templ->get_field(i).elem->get_field_type().ftype, value); 
						    
        p        += len;
        (*nread) += len;
    }
    
    data_list.push_back(g_data);

}

mnslp_ipfix_template * 
mnslp_ipfix_message::get_template(uint16_t templid)
{
	if (message != NULL)
		return message->templates.get_template(templid);
	else
		return NULL;
}


int 
mnslp_ipfix_message::mnslp_ipfix_import( char  *buffer,
									     size_t message_length )
{
    char                 *buf;                 /* ipfix payload */
    uint16_t             setid, setlen;        /* set id, set lenght */
    int                  i, nread, offset;     /* counter */
    int                  bytes, bytesleft;
    int                  err_flag = 0;
    std::string          func = "mnslp_ipfix_import";

	std::cout << " In function" << func << std::endl;

    if (message_length < 2)
		throw mnslp_ipfix_bad_argument("Invalid Message");

    mnslp_ipfix_parse_hdr( buffer, message_length );
	
    switch( message->version ) {
      case IPFIX_VERSION_NF9:
          buf   = buffer;
          nread = IPFIX_HDR_BYTES_NF9;
          break;
      case IPFIX_VERSION:
          buf   = buffer;
          nread = IPFIX_HDR_BYTES;
          break;
      default:
          throw mnslp_ipfix_bad_argument("Invalid IPFIX Version");
    }
	
	std::cout << "In function:" << func << "Arrive 2" << std::endl;

    /** read ipfix sets
     */
    for ( i=0; (nread+4) < message_length; i++ ) {


		std::cout << "In function:" << func << "reading data set " << i << std::endl;

        if ( (message->version == IPFIX_VERSION_NF9)
             && (i>=message->count) ) {
            break;
        }

        /** read ipfix record header (set id, lenght). Verifies that the lenght 
         *  given is valid.
         */
        if (encode_network == true){
			setid   = READ16(buf+nread);
			setlen  = READ16(buf+nread+2);
		}
		else{
			READ16_NOENCODE(setid, buf+nread);
			READ16_NOENCODE(setlen, buf+nread+2);
		}
		
		std::cout << "reading data set " << i << "setid:" << setid << "length:" << setlen << std::endl;
		
        nread  += 4;
        if ( setlen <4 ) {
            // std::string err1 = func + "set" + std::to_string(i+1) + ": invalid set length " + std::to_string(setlen);
            continue;
        }
        setlen -= 4;  // this corresponds to the record's header.
        if (setlen > (message_length - nread)) {
			int ii;

			for (ii = 0; ii < message_length; ii++)
				fprintf(stderr, "[%02x]", ((buffer)[ii] & 0xFF));
			fprintf(stderr, "\n");

			//std::string err2 = func + "set" + std::to_string(i+1) + ": message too short (" + std::to_string(setlen+nread) + ">" + std::to_string(offset)+ ")";
			goto end;
		}

		/** read rest of ipfix message
         */
        if ( (setid == IPFIX_SETID_TEMPLATE_NF9)
             || (setid == IPFIX_SETID_OPTTEMPLATE_NF9)
             || (setid == IPFIX_SETID_TEMPLATE)
             || (setid == IPFIX_SETID_OPTTEMPLATE) ) {
            /** parse a template set ( option or normal template ).
             */

            for (offset = nread, bytesleft = setlen; bytesleft > 4;) {

				// mlogf(4, "[%s] set%d: decode template, setlen=%d, left=%d\n",
				//		func, i + 1, setlen, bytesleft);

				mnslp_ipfix_decode_trecord(setid, buf + offset, bytesleft,&bytes);
				bytesleft -= bytes;
				offset += bytes;
				std::cout << "setid:" << i << "template-" <<" bytes decoded:" << bytes << std::endl;
			}
            nread += setlen;
        }
        else if ( setid > 255 )
        {
            /** get template
             */
            mnslp_ipfix_template *templ = get_template(setid);

            if ( templ == NULL ) {
                std::cout <<  "no template for" << setid << "skip data set" << std::endl;
                nread += setlen;
                err_flag = 1;
            }
            else {
				std::cout << "is going to read - setlen" << setlen << std::endl;
                /** read data records
                 */
                for ( offset=nread, bytesleft=setlen; bytesleft>0; ) {
                    mnslp_ipfix_decode_datarecord( templ, buf+offset, bytesleft,
                                                  &bytes );
                    
                    bytesleft -= bytes;
                    std::cout << "bytes read:"<< bytes << "bytes left:" << bytesleft << std::endl;
                    offset    += bytes;
                }

                if ( bytesleft ) {
                    // mlogf( 3, "[%s] set%d: skip %d bytes padding\n",
                    //       func, i+1, bytesleft );
                }
                nread += setlen;
            }
        }
        else {
            // mlogf( 0, "[%s] set%d: invalid set id %d, set skipped!\n",
            //       func, i+1, setid );
            nread += setlen;
        }
    } /* for (read sets */

    if ( err_flag )
        goto errend;

end:
    return nread;

 errend:
    data_list.clear();
    return -1;
	
}


bool 
mnslp_ipfix_message::operator== (mnslp_ipfix_message& rhs)
{
	
	std::cout << "here I am 1" << std::endl;
	
	if ((rhs.message == NULL) and (message == NULL))
		return true;

	std::cout << "here I am 2" << std::endl;

	if ((rhs.message != NULL) and (message == NULL))
		return false;

	std::cout << "here I am 3" << std::endl;

	if ((rhs.message == NULL) and (message != NULL))
		return false;

	std::cout << "here I am 4" << std::endl;
		
	// compare message header information.
	if ((rhs.message != NULL) and (message != NULL))
	
		if ((rhs.message)->sourceid != message->sourceid)
			return false;
		
		if ((rhs.message)->version != message->version)
			return false;

		if ((rhs.message)->seqno != message->seqno)
			return false;

		if ((rhs.message)->seqno != message->seqno)
			return false;
	
		std::cout << "here I am 5" << std::endl;
		
		if (message->version == IPFIX_VERSION_NF9)
			if (((rhs.message)->count != message->count ) ||
			   ((rhs.message)->sysuptime != message->sysuptime ) ||
			   ((rhs.message)->unixtime != message->unixtime ))
			   return false;

		std::cout << "here I am 5a:" 
				  << (rhs.message)->length << "d"
				  << (rhs.message)->exporttime << "e"
				  << message->length << "f"
				  << message->exporttime << "g"
				  <<std::endl;
		
		if (message->version == IPFIX_VERSION)
			if (((rhs.message)->length != message->length) ||
			   ((rhs.message)->exporttime != message->exporttime))
			   return false;

		std::cout << "here I am 5b" << std::endl;

		
		if (message->templates != rhs.message->templates)
			return false;

		std::cout << "here I am 6" << std::endl;
		
		try
		{
			for (int i = 0; i < data_list.size(); i++){
				std::cout << "here I am 7" << std::endl;
				if ( data_list[i] != rhs.data_list[i] )
					return false;
			}
		}
		catch(const std::out_of_range& oor)
		{
			std::cout << "here I am 8" << std::endl;
			return false;
		}
		
	
	return true;
			

}


}
