/// ----------------------------------------*- mode: C; -*--
/// @file ipfix.h
/// Tools for processing IPFIX messages in NSIS metering.
/// Export declarations of ipfix messages.
/// ----------------------------------------------------------
/// $Id: ipfix.h 2558 2014-11-26 14:11:00 amarentes $
/// $HeadURL: https://./include/ipfix.h $
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
** $Id: ipfix.h 152 2014-11-26 14:52:00 $
**
*/
#ifndef IPFIX_H
#define IPFIX_H

#include <inttypes.h>
#include "ipfix_def.h"

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

/** bearer protocol
 */
typedef enum {
    IPFIX_PROTO_SCTP = 132,    /* IPPROTO_SCTP */    
    IPFIX_PROTO_TCP  = 6,      /* IPPROTO_TCP  */    
    IPFIX_PROTO_UDP  = 17      /* IPPROTO_UDP  */    
} ipfix_proto_t;

typedef struct
{
    uint16_t            flength;           /* less or eq. elem->flength  */
    int                 unknown_f;         /* set if unknown elem */
    int                 relay_f;           /* just relay no, encoding (exp.) */
    ipfix_field_t       *elem;
} ipfix_template_field_t;

typedef struct ipfix_datarecord
{
    void              **addrs;
    uint16_t          *lens;
    uint16_t          maxfields;         /* sizeof arrays */
} ipfix_datarecord_t;

typedef enum {
    DATA_TEMPLATE, OPTION_TEMPLATE
} ipfix_templ_type_t;

typedef struct ipfix_template
{
    struct ipfix_template   *next; /* for internal use          */
    ipfix_templ_type_t      type;  /* data or option template   */
    time_t                  tsend; /* time of last transmission */

    uint16_t                tid;
    int                     ndatafields;
    int                     nscopefields;
    int                     nfields;        /* = ndata + nscope */
    ipfix_template_field_t  *fields;
    int                     maxfields;         /* sizeof fields */
} ipfix_template_t;

typedef struct
{
    int              sourceid;    /* domain id of the exporting process */
    int              version;     /* ipfix version to export */
    void             *collectors; /* list of collectors */
    ipfix_template_t *templates;  /* list of templates  */

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


class mnslp_ipfix_message
{
	
   private:
   
   
   public:	
   
	   int  ipfix_open( ipfix_t **ifh, int sourceid, int ipfix_version );

	   int  ipfix_new_data_template( ipfix_t *ifh,
                              ipfix_template_t **templ, int nfields );
                              
	   int  ipfix_new_option_template( ipfix_t *ifh,
                                ipfix_template_t **templ, int nfields );
                                
	   int  ipfix_add_field( ipfix_t *ifh, ipfix_template_t *templ,
                      uint32_t enterprise_number,
                      uint16_t type, uint16_t length );
                      
	   int  ipfix_add_scope_field( ipfix_t *ifh, ipfix_template_t *templ,
                            uint32_t enterprise_number,
                            uint16_t type, uint16_t length );
                            
	   void ipfix_delete_template( ipfix_t *ifh, ipfix_template_t *templ );
	   
	   int  ipfix_make_template( ipfix_t *handle, ipfix_template_t **templ,
                         export_fields_t *fields, int nfields );
                         
	   int  ipfix_export( ipfix_t *ifh, ipfix_template_t *templ, ... );
	   
	   
	   int  ipfix_export_array( ipfix_t *ifh, ipfix_template_t *templ,
                         int nfields, void **fields, uint16_t *lengths );
                         
	   int  ipfix_export_flush( ipfix_t *ifh );
	   
	   void ipfix_close( ipfix_t *ifh );

	   mnslp_ipfix_message();
	   
	   ~mnslp_ipfix_message();

}


}

#endif

