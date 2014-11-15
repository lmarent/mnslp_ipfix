/// ----------------------------------------*- mode: C; -*--
/// @file mnslp_libipfix.c
/// Tools for processing IPFIX messages in NSIS metering.
/// ----------------------------------------------------------
/// $Id: mnslp_libipfix.c 2558 2014-11-14 14:11:00 amarentes $
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

#include <ipfix.h>
#include <mlog.h>
#include "mnslp_libipfix.h"


#define NODEBUG
#define IPFIX_DEFAULT_BUFLEN  1400

//#define INSERTU16(b,l,val) \
//        { uint16_t _t=htons((val)); memcpy((b),&_t,2); (l)+=2; }
//#define INSERTU32(b,l,val) \
//        { uint32_t _t=htonl((val)); memcpy((b),&_t,4); (l)+=4; }

#define READ16(b) ((*(b)<<8)|*((b)+1))
#define READ32(b) ((((((*(b)<<8)|*(b+1))<<8)|(*(b+2)))<<8)|*(b+3))

#define INSERTU16(b,l,val) \
        { uint16_t _t=(val); memcpy((b),&_t,2); (l)+=2; }
#define INSERTU32(b,l,val) \
        { uint32_t _t=(val); memcpy((b),&_t,4); (l)+=4; }



typedef struct ipfixiobuf
{
    struct ipfixiobuf  *next;
    size_t             buflen;
    char               buffer[IPFIX_DEFAULT_BUFLEN+IPFIX_HDR_BYTES_NF9]; /*!!*/
} mnslp_iobuf_t;

static mnslp_iobuf_t            g_iobuf[2], *g_buflist =NULL;
static ipfix_datarecord_t 		g_mnslp_data = { NULL, NULL, 0 }; /* ipfix_export */

void allocate_additional_memory(mnslp_ipfix_message_t *mes, size_t additional)
{
	if (mes)
	{
		if ((mes->offset + additional) > sizeof(mes->buffer))
			mes->buffer=(char *)realloc(mes->buffer, mes->offset + additional + 1);
	}
}


/* Name      : do_writen
 * Parameter : > mes   		output message
 *             > *ptr       buffer
 *             > nbytes     number of bytes to write
 * Purpose   : write 'n' bytes to message
 * Returns   : number of written bytes.
 */
static int _mnslp_do_writen( mnslp_ipfix_message_t *mes, 
							 char *ptr, 
							 int nbytes )
{
    int     nleft, nwritten;
#ifdef DEBUG
    int     i;

    for ( i=0; i<nbytes; i++ )
        fprintf( stderr, "[%02x]", (ptr[i]&0xFF) );
    fprintf( stderr, "\n" );
#endif

    allocate_additional_memory(mes, nbytes);
    memcpy( mes->buffer+mes->offset, ptr, nbytes );
    return nbytes;
}


/*
 * name:        mnslp_ipfix_parse_hdr()
 * parameters:
 * return:      0/-1
 */
int mnslp_ipfix_parse_hdr( mnslp_ipfix_message_t *mes, ipfix_hdr_t *hdr )
{
    uint16_t version = READ16(mes->buffer);

	printf (" Arrive 1.1 [%u]", version);
	fflush( stdout) ;


    switch ( version ) {
      case IPFIX_VERSION_NF9:

		  printf (" Arrive 1.3");
		  fflush( stdout) ;

          if ( (mes->offset) < IPFIX_HDR_BYTES_NF9 )
              return -1;
          hdr->version = version;
          hdr->u.nf9.count = READ16(mes->buffer+2);
          hdr->u.nf9.sysuptime = READ32(mes->buffer+4);
          hdr->u.nf9.unixtime = READ32(mes->buffer+8);
          hdr->seqno = READ32(mes->buffer+12);
          hdr->sourceid = READ32(mes->buffer+16);
          break;

      case IPFIX_VERSION:

		  printf (" Arrive 1.4");
		  fflush( stdout) ;

          if ( (mes->offset) < IPFIX_HDR_BYTES )
              return -1;
          hdr->version = version;
          hdr->u.ipfix.length = READ16(mes->buffer+2);
          hdr->u.ipfix.exporttime = READ32(mes->buffer+4);
          hdr->seqno = READ32(mes->buffer+8);
          hdr->sourceid = READ32(mes->buffer+12);

          break;

      default:
		  printf (" Arrive 1.5");
		  fflush( stdout) ;

          hdr->version = -1;
          return -1;
    }
	printf (" Arrive 1.2");
	fflush( stdout) ;


    return 0;



}

ipfix_template_t * mnslp_get_template(ipfix_t 			   *ifh, 
									  uint16_t            tid_param)
{
	ipfix_template_t    *res = NULL;
	ipfix_template_t    *n = NULL;
	
	for ( n = ifh->templates; n!=NULL; n=n->next){
		if (n->tid == tid_param)
		{
			res = n;
			break;
		}
	}
	return res;
}


/*
 * name:        mnslp_ipfix_decode_trecord()
 * func:        create or update template inside ifh. 
 * return:      0/-1
 */
int mnslp_ipfix_decode_trecord( ipfix_t *ifh,
								int            setid,
								const char     *buf,
								size_t         len,
								int            *nread,
								ipfix_template_t **templ )
{
    ipfix_template_t  *t;
    uint16_t          templid, nfields, nscopefields, ndatafields;
    int               i, newnode =0;
    size_t            offset;
    char              *func = "ipfix_decode_trecord";

    errno = EIO;

    /** read template header
     */
    switch( setid ) {
      case IPFIX_SETID_OPTTEMPLATE:
          if ( len<6 )
              goto ioerr;
          templid      = READ16(buf);
          nfields      = READ16(buf+2);
          nscopefields = READ16(buf+4);
          offset = 6;
          ndatafields = nfields - nscopefields;
          break;
      case IPFIX_SETID_OPTTEMPLATE_NF9:
      {
          size_t scopelen, optionlen;

          if ( len<6 )
              goto ioerr;
          templid   = READ16(buf);
          scopelen  = READ16(buf+2);
          optionlen = READ16(buf+4);
          offset = 6;
          if ( (scopelen+optionlen) < len ) {
              mlogf( 1, "[%s] read invalid nf9 template %d\n", func, templid ); 
              return -1;
          } 
          nscopefields = scopelen / 4;
          ndatafields  = optionlen / 4;
          nfields   = nscopefields + ndatafields;
          break;
      }
      case IPFIX_SETID_TEMPLATE:
      case IPFIX_SETID_TEMPLATE_NF9:
          if ( len<4 )
              goto ioerr;
          templid   = READ16(buf);
          nfields   = READ16(buf+2);
          offset = 4;
          ndatafields  = nfields;
          nscopefields = 0;
          break;
      default:
          return -1;
    }

    if ( nfields == 0 ) {
        /** template withdrawal message
         */
        if ( templid == setid ) {
            while( ifh->templates )
                if ((ifh->templates)->type == setid)
					ipfix_delete_template( ifh, ifh->templates );
            mlogf( 3, "[%s] %u withdraw all templates\n",
                   func, setid ); 
        }
        else if ( (t=mnslp_get_template(ifh, templid )) == NULL) {
            mlogf( 3, "[%s] %u got withdraw for non-existant template %d\n", 
                   func, setid, templid ); 
        } else {
            ipfix_delete_template( ifh, t );
            mlogf( 3, "[%s] %u withdraw template %u\n", 
                   func, setid, templid ); 
        }

        *nread = offset;
        *templ = NULL;
        errno  = 0;
        return 0;
    }

#ifdef DEBUG
    mlogf( 3, "  tid=%d, nfields=%d(%ds/%d)\n",
           templid, nfields, nscopefields, ndatafields );
#endif

    /** get template node
     */
    t = mnslp_get_template(ifh, templid );
    if ( (t == NULL)
         || (nfields > t->nfields) ) {

        if ( t )
           // Replace the template.
           ipfix_delete_template( ifh, t );
		/** alloc mem
		*/
		if ( (t=calloc( 1, sizeof(ipfix_template_t) )) ==NULL )
			return -1;

		if ( (t->fields=calloc( nfields, sizeof(ipfix_template_field_t) )) ==NULL ) {
			free(t);
			return -1;
		}
		newnode =1;
    }
    else {
        newnode =0;
        /* todo: remove the code below */
        for( i=0;  i<nfields; i++ ) {
            if ( t->fields[i].unknown_f ) {
                ipfix_free_unknown_ftinfo( t->fields[i].elem );
            }
        }
    }

    t->tid     = templid;
    t->nfields = nfields;
    t->ndatafields  = ndatafields;
    t->nscopefields = nscopefields;

    /** read field definitions
     */
    for( i=0;  i<nfields; i++ ) {
        if ( (offset >= len)
             || (ipfix_read_templ_field( buf+offset, len-offset,
                                         &offset, &(t->fields[i]) ) <0 ) ) {
            goto errend;
        }
    }    

    /** add template to template list
     */
    t->next = ifh->templates;
    ifh->templates = t;

    *nread = offset;
    *templ = t;
    errno  = 0;
    return 0;

 errend:
    if ( newnode ) {
        free(t);
    }
    else {
        ipfix_delete_template( ifh, t );
    }
    return -1;

 ioerr:
    mlogf( 1, "[%s] invalid message lenght\n", func ); 
    return -1;
}
				  
/*
 * name:        mnslp_ipfix_decode_datarecord()
 * parameters:
 * desc:        this func parses and exports the ipfix data set
 * return:      0=ok, -1=error
 * todo:        parse message before calling this func
 */
int mnslp_ipfix_decode_datarecord( ipfix_template_t   *t,
								   char      		  *buf, 
								   int                buflen,
								   int                *nread,
								   ipfix_datarecord_t *data )
{
    uint8_t       *p;
    int           i, len, bytesleft;
    char          *func = "mnslp_ipfix_decode_datarecord";

    /** check data size
     */
    if ( t->nfields > data->maxfields ) {
        if ( data->addrs ) free( data->addrs );
        if ( data->lens ) free( data->lens );
        if ( (data->lens=calloc( t->nfields, sizeof(uint16_t)))==NULL) {
            data->maxfields = 0;
            return -1;
        }
        if ( (data->addrs=calloc( t->nfields, sizeof(void*)))==NULL) {
            free( data->lens );
            data->lens = NULL;
            data->maxfields = 0;
            return -1;
        }           
        data->maxfields = t->nfields;
    }

    /** parse message
     */
    bytesleft = buflen;
    *nread    = 0;
    p         = buf;
    for ( i=0; i< t->nfields; i++ ) {

        len = t->fields[i].flength;
        if ( len == IPFIX_FT_VARLEN ) {
            len =*p;
            p++;
            (*nread) ++;
            if ( len == 255 ) {
                len = READ16(p);
                p += 2;
                (*nread) +=2;
            }
        }

        bytesleft -= len;
        if ( bytesleft < 0 ) {
            mlogf( 0, "[%s] record%d: msg too short\n", func, i+1 );
            errno = EIO;
            return -1;
        }

        t->fields[i].elem->decode(p,p,len);

        data->lens[i]  = len;
        data->addrs[i] = p;

        p        += len;
        (*nread) += len;
    }

    return 0;
}



int _mnslp_ipfix_send_msg( ipfix_t *ifh, 
						   mnslp_ipfix_message_t *mes, 
						   mnslp_iobuf_t *buf )
{
    int i, retval =-1;

    /* send ipfix message */
    if ( _mnslp_do_writen( mes, buf->buffer, buf->buflen )
         != (int)buf->buflen ) {
         return -1;
    }
    
    retval =0;
    return retval;
}


static void _finish_cs( ipfix_t *ifh )
{
    size_t   buflen;
    uint8_t  *buf;

    /* finish current dataset */
    if ( (buf=ifh->cs_header) ==NULL )
        return;
    buflen = 0;
    INSERTU16( buf+buflen, buflen, ifh->cs_tid );
    INSERTU16( buf+buflen, buflen, ifh->cs_bytes );
    ifh->cs_bytes = 0;
    ifh->cs_header = NULL;
    ifh->cs_tid = 0;
}


/*
 * name:        mnslp_message_create()
 * parameters:
 * return:      0 = ok, -1 = error
 */
int mnslp_message_create( mnslp_ipfix_message_t ** ipfix_message  )
{
    mnslp_ipfix_message_t       *i;

	if ( (i=calloc( 1, sizeof(mnslp_ipfix_message_t) )) ==NULL )
        return -1;
	
	if ( (i->buffer=calloc( 1, IPFIX_DEFAULT_BUFLEN )) ==NULL ) {
        free( i );
        return -1;
    } 
	i->nrecords = 0;
	i->offset = 0;
	
    *ipfix_message = i;
    return 0;
}

void ipfix_message_release( mnslp_ipfix_message_t * ipfix_message )
{
	if (ipfix_message)
	{
		free(ipfix_message->buffer);
		free(ipfix_message);
	}
}


int mnslp_ipfix_export( ipfix_t *ifh, 
					    mnslp_ipfix_message_t *mes, 
					    ipfix_template_t *templ, char *fmt, ... )
{
    int       i;
    va_list   args;
    int d;
    char c, *s;

    if ( !templ ) {
        errno = EINVAL;
        return -1;
    }

    if ( templ->nfields > g_mnslp_data.maxfields ) {
        if ( g_mnslp_data.addrs ) free( g_mnslp_data.addrs );
        if ( g_mnslp_data.lens ) free( g_mnslp_data.lens );
        if ( (g_mnslp_data.lens=calloc( templ->nfields, sizeof(uint16_t))) ==NULL) {
            g_mnslp_data.maxfields = 0;
            return -1;
        }
        if ( (g_mnslp_data.addrs=calloc( templ->nfields, sizeof(void*))) ==NULL) {
            free( g_mnslp_data.lens );
            g_mnslp_data.lens = NULL;
            g_mnslp_data.maxfields = 0;
            return -1;
        }
        g_mnslp_data.maxfields = templ->nfields;
    }

    printf( "export some data c \n" );
    fflush( stdout) ;
    
    va_start(args, fmt);
    while (*fmt)
        switch (*fmt++) {
        case 's':              /* string */
            s = va_arg(args, char *);
            printf("string %s\n", s);
            break;
        case 'd':              /* int */
            d = va_arg(args, int);
            printf("int %d\n", d);
            break;
        case 'c':              /* char */
            /* need a cast here since va_arg only
               takes fully promoted types */
            c = (char) va_arg(args, int);
            printf("char %c\n", c);
            break;
        }
    va_end(args);
    
    
    /** collect pointers
     */
    va_start(args, fmt);
    for ( i=0; i<templ->nfields; i++ )
    {
        printf ("nfield: %d", i);
        char *temp = va_arg(args, char*);
        
        g_mnslp_data.addrs[i] = temp;          /* todo: change this! */
        printf ("data: %c", *temp);
        printf ("size of data: %d", sizeof(temp) );
        if ( templ->fields[i].flength == IPFIX_FT_VARLEN )
            g_mnslp_data.lens[i] = va_arg(args, int);
        else
            g_mnslp_data.lens[i] = templ->fields[i].flength;
        printf ("length: %d \n", g_mnslp_data.lens[i]);
    }
    va_end(args);

    printf( "export some data d " );
    fflush( stdout) ;

    return mnslp_ipfix_export_array( ifh, templ, mes, templ->nfields,
									   g_mnslp_data.addrs, g_mnslp_data.lens );
}


int mnslp_ipfix_import( ipfix_t *ifh, 
						mnslp_ipfix_message_t *mes,
						ipfix_datarecord_t  data )
{
    ipfix_hdr_t          hdr;                  /* ipfix packet header */
    char                 *buf;                 /* ipfix payload */
    uint16_t             setid, setlen;        /* set id, set lenght */
    int                  i, nread, offset;     /* counter */
    int                  bytes, bytesleft;
    int                  err_flag = 0;
    char                 *func = "mnslp_ipfix_import";

    if ( mnslp_ipfix_parse_hdr( mes, &hdr ) <0 ) {
        mlogf( 1, "[%s] read invalid msg header!\n", func );
        return -1;
    }
	
	printf (" Arrive 1");
	fflush( stdout) ;
	
    switch( hdr.version ) {
      case IPFIX_VERSION_NF9:
          buf   = mes->buffer;
          nread = IPFIX_HDR_BYTES_NF9;
          break;
      case IPFIX_VERSION:
          buf   = mes->buffer;
          nread = IPFIX_HDR_BYTES;
          break;
      default:
          return -1;
    }

	printf (" Arrive 2");
	fflush( stdout) ;

    /* 
     *  TODO AM: Remove because it seems that it not what we need 
    if ( ipfix_export_hdr( src, &hdr ) <0 )
        return -1;
     */

    /** read ipfix sets
     */
    for ( i=0; (nread+4) < (mes->offset); i++ ) {

        if ( (hdr.version == IPFIX_VERSION_NF9)
             && (i>=hdr.u.nf9.count) ) {
            break;
        }

        /** read ipfix record header (set id, lenght). Verifies that the lenght 
         *  given is valid.
         */
        setid   = READ16(buf+nread);
        setlen  = READ16(buf+nread+2);
        nread  += 4;
        if ( setlen <4 ) {
            mlogf( 0, "[%s] set%d: invalid set length %d\n",
                   func, i+1, setlen );
            continue;
        }
        setlen -= 4;
        if (setlen > (mes->offset - nread)) {
			int ii;

			for (ii = 0; ii < mes->offset; ii++)
				fprintf(stderr, "[%02x]", ((mes->buffer)[ii] & 0xFF));
			fprintf(stderr, "\n");

			mlogf(0, "[%s] set%d: message too short (%d>%d)!\n", func, i + 1,
					setlen + nread, (int) mes->offset);
			goto end;
		}

		mlogf(4, "[%s] set%d: sid=%u, setid=%d, setlen=%d\n", func, i + 1,
				(u_int) hdr.sourceid, setid, setlen + 4);

		/** read rest of ipfix message
         */
        if ( (setid == IPFIX_SETID_TEMPLATE_NF9)
             || (setid == IPFIX_SETID_OPTTEMPLATE_NF9)
             || (setid == IPFIX_SETID_TEMPLATE)
             || (setid == IPFIX_SETID_OPTTEMPLATE) ) {
            /** parse a template set ( option or normal template ).
             */

            for (offset = nread, bytesleft = setlen; bytesleft > 4;) {

				mlogf(4, "[%s] set%d: decode template, setlen=%d, left=%d\n",
						func, i + 1, setlen, bytesleft);

				ipfix_template_t  		*ipfixt  = NULL;

				if (mnslp_ipfix_decode_trecord(ifh, setid, buf + offset, bytesleft,
						&bytes, &ipfixt) < 0) {
					mlogf(0, "[%s] record%d: decode template failed: %s\n",
							func, i + 1, strerror(errno));
					break;
				} else {

					bytesleft -= bytes;
					offset += bytes;
					mlogf(4, "[%s] set%d: %d bytes decoded\n", func, i + 1,
							bytes);
				}
			}
            nread += setlen;
        }
        else if ( setid >255 )
        {
            /** get template
             */
            ipfix_template_t  		*ipfixt = NULL;

            if ( (ipfixt = mnslp_get_template(ifh, setid)) == NULL ) {
                mlogf( 0, "[%s] no template for %d, skip data set\n",
                       func, setid );
                nread += setlen;
                err_flag = 1;
            }
            else {

                /** read data records
                 */
                for ( offset=nread, bytesleft=setlen; bytesleft>4; ) {
                    if ( mnslp_ipfix_decode_datarecord( ipfixt, buf+offset, bytesleft,
                                                  &bytes, &data ) <0 ) {
                        mlogf( 0, "[%s] set%d: decode record failed: %s\n",
                               func, i+1, strerror(errno) );
                        goto errend;
                    }

                    //(void) ipfix_export_datarecord( src, t, &data );

                    bytesleft -= bytes;
                    offset    += bytes;
                }

                if ( bytesleft ) {
                    mlogf( 3, "[%s] set%d: skip %d bytes padding\n",
                           func, i+1, bytesleft );
                }
                nread += setlen;
            }
        }
        else {
            mlogf( 0, "[%s] set%d: invalid set id %d, set skipped!\n",
                   func, i+1, setid );
            nread += setlen;
        }
    } /* for (read sets */

    if ( err_flag )
        goto errend;

 end:
    ipfix_free_datarecord( &data );
    return nread;

 errend:
    ipfix_free_datarecord( &data );
    return -1;
	
}

int mnslp_ipfix_export_array( ipfix_t          *ifh,
							  ipfix_template_t *templ,
							  mnslp_ipfix_message_t *mes, 
							  int              nfields,
							  void             **fields,
							  uint16_t         *lengths )
{
    int ret;

	printf("mnslp_ipfix_export_array:\n");
    if (_mnslp_ipfix_export_array( ifh, templ, mes, nfields, fields, lengths ) < 0 )
    {
		fprintf(stderr, "_mnslp_ipfix_export_array error:\n");
		return -1;
	}
	printf("_mnslp_ipfix_export_flush: \n");
	fflush( stdout) ;
	ret = _mnslp_ipfix_export_flush(ifh,  mes);
    return ret;
}


/*
 * name:        _ipfix_write_template()
 * parameters:
 * return:      0/-1
 */
int _mnslp_ipfix_write_template( ipfix_t           *ifh,
								 mnslp_ipfix_message_t *mes,
								 ipfix_template_t  *templ )
{
    size_t            buflen, tsize=0, ssize=0, osize=0;
    char              *buf;
    uint16_t          tmp16;
    int               i, n;

    /** calc template size
     */
    if ( templ->type == OPTION_TEMPLATE ) {
        for ( i=0, ssize=0; i<templ->nscopefields; i++ ) {
            ssize += 4;
            if (templ->fields[i].elem->ft->eno != IPFIX_FT_NOENO)
                ssize += 4;
        }
        for ( osize=0; i<templ->nfields; i++ ) {
            osize += 4;
            if (templ->fields[i].elem->ft->eno != IPFIX_FT_NOENO)
                osize += 4;
        }
        tsize = 10 + osize + ssize;
    } else {
        for ( tsize=8,i=0; i<templ->nfields; i++ ) {
            tsize += 4;
            if (templ->fields[i].elem->ft->eno != IPFIX_FT_NOENO)
                tsize += 4;
        }
    }

    /** insert template set into buffer
     */
    if ( ifh->version == IPFIX_VERSION_NF9 ) {
        if ( templ->type == OPTION_TEMPLATE ) {
			size_t additional = 10;
			allocate_additional_memory(mes, additional);
            INSERTU16( mes->buffer+mes->offset, mes->offset, IPFIX_SETID_OPTTEMPLATE_NF9);
            INSERTU16( mes->buffer+mes->offset, mes->offset, tsize );
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->tid );
            INSERTU16( mes->buffer+mes->offset, mes->offset, ssize );
            INSERTU16( mes->buffer+mes->offset, mes->offset, osize );
        } else {
            size_t additional = 8;
            allocate_additional_memory(mes, additional);
            INSERTU16( mes->buffer+mes->offset, mes->offset, IPFIX_SETID_TEMPLATE_NF9);
            INSERTU16( mes->buffer+mes->offset, mes->offset, tsize );
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->tid );
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->nfields );
        }
    } else {
        if ( templ->type == OPTION_TEMPLATE ) {
            size_t additional = 10;
            allocate_additional_memory(mes, additional);
            INSERTU16( mes->buffer+mes->offset, mes->offset, IPFIX_SETID_OPTTEMPLATE );
            INSERTU16( mes->buffer+mes->offset, mes->offset, tsize );
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->tid );
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->nfields );
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->nscopefields );
        } else {
            size_t additional = 8;
            allocate_additional_memory(mes, additional);
            INSERTU16( mes->buffer+mes->offset, mes->offset, IPFIX_SETID_TEMPLATE);
            INSERTU16( mes->buffer+mes->offset, mes->offset, tsize );
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->tid );
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->nfields );
        }
    }

    if ( templ->type == OPTION_TEMPLATE ) {
        n = templ->nfields;
        for ( i=0; i<templ->nscopefields; i++ ) {
            if ( templ->fields[i].elem->ft->eno == IPFIX_FT_NOENO ) {
				size_t additional = 4;
				allocate_additional_memory(mes, additional);
                INSERTU16( mes->buffer+mes->offset, mes->offset, templ->fields[i].elem->ft->ftype );
                INSERTU16( mes->buffer+mes->offset, mes->offset, templ->fields[i].flength );
            } else {
				size_t additional = 6;
				allocate_additional_memory(mes, additional);
                tmp16 = templ->fields[i].elem->ft->ftype|IPFIX_EFT_VENDOR_BIT;
                INSERTU16( mes->buffer+mes->offset, mes->offset, tmp16 );
                INSERTU16( mes->buffer+mes->offset, mes->offset, templ->fields[i].flength );
                INSERTU32( mes->buffer+mes->offset, mes->offset, templ->fields[i].elem->ft->eno );
            }
        }
    } else {
        i = 0;
        n = templ->nfields;
    }

    for ( ; i<templ->nfields; i++ )
    {
        if ( templ->fields[i].elem->ft->eno == IPFIX_FT_NOENO ) {
			size_t additional = 4;
			allocate_additional_memory(mes, additional);
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->fields[i].elem->ft->ftype );
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->fields[i].flength );
        } else {
			size_t additional = 8;
			allocate_additional_memory(mes, additional);
            tmp16 = templ->fields[i].elem->ft->ftype|IPFIX_EFT_VENDOR_BIT;
            INSERTU16( mes->buffer+mes->offset, mes->offset, tmp16 );
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->fields[i].flength );
            INSERTU32( mes->buffer+mes->offset, mes->offset, templ->fields[i].elem->ft->eno );
        }
    }
    templ->tsend = time(NULL);

    return 0;
}



mnslp_iobuf_t *_mnslp_ipfix_getbuf ( void )
{
    mnslp_iobuf_t *b = g_buflist;

    if ( b ) {
        g_buflist = b->next;
        b->next = NULL;
    }

    return b;
}

void _mnslp_ipfix_freebuf( mnslp_iobuf_t *b )
{
    if ( b ) {
        b->next = g_buflist;
        g_buflist = b;
    }
}


/*
 * name:        _msnlp_ipfix_write_hdr()
 * parameters:
 * return:      0/-1
 */
int _mnslp_ipfix_write_hdr( ipfix_t *ifh, mnslp_iobuf_t *buf )
{
    time_t      now = time(NULL);

    printf("version export : %d \n", ifh->version);
    fflush( stdout) ;

    /** fill ipfix header
     */
    if ( ifh->version == IPFIX_VERSION_NF9 ) {
        buf->buflen = 0;
        ifh->seqno++;
        INSERTU16( buf->buffer+buf->buflen, buf->buflen, ifh->version );
        INSERTU16( buf->buffer+buf->buflen, buf->buflen, ifh->nrecords );
        INSERTU32( buf->buffer+buf->buflen, buf->buflen, ((now)*1000));
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

/* name:        _mnslp_ipfix_export_flush()
 * parameters:
 */
int _mnslp_ipfix_export_flush( ipfix_t *ifh, 
							   mnslp_ipfix_message_t *mes )
{
    mnslp_iobuf_t     *buf;
    int        		 ret;

    printf("in function  _mnslp_ipfix_export_flush: \n");
    fflush( stdout) ;

    if ( (ifh==NULL) || (ifh->offset==0) )
        return 0;

    if ( ifh->cs_tid > 0 ) {
        /* finish current dataset */
        _finish_cs( ifh );
    }
    
    if ( (buf= _mnslp_ipfix_getbuf()) ==NULL )
        return -1;

#ifdef DEBUG
    mlogf( 0, "[ipfix_export_flush] msg has %d records, %d bytes\n",
           ifh->nrecords, ifh->offset );
#endif
    _mnslp_ipfix_write_hdr( ifh, buf );
    memcpy( buf->buffer+buf->buflen, ifh->buffer, ifh->offset );
    buf->buflen += ifh->offset;

    ret = 0;
    if ( _mnslp_ipfix_send_msg( ifh, mes, buf ) <0 ) {
        ret = -1;
    }
    else {
        ifh->offset = 0;    /* ugh! */
        ifh->nrecords = 0;
    }

    _mnslp_ipfix_freebuf( buf );
    return ret;
}

int _mnslp_ipfix_export_array( ipfix_t          *ifh,
							   ipfix_template_t *templ,
							   mnslp_ipfix_message_t *mes,
							   int              nfields,
							   void             **fields,
							   uint16_t         *lengths )
{
    int               i, newset_f=0;
    size_t            buflen, datasetlen;
    uint8_t           *p, *buf;

    

    /** parameter check
     */
    if ( (templ==NULL) || (nfields!=templ->nfields) ) {
        errno = EINVAL;
        return -1;
    }
	
	printf("in function  _mnslp_ipfix_export_array: \n");
	printf( "Time send: %s", ctime (templ->tsend) );
	
	printf("message characters: %d \n", mes->offset);
    
    if ( templ->tsend == 0 ) {
		if ( _mnslp_ipfix_write_template( ifh, mes, templ ) <0 )
			return -1;		
    }
    
    printf("message header written: %d \n", mes->offset);
  
    /** get size of data set, check space
     */
    if ( templ->tid == ifh->cs_tid ) {
        newset_f = 0;
        datasetlen = 0;
    }
    else {
        if ( ifh->cs_tid > 0 ) {
            _finish_cs( ifh );
        }
        newset_f = 1;
        datasetlen = 4;
    }
        
    for ( i=0; i<nfields; i++ ) {
        if ( templ->fields[i].flength == IPFIX_FT_VARLEN ) {
            if ( lengths[i]>254 )
                datasetlen += 3;
            else
                datasetlen += 1;
        } else {
            if ( lengths[i] > templ->fields[i].flength ) {
                errno = EINVAL;
                return -1;
            }
        }
        datasetlen += lengths[i];
    }

    printf("datasetlen: %d", datasetlen);
    fflush( stdout) ;

    if ( (ifh->offset + datasetlen) > IPFIX_DEFAULT_BUFLEN ) {
        if ( ifh->cs_tid )
            _finish_cs( ifh );
        newset_f = 1;

        if ( _mnslp_ipfix_export_flush( ifh, mes ) <0 )
            return -1;
    }   

    /* fill buffer */
    buf    = (uint8_t*)(ifh->buffer) + ifh->offset;
    buflen = 0;

    if ( newset_f ) {
        /* insert data set
         */
        ifh->cs_bytes = 0;
        ifh->cs_header = buf;
        ifh->cs_tid = templ->tid;
        INSERTU16( buf+buflen, buflen, templ->tid );
        INSERTU16( buf+buflen, buflen, datasetlen );
    }
    /* csc: to be checked with Lutz whether the usage of "datasetlen" 
     * in the last 30 lines of code is correct */


    printf("offf set ifh: %d", ifh->offset);
    fflush( stdout) ;

    /* insert data record
     */
    for ( i=0; i<nfields; i++ ) {
        if ( templ->fields[i].flength == IPFIX_FT_VARLEN ) {
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
        if ( templ->fields[i].relay_f ) {
            ipfix_encode_bytes( p, buf+buflen, lengths[i] ); /* no encoding */
        }
        else {
            templ->fields[i].elem->encode( p, buf+buflen, lengths[i] );
        }
        buflen += lengths[i];
    }

    printf("dddd _mnslp_ipfix_export_array: %d \n", mes->offset);

    ifh->nrecords ++;
    ifh->offset += buflen;
    ifh->cs_bytes += buflen;
    if ( ifh->version == IPFIX_VERSION ) {
        ifh->seqno ++;
    }

    printf("end _mnslp_ipfix_export_array: %d \n", mes->offset);

    return 0;
}

