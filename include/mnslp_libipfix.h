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

#include <ipfix.h>



#define MNSLP_IPFIX_DEFAULT_BUFLEN  1400

typedef struct mnslp_ipfix_message
{
    char        *buffer;   						/* message buffer */
    int         nrecords;                       /* no. of records in buffer */
    size_t      offset;                         /* output buffer fill level */
} mnslp_ipfix_message_t;


int mnslp_ipfix_export( ipfix_t *ifh, 
					    mnslp_ipfix_message_t *mes, 
					    ipfix_template_t *templ, ... );

int mnslp_message_create( mnslp_ipfix_message_t ** ipfix_message  );
					    
void ipfix_message_release( mnslp_ipfix_message_t * ipfix_message );

int mnslp_ipfix_import( ipfix_t *ifh, 
						mnslp_ipfix_message_t *mes,
						ipfix_datarecord_t  data );


#endif // MNSLP__LIBIPFIX_H
