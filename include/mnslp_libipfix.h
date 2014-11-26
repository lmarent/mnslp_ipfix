/// ----------------------------------------*- mode: C; -*--
/// @file mnslp_libipfix.h
/// Tools for processing IPFIX messages in NSIS metering.
/// ----------------------------------------------------------
/// $Id: mnslp_libipfix.h 2558 2014-11-14 14:11:00 amarentes $
/// $HeadURL: https://./include/mnslp_libipfix.h $
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

#ifndef MNSLP__LIBIPFIX_H
#define MNSLP__LIBIPFIX_H

#ifdef __cplusplus 
extern "C" {
#endif

#include <ipfix.h>
#define MNSLP_IPFIX_DEFAULT_BUFLEN  1400


#ifndef NTOHLL
uint8_t g_isLittleEndian = 0;
void testEndianness() {
	uint32_t tmp = 0x0a0b0c0d;
	g_isLittleEndian = (tmp != ntohl(tmp));
}
#define HTONLL(val)  (g_isLittleEndian ? ((uint64_t)(htonl((uint32_t)((val)>>32))) | \
                          (((uint64_t)htonl((uint32_t)((val)&0xFFFFFFFF)))<<32)) : (val))
#define NTOHLL(val)  (g_isLittleEndian ? ((uint64_t)(ntohl((uint32_t)((val)>>32))) | \
                          (((uint64_t)ntohl((uint32_t)((val)&0xFFFFFFFF)))<<32)) : (val))
#endif


#define READ16(b) ((*(b)<<8)|*((b)+1))
#define READ32(b) ((((((*(b)<<8)|*(b+1))<<8)|(*(b+2)))<<8)|*(b+3))

#define INSERTU16(b,l,val) \
        { uint16_t _t=(val); memcpy((b),&_t,2); (l)+=2; }
#define INSERTU32(b,l,val) \
        { uint32_t _t=(val); memcpy((b),&_t,4); (l)+=4; }

#ifdef __cplusplus 
}
#endif

namespace mnslp_ipfix
{

    typedef struct mnslp_ipfix_message
    {
    	char        *buffer;   						/* message buffer */
    	int         nrecords;                       /* no. of records in buffer */
    	size_t      offset;                         /* output buffer fill level */
    } mnslp_ipfix_message_t;


    typedef struct mnslp_ipfixiobuf
    {
    	struct mnslp_ipfixiobuf  *next;
    	size_t       	      buflen;
    	char         	      buffer[IPFIX_DEFAULT_BUFLEN+IPFIX_HDR_BYTES_NF9]; /*!!*/
    } mnslp_iobuf_t;

    typedef struct mnslp_ipfix_message
    {
    	char        buffer[IPFIX_DEFAULT_BUFLEN];   /* message buffer */
    	int         nrecords;                       /* no. of records in buffer */
    	size_t      offset;                         /* output buffer fill level */
    } mnslp_ipfix_message_t;

    typedef struct mnslp_ipfix_node
    {
    	struct mnslp_ipfix_node   *next;
    	ipfix_t             *ifh;
    } mnslp_ipfix_node_t;


    class mnslp_ipfix_mes
    {

	private:
	   
      // attributes
       time_t             	g_tstart = 0;
	   mnslp_iobuf_t      	g_iobuf[2], *g_buflist=NULL;
	   ipfix_collector_t 	*g_collectors =NULL;
	   mnslp_ipfix_node_t 	*g_ipfixlist =NULL;
	   uint16_t           	g_lasttid;                 
	   ipfix_datarecord_t 	g_data = { NULL, NULL, 0 }; 
	   ipfix_field_t      	*g_ipfix_fields;
	  
	  // private methods
	   
	  void allocate_additional_memory(mnslp_ipfix_message_t *mes, size_t additional);
	  
	  int _mnslp_do_writen( mnslp_ipfix_message_t *mes, char *ptr, int nbytes );
	  
	  int mnslp_ipfix_parse_hdr( mnslp_ipfix_message_t *mes, ipfix_hdr_t *hdr );
	  
	  ipfix_template_t * mnslp_get_template(ipfix_t *ifh, uint16_t tid_param);
	  
	  int mnslp_ipfix_decode_trecord( ipfix_t *ifh,
	  								int            setid,
	  								const char     *buf,
	  								size_t         len,
	  								int            *nread,
	  								ipfix_template_t **templ );
	  
	  int mnslp_ipfix_decode_datarecord( ipfix_template_t   *t,
	  								   char      		  *buf, 
	  								   int                buflen,
	  								   int                *nread,
	  								   ipfix_datarecord_t *data );
	  
	  int _mnslp_ipfix_send_msg( ipfix_t *ifh, 
	  						   mnslp_ipfix_message_t *mes, 
	  						   mnslp_iobuf_t *buf );
	 
	  void _finish_cs( ipfix_t *ifh );
	  
	  int mnslp_ipfix_export_array( ipfix_t          *ifh,
	  							  ipfix_template_t *templ,
	  							  mnslp_ipfix_message_t *mes, 
	  							  int              nfields,
	  							  void             **fields,
	  							  uint16_t         *lengths );

	  int _mnslp_ipfix_write_template( ipfix_t           *ifh,
	  								 mnslp_ipfix_message_t *mes,
	  								 ipfix_template_t  *templ );
	  
	  mnslp_iobuf_t *_mnslp_ipfix_getbuf ( void );
	  
	  void _mnslp_ipfix_freebuf( mnslp_iobuf_t *b );
	  
	  int _mnslp_ipfix_write_hdr( ipfix_t *ifh, mnslp_iobuf_t *buf );
	  
	  int _mnslp_ipfix_export_flush( ipfix_t *ifh, 
	  							   mnslp_ipfix_message_t *mes );
	  
	  int _mnslp_ipfix_export_array( ipfix_t          *ifh,
	  							   ipfix_template_t *templ,
	  							   mnslp_ipfix_message_t *mes,
	  							   int              nfields,
	  							   void             **fields,
	  							   uint16_t         *lengths );

    public:

	   int mnslp_ipfix_export( ipfix_t *ifh, 
	   			   mnslp_ipfix_message_t *mes, 
				   ipfix_template_t *templ, ... );

	   int mnslp_ipfix_import( ipfix_t *ifh, 
	  			   mnslp_ipfix_message_t *mes,
				   ipfix_datarecord_t  data );
	   
	   int mnslp_message_create( mnslp_ipfix_message_t ** ipfix_message  );
						    
	   void ipfix_message_release( mnslp_ipfix_message_t * ipfix_message );



    };

}


#endif // MNSLP__LIBIPFIX_H
