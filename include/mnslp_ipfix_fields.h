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
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "ipfix_def.h"
#include "mnslp_ipfix_exception.h"


namespace mnslp_ipfix
{

#ifndef NTOHLL

#define HTONLL(val)  (((uint32_t) 0x0a0b0c0d != ntohl( (uint32_t) 0x0a0b0c0d )) ? ((uint64_t)(htonl((uint32_t)((val)>>32))) | \
                          (((uint64_t)htonl((uint32_t)((val)&0xFFFFFFFF)))<<32)) : (val))
#define NTOHLL(val)  (((uint32_t) 0x0a0b0c0d != ntohl( (uint32_t) 0x0a0b0c0d )) ? ((uint64_t)(ntohl((uint32_t)((val)>>32))) | \
                          (((uint64_t)ntohl((uint32_t)((val)&0xFFFFFFFF)))<<32)) : (val))
#endif


class mnslp_ipfix_value_field;

class mnslp_ipfix_field
{
	private:
	
		ipfix_field_type_t field_type;
	
	public:
	
		mnslp_ipfix_field(ipfix_field_type_t param);
		~mnslp_ipfix_field();
		mnslp_ipfix_field& operator= (const mnslp_ipfix_field&);
		
		inline bool operator== (const mnslp_ipfix_field& rhs)
		{ 
			return (field_type.eno == (rhs.field_type).eno ) && (field_type.ftype == (rhs.field_type).ftype); 
		}
				
		inline ipfix_field_type_t get_field_type(){ return field_type; }
		

		int ipfix_encode_int( mnslp_ipfix_value_field &in, uint8_t *out, size_t len, bool relay_f );

		int ipfix_decode_int( uint8_t *in, mnslp_ipfix_value_field *out, size_t len, bool relay_f );
	
		int ipfix_snprint_int( char * str, size_t size, mnslp_ipfix_value_field &in, size_t len );

		int ipfix_snprint_uint( char * str, size_t size, mnslp_ipfix_value_field &in, size_t len );

		int ipfix_encode_bytes( mnslp_ipfix_value_field &in, uint8_t *out, size_t len, bool relay_f );

		int ipfix_decode_bytes( uint8_t *in, mnslp_ipfix_value_field *out, size_t len, bool relay_f );

		int ipfix_snprint_bytes( char * str, size_t size, mnslp_ipfix_value_field &in, size_t len );

		int ipfix_snprint_string( char * str, size_t size, mnslp_ipfix_value_field &in, size_t len );

		int ipfix_snprint_ipaddr( char * str, size_t size, mnslp_ipfix_value_field &in, size_t len );

		int ipfix_encode_float( mnslp_ipfix_value_field &in, uint8_t *out, size_t len, bool relay_f );
	
		int ipfix_decode_float( mnslp_ipfix_value_field &in, uint8_t *out, size_t len, bool relay_f );

		int ipfix_snprint_float( char * str, size_t size, mnslp_ipfix_value_field &in, size_t len );
		
};


class mnslp_ipfix_field_key
{

private:

	int eno;  				/* enterprise number or 0 */
	int ftype;              /* field type */

public:
	inline mnslp_ipfix_field_key(int _eno, int _ftype): 
			eno(_eno), ftype(_ftype){}
	
	inline ~mnslp_ipfix_field_key(){}
		
	inline bool operator ==(const mnslp_ipfix_field_key &rhs) const
	{ 
		return ((eno == rhs.eno) && (ftype == rhs.ftype)); 
	}

	inline bool operator< (const mnslp_ipfix_field_key& rhs) const
	{
		return (eno + ftype) < (rhs.eno + rhs.ftype ); 
	}

	inline mnslp_ipfix_field_key& operator= (const mnslp_ipfix_field_key& param)
	{
		eno = param.eno;
		ftype = param.ftype;
		return *this;
	}
	
	inline bool operator != (const mnslp_ipfix_field_key &rhs) const
	{
		return ((eno != rhs.eno) || (ftype != rhs.ftype)); 
	}

};

class mnslp_ipfix_value_field
{

private:
	uint8_t value8;
	uint16_t value16;
	uint32_t value32;
	uint64_t value64;
	int length;
	char *valuechar;
	uint8_t *valuebyte;
	
public:
	mnslp_ipfix_value_field(mnslp_ipfix_field f_type, uint8_t _value8);
	mnslp_ipfix_value_field(mnslp_ipfix_field f_type, uint16_t _value16);
	mnslp_ipfix_value_field(mnslp_ipfix_field f_type, uint32_t _value32);
	mnslp_ipfix_value_field(mnslp_ipfix_field f_type, uint64_t _value64);
	mnslp_ipfix_value_field(mnslp_ipfix_field f_type, char * _valuechar, int _length);
	mnslp_ipfix_value_field(mnslp_ipfix_field f_type, uint8_t * _valuebyte, int _length);
	mnslp_ipfix_value_field(const mnslp_ipfix_value_field &param);
	
	~mnslp_ipfix_value_field();
	mnslp_ipfix_value_field& operator= (const mnslp_ipfix_value_field &param);
	
	
	inline uint8_t get_value_int8(){ return value8; }
	inline uint16_t get_value_int16(){ return value16; }
	inline uint32_t get_value_int32(){ return value32; }
	inline uint64_t get_value_int64(){ return value64; }
	inline uint32_t get_value_float32(){ return value32; }
	inline uint64_t get_value_float64(){ return value64; }
	inline char * get_value_string(){ return valuechar; }
	inline uint8_t * get_value_byte(){ return valuebyte; }
	inline uint8_t * get_value_address(){ return valuebyte; }
	
	void print(void);
	
};

class msnlp_ipfix_field_container
{
private:

	std::vector<mnslp_ipfix_field> fieldTypeList;					  
	
public:
    msnlp_ipfix_field_container();
    
    ~msnlp_ipfix_field_container();

    void initialize_forward(void);
    
    void initialize_reverse(void);
    
    mnslp_ipfix_field * get_field( int eno, int type );
        
    void AddFieldType(ipfix_field_type_t &param) throw(mnslp_ipfix_bad_argument);

    void AddFieldType(int _eno, int _ftype, ssize_t _length, 
					  int _coding, 
					  const std::string _name, 
					  const std::string _documentation ) throw(mnslp_ipfix_bad_argument);

    int get_num_fields(void);
    
    inline void clear(void){ fieldTypeList.clear(); }
    
};

inline int msnlp_ipfix_field_container::get_num_fields(void)
{
	return fieldTypeList.size();
}


}

#endif // MNSLP_IPFIX_FIELDS_H
