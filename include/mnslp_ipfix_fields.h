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
		

		int ipfix_encode_int( mnslp_ipfix_value_field &in, uint8_t *out, int relay_f );

		mnslp_ipfix_value_field ipfix_decode_int( uint8_t *in, size_t len, int relay_f );
	
		int ipfix_snprint_int( char * str, size_t size, mnslp_ipfix_value_field &in, size_t len );

		int ipfix_snprint_uint( char * str, size_t size, mnslp_ipfix_value_field &in, size_t len );

		int ipfix_encode_bytes(mnslp_ipfix_value_field in, uint8_t *out, int relay_f );

		mnslp_ipfix_value_field ipfix_decode_bytes( uint8_t *in, size_t len, int relay_f );

		int ipfix_snprint_bytes( char * str, size_t size, mnslp_ipfix_value_field &in_field, size_t len );

		int ipfix_snprint_string( char * str, size_t size, mnslp_ipfix_value_field &in_field, size_t len );

		int ipfix_snprint_ipaddr( char * str, size_t size, mnslp_ipfix_value_field in_field, size_t len );

		int ipfix_encode_float( mnslp_ipfix_value_field in, uint8_t *out, int relay_f );
	
		mnslp_ipfix_value_field ipfix_decode_float( uint8_t *in, size_t len, int relay_f );

		int ipfix_snprint_float( char * str, size_t size, mnslp_ipfix_value_field in, size_t len );

		mnslp_ipfix_value_field get_ipfix_value_field(uint8_t &_value8);
		mnslp_ipfix_value_field get_ipfix_value_field(uint16_t &_value16);
		mnslp_ipfix_value_field get_ipfix_value_field(uint32_t &_value32);
		mnslp_ipfix_value_field get_ipfix_value_field(uint64_t &_value64);
		mnslp_ipfix_value_field get_ipfix_value_field(char * _valuechar, int _length);
		mnslp_ipfix_value_field get_ipfix_value_field(uint8_t * _valuebyte, int _length);
		
		int encode( mnslp_ipfix_value_field in, 
						uint8_t *out, int relay_f);
				
		mnslp_ipfix_value_field decode( uint8_t *in, size_t len, int relay_f);
		
		int snprint( char * str, size_t size, mnslp_ipfix_value_field in, size_t len );

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
	
	mnslp_ipfix_value_field(const mnslp_ipfix_value_field &param);
	
	mnslp_ipfix_value_field():
		valuebyte(NULL), valuechar(NULL), length(0), 
		value16(0), value32(0), value64(0) {};
	~mnslp_ipfix_value_field();
	mnslp_ipfix_value_field& operator= (const mnslp_ipfix_value_field &param);
	
	inline void set_value_int8(uint8_t &_value){value8 =_value; length = 1;}
	inline void set_value_int16(uint16_t &_value){value16 =_value; length = 2;}
	inline void set_value_int32(uint32_t &_value){value32 =_value; length = 4;}
	inline void set_value_int64(uint64_t &_value){value64 =_value; length = 8;}
	inline void set_value_float32(uint32_t &_value){value32 =_value; length = 4;}
	inline void set_value_float64(uint64_t &_value){value64 =_value; length = 8;}
	void set_value_vunit8(uint8_t * _valuebyte, int _length);
	void set_value_vchar(char * _valuechar, int _length);
	
	
	inline uint8_t get_value_int8(){ return value8; }
	inline uint16_t get_value_int16(){ return value16; }
	inline uint32_t get_value_int32(){ return value32; }
	inline uint64_t get_value_int64(){ return value64; }
	inline uint32_t get_value_float32(){ return value32; }
	inline uint64_t get_value_float64(){ return value64; }
	inline char * get_value_string(){ return valuechar; }
	inline uint8_t * get_value_byte(){ return valuebyte; }
	inline uint8_t * get_value_address(){ return valuebyte; }
	inline int getLength(){ return length; }
	
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
