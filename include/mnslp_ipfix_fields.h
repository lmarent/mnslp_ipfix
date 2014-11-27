/// ----------------------------------------*- mode: C; -*--
/// @file mnslp_ipfix_fields.h
/// Tools for processing IPFIX messages in NSIS metering.
/// ----------------------------------------------------------
/// $Id: mnslp_ipfix_fields.h 2558 2014-11-26 14:11:00 amarentes $
/// $HeadURL: https://./include/mnslp_ipfix_fields.h $
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

/* ipfix information element list
 */

#ifndef MNSLP_IPFIX_FIELDS_H
#define MNSLP_IPFIX_FIELDS_H

#include <vector>
#include <stdlib.h>
#include <string>
#include "ipfix_def.h"


namespace mnslp_ipfix
{

	int ipfix_encode_int( void *in, void *out, size_t len );

	int ipfix_decode_int( void *in, void *out, size_t len );

	int ipfix_snprint_int( char *str, size_t size, void *data, size_t len );

	int ipfix_snprint_uint( char *str, size_t size, void *data, size_t len );

	int ipfix_encode_bytes( void *in, void *out, size_t len );

	int ipfix_decode_bytes( void *in, void *out, size_t len );

	int ipfix_snprint_bytes( char *str, size_t size, void *data, size_t len );

	int ipfix_snprint_string( char *str, size_t size, void *data, size_t len );

	int ipfix_snprint_ipaddr( char *str, size_t size, void *data, size_t len );

	int ipfix_encode_float( void *in, void *out, size_t len );
	
	int ipfix_decode_float( void *in, void *out, size_t len );

	int ipfix_snprint_float( char *str, size_t size, void *data, size_t len );


class mnslp_ipfix_field
{
	private:
	
		ipfix_field_type_t field_type;
		ipfix_encode_func    encode;
		ipfix_decode_func    decode;
		ipfix_snprint_func   snprint;
	
	public:
	
		mnslp_ipfix_field(ipfix_field_type_t param);
		~mnslp_ipfix_field();
		mnslp_ipfix_field& operator= (const mnslp_ipfix_field&);
		
		inline ipfix_field_type_t get_field_type(){ return field_type; }
};


class msnlp_ipfix_field_container
{
private:

	std::vector<mnslp_ipfix_field> fieldTypeList;
    
    void AddFieldType(int _eno, int _ftype, ssize_t _length, 
					  int _coding, const std::string _name, const std::string _documentation );
		
	void AddFieldType(ipfix_field_type_t &param);


public:
    msnlp_ipfix_field_container();
    
    ~msnlp_ipfix_field_container();

    void initialize_forward(void);
    
    void initialize_reverse(void);
    
    mnslp_ipfix_field * get_field( int eno, int type );

    
};


}

#endif // MNSLP_IPFIX_FIELDS_H
