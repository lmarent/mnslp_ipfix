/// ----------------------------------------*- mode: C; -*--
/// @file mnslp_ipfix_message.h
/// Tools for processing IPFIX messages in NSIS metering.
/// Export declarations of mnslp ipfix messages.
/// ----------------------------------------------------------
/// $Id: mnslp_ipfix_message.h 2558 2014-11-26 14:11:00 amarentes $
/// $HeadURL: https://./include/mnslp_ipfix_message.h $
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


/*
** ipfix.h - export declarations of mnslp_ipfix
**
** Copyright System and Computing Engineering, Universidad de los Andes
**
** $Id: mnslp_ipfix_message.h 152 2014-11-26 14:52:00 $
**
*/
#ifndef MNSLP_IPFIX_MESSAGE_H
#define MNSLP_IPFIX_MESSAGE_H

#include <inttypes.h>
#include "ipfix_def.h"
#include "mnslp_ipfix_fields.h"
#include "mnslp_ipfix_templates.h"

#ifndef ENOTSUP
#define ENOTSUP EOPNOTSUPP
#endif


namespace mnslp_ipfix
{
	
/*------ structs ---------------------------------------------------------*/

/** netflow9 header format
 **  0                   1                   2                   3
 **    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 **   |       Version Number          |            Count              |
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 **   |                           sysUpTime                           |
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 **   |                           UNIX Secs                           |
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 **   |                       Sequence Number                         |
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 **   |                        Source ID                              |
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */   
/** ipfix header format
 **  0                   1                   2                   3
 **    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 **   |       Version Number          |            Length             |
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 **   |                         Export Time                           |
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 **   |                       Sequence Number                         |
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 **   |                     Observation Domain ID                     |
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */   
typedef struct {
    uint16_t   version;     /* version of Flow Record format of this packet */
    union {
        struct {
            uint16_t   count;       /* total number of record in this packet */
            uint32_t   sysuptime;   /* sysuptime in milliseconds */
            uint32_t   unixtime;    /* seconds since 1970 */
        } nf9;
        struct {
            uint16_t   length;      /* total number of record in this packet */
            uint32_t   exporttime;  /* seconds since 1970 */
        } ipfix;
    } u;
    uint32_t   seqno;       /* incremental sequence counter */
    uint32_t   sourceid;    /* sourceid / observation domain id */

} ipfix_hdr_t;

#define IPFIX_VERSION_NF9           0x09
#define IPFIX_HDR_BYTES_NF9         20
#define IPFIX_SETID_TEMPLATE_NF9    0
#define IPFIX_SETID_OPTTEMPLATE_NF9 1

#define IPFIX_VERSION               0x0A
#define IPFIX_HDR_BYTES             16
#define IPFIX_SETID_TEMPLATE        2
#define IPFIX_SETID_OPTTEMPLATE     3
#define IPFIX_FT_VARLEN             65535
#define IPFIX_FT_NOENO              0
#define IPFIX_EFT_VENDOR_BIT        0x8000

#define IPFIX_PORTNO                4739
#define IPFIX_TLS_PORTNO            4740

#define IPFIX_DFLT_TEMPLRESENDINT   30
#define IPFIX_DFLT_TEMPLLIFETIME    300

#define IPFIX_DEFAULT_BUFLEN 		1400

/** bearer protocol
 */
typedef enum {
    IPFIX_PROTO_SCTP = 132,    /* IPPROTO_SCTP */    
    IPFIX_PROTO_TCP  = 6,      /* IPPROTO_TCP  */    
    IPFIX_PROTO_UDP  = 17      /* IPPROTO_UDP  */    
} ipfix_proto_t;


typedef struct ipfix_datarecord
{
    void              **addrs;
    uint16_t          *lens;
    uint16_t          maxfields;         /* sizeof arrays */
} ipfix_datarecord_t;


typedef struct
{
    int              		 sourceid;    /* domain id of the exporting process */
	int              		 version;     /* ipfix version to export */
    mnslp_template_container templates;  /* list of templates  */

    char        *buffer;          /* output buffer */
    int         nrecords;         /* no. of records in buffer */
    size_t      offset;           /* output buffer fill level */
    uint32_t    seqno;            /* sequence no. of next message */

    /* experimental */
    int        cs_tid;            /* template id of current dataset */
    int        cs_bytes;          /* size of current set */
    uint8_t    *cs_header;        /* start of current set */

} ipfix_t;

typedef struct {
    int       eno;		/* IPFIX enterprize number, 0 for standard element */
    uint16_t  ienum;		/* IPFIX information element number */
    uint16_t  length;		/* length of this element in bytes - use 65535 for varlen elements */
} export_fields_t;

typedef struct ipfixiobuf
{
    struct ipfixiobuf  *next;
    size_t             buflen;
    char               buffer[IPFIX_DEFAULT_BUFLEN+IPFIX_HDR_BYTES_NF9]; /*!!*/
} iobuf_t;

typedef struct ipfix_message
{
    char        buffer[IPFIX_DEFAULT_BUFLEN];   /* message buffer */
    int         nrecords;                       /* no. of records in buffer */
    size_t      offset;                         /* output buffer fill level */
} ipfix_message_t;


class mnslp_ipfix_message
{
	
   private:
	   ipfix_t * 						message;
	   msnlp_ipfix_field_container 		g_ipfix_fields;
	   time_t             				g_tstart;
	   iobuf_t            				g_iobuf[2], *g_buflist;
	   uint16_t           				g_lasttid;                  /* change this! */
	   ipfix_datarecord_t 				g_data; 					  /* ipfix_export */

   protected:
	
	   void init( void );
	   
	   void close( void );
	   
	   int add_vendor_information_elements( ipfix_field_type_t *fields );
	   
	   void new_template( mnslp_ipfix_template **templ, int nfields );

   public:	
   
	   void new_data_template( mnslp_ipfix_template **templ, int nfields );
                              
	   int  new_option_template( mnslp_ipfix_template **templ, int nfields );
                                
	   int  add_field( mnslp_ipfix_template *templ,
                       uint32_t enterprise_number,
                       uint16_t type, uint16_t length );
                      
	   int  add_scope_field( mnslp_ipfix_template *templ,
                             uint32_t enterprise_number,
                             uint16_t type, uint16_t length );
                            
	   void delete_template( mnslp_ipfix_template *templ );
	   
	   int  make_template( mnslp_ipfix_template **templ,
						   export_fields_t *fields, 
						   int nfields );
	   int  output( mnslp_ipfix_template *templ, ... );
	   
	   
	   int  output_array( mnslp_ipfix_template *templ, 
			   			  int nfields, 
					   	  void **fields, 
					      uint16_t *lengths );
                         
	   int  output_flush( void );
	   
	   mnslp_ipfix_message( int sourceid, int ipfix_version);
	   
	   ~mnslp_ipfix_message(void);

};


}

#endif // MNSLP_IPFIX_MESSAGE_H

