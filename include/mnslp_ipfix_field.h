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

/**
 * \class mnslp_ipfix_field
 *
 *
 * \brief This respresents a field to be exchanged
 *
 * This class is used to represent a field that is going to be exchange.
 * a field for this project knows how to create its corresponding field
 * value, it also knows how to encode, decode and convert to string a field value
 *
 * \author Andres Marentes
 *
 * \version 0.1 
 *
 * \date 2014/12/18 9:57:00
 *
 * Contact: la.marentes455@uniandes.edu.co
 *  
 */
class mnslp_ipfix_field
{
	private:
	
		ipfix_field_type_t field_type;  ///< Type of this field.
	
	public:
	
		
		/** Constructor without parameters, this is required for the template class 
		*/
		mnslp_ipfix_field();
		
		/** Constructor. Creates a field 
		*  @param Takes the field type and creates the field 
		*/
		mnslp_ipfix_field(ipfix_field_type_t &param);
		
		/** Constructor from the data of another instance
		 *
		 */
		mnslp_ipfix_field(const mnslp_ipfix_field &rhs); 
		
		/** Destructor.
		*/
		~mnslp_ipfix_field();
		
		/** Assignment operator. It equals a field from another field value.
		*  @param  the field to copy from.
		*/
		mnslp_ipfix_field& operator= (const mnslp_ipfix_field&);
		
		/** Equal operator. verifies is the field given as parameter is equal
		*  @param  the field to compare
		*/
		inline bool operator== (const mnslp_ipfix_field& rhs)
		{ 
			return (field_type.eno == (rhs.field_type).eno ) && (field_type.ftype == (rhs.field_type).ftype); 
		}

		/** Equal operator. verifies is the field given as parameter is not equal
		*  @param  the field to compare
		*/
		inline bool operator!= (const mnslp_ipfix_field& rhs)
		{ 
			return (field_type.eno != (rhs.field_type).eno ) || (field_type.ftype != (rhs.field_type).ftype); 
		}
				
		/** Get the field type
		*  @param  None
		*/
		inline ipfix_field_type_t get_field_type(){ return field_type; }
		
		/** Encode a field value of type int into a uint8_t array. 
		*   verifies that the value is of type int. 
		*  @param  in - value to enconce
		* 		   out - array where the method puts the information
		* 		   relay_f - if 0 it must do network encoding, 
		* 						1 just copy the information.
		*/
		int ipfix_encode_int( mnslp_ipfix_value_field &in, uint8_t *out, int relay_f ); /* Tested */

		/** Decode from a uint8_t array to a field value of type int.
		*   verifies that the value is of type int. 
		*  @param  in - we take the value from param
		* 		   len - how many bytes are going to be taken
		* 		   relay_f - if 0 it must do network encoding, 
		* 						1 just copy the information.
		*/
		mnslp_ipfix_value_field ipfix_decode_int( uint8_t *in, size_t len, int relay_f ); /* Tested */
	
		/** Put the field value int into a char string 
		*  @param  in - value to put
		* 		   str - string where the method puts the information
		* 		   size - number of characters allocated to str, so the method could no 
		* 				  copy more size characters.
		*/
		int ipfix_snprint_int( char * str, size_t size, mnslp_ipfix_value_field &in ); /* Tested */

		/** Put the field value unsigned int into a char string 
		*  @param  in - value to put
		* 		   str - string where the method puts the information
		* 		   size - number of characters allocated to str, so the method could no 
		* 				  copy more size characters.
		*/
		int ipfix_snprint_uint( char * str, size_t size, mnslp_ipfix_value_field &in );

		/** Encode a field value of type bytes into a uint8_t array. 
		*   verifies that the value is of type bytes. 
		*  @param  in - value to enconce
		* 		   out - array where the method put the information
		* 		   relay_f - if 0 it must do network encoding, 
		* 						1 just copy the information.
		*/
		int ipfix_encode_bytes(mnslp_ipfix_value_field in, uint8_t *out, int relay_f ); /* Tested */

		/** Decode from a uint8_t array to a field value of type bytes.
		*   verifies that the value is of type bytes. 
		*  @param  in - we take the value from param
		* 		   len - how many bytes are going to be taken
		* 		   relay_f - if 0 it must do network encoding, 
		* 						1 just copy the information.
		*/
		mnslp_ipfix_value_field ipfix_decode_bytes( uint8_t *in, size_t len, int relay_f ); /* Tested */

		/** Put a bytes field value into a char string 
		*  @param  in - value to put
		* 		   str - string where the method puts the information
		* 		   size - number of characters allocated to str, so the method could no 
		* 				  copy more size characters.
		*/
		int ipfix_snprint_bytes( char * str, size_t size, mnslp_ipfix_value_field &in_field );

		/** Put an string field value into a char string 
		*  @param  in - value to put
		* 		   str - string where the method puts the information
		* 		   size - number of characters allocated to str, so the method could no 
		* 				  copy more size characters.
		*/
		int ipfix_snprint_string( char * str, size_t size, mnslp_ipfix_value_field &in_field );

		/** Put an ip address field value into a char string 
		*  @param  in - value to put
		* 		   str - string where the method puts the information
		* 		   size - number of characters allocated to str, so the method could no 
		* 				  copy more size characters.
		*/
		int ipfix_snprint_ipaddr( char * str, size_t size, mnslp_ipfix_value_field &in_field ); /* Tested */

		/** Encode a field value of type float into a uint8_t array. 
		*   verifies that the value is of type float. 
		*  @param  in - value to enconce
		* 		   out - array where the method put the information
		* 		   relay_f - if 0 it must do network encoding, 
		* 						1 just copy the information.
		*/
		int ipfix_encode_float( mnslp_ipfix_value_field in, uint8_t *out, int relay_f ); /* Tested */
	
		/** Decode from a uint8_t array to a field value of type float.
		*   verifies that the value is a float type. 
		*  @param  in - we take the value from param
		* 		   len - how many bytes are going to be taken
		* 		   relay_f - if 0 it must do network encoding, 
		* 						1 just copy the information.
		*/
		mnslp_ipfix_value_field ipfix_decode_float( uint8_t *in, size_t len, int relay_f ); /* Tested */

		/** Put a float field value into a char string 
		*  @param  in - value to put
		* 		   str - string where the method puts the information
		* 		   size - number of characters allocated to str, so the method could no 
		* 				  copy more size characters.
		*/
		int ipfix_snprint_float( char * str, size_t size, mnslp_ipfix_value_field &in );

		/** Get the field value from a field of type int of one character
		*  verifies that the field is of type int and one character long.
		*  @param  _value8 - Value that is going to be assigned
		*/
		mnslp_ipfix_value_field get_ipfix_value_field(uint8_t &_value8); /* Tested */

		/** Get the field value from a field of type int of two characters
		*  verifies that the field is of type int and two characters long.
		*  @param  _value16 - Value that is going to be assigned
		*  @throw   mnslp_ipfix_exception The length of field is not one byte
		*/
		mnslp_ipfix_value_field get_ipfix_value_field(uint16_t &_value16); /* Tested */

		/** Get the field value from a field of type int of four characters
		*  verifies that the field is of type int and four characters long.
		*  @param  _value32 - Value that is going to be assigned
		*  @throw   mnslp_ipfix_exception The length of field is not two bytes
		*/
		mnslp_ipfix_value_field get_ipfix_value_field(uint32_t &_value32); /* Tested */

		/** Get the field value from a field of type int of eight characters
		*  verifies that the field is of type int and eight characters long.
		*  @param  _value64 - Value that is going to be assigned
		*  @throw   mnslp_ipfix_exception The length of field is not four bytes
		*/
		mnslp_ipfix_value_field get_ipfix_value_field(uint64_t &_value64); /* Tested */

		/** Get the field value from a field of type char
		*  verifies that the field is of type char
		*  @param  _valuechar - Value that is going to be assigned
		* 		   _length	  - number of characters to copy from the _valuechar pointer
		*  @throw   mnslp_ipfix_exception The length of field is not eight bytes
		*/
		mnslp_ipfix_value_field get_ipfix_value_field(char * _valuechar, int _length); /* Tested */

		/** Get the field value from a field of type unit8_t
		*  verifies that the field is of type char
		*  @param  _valuebyte - Value that is going to be assigned
		* 		   _length	  - number of characters to copy from the _valuebyte pointer
		*  @throw   mnslp_ipfix_exception The length requested is not as the field length
		*/
		mnslp_ipfix_value_field get_ipfix_value_field(uint8_t * _valuebyte, int _length); /* Tested */
		
		/** Encode a field value. It takes the field type from the field 
		*   and encode based on the coding for that field type 
		*  @param  in - value to enconce
		* 		   out - array where the method put the information
		* 		   relay_f - if 0 it must do network encoding, 
		* 						1 just copy the information.
		* @throw   mnslp_ipfix_exception The length requested is not as the field length
		*/
		int encode( mnslp_ipfix_value_field in, 
						uint8_t *out, int relay_f);
				
		/** Decode a field value. It takes the field type from the field 
		*   and decode based on the coding for that field type 
		*  @param  in - value to deconce
		* 		   len - how many bytes are going to be taken
		* 		   relay_f - if 0 it must do network encoding, 
		* 						1 just copy the information.
		*/
		mnslp_ipfix_value_field decode( uint8_t *in, size_t len, int relay_f);
		
		/** Put a field value into a char string. It takes the field type from
		 *  the field object for who is call is method.
		*  @param  in - value to put
		* 		   str - string where the method puts the information
		* 		   size - number of characters allocated to str, so the method could no 
		* 				  copy more size characters.
		*/
		int snprint( char * str, size_t size, mnslp_ipfix_value_field &in );

};

/**
 * \class mnslp_ipfix_field_key
 *
 *
 * \brief This respresents the key of a field to be exchanged.
 *
 * This class is used to represent the key of a field that is going to be exchange.
 * The class is the key used on the map of field values inside the record set class.
 *
 * \author Andres Marentes
 *
 * \version 0.1 
 *
 * \date 2014/12/18 9:57:00
 *
 * Contact: la.marentes455@uniandes.edu.co
 *  
 */
class mnslp_ipfix_field_key
{

private:

	int eno;  				///< enterprise number or 0 
	int ftype;              ///< field type 

public:
	
	/// Constructor of the field key
	inline mnslp_ipfix_field_key(int _eno, int _ftype): 
			eno(_eno), ftype(_ftype){}
	
	/// Destructor of the field key
	inline ~mnslp_ipfix_field_key(){}
		
	/**
	 *  Equals to operator. It is equal when it has the same enterprise number an type
	 */
	inline bool operator ==(const mnslp_ipfix_field_key &rhs) const
	{ 
		return ((eno == rhs.eno) && (ftype == rhs.ftype)); 
	}

	/** less operator. a key field is less than other when the sum of its 
	*    attributes is less that the same sum for the key field given as 
	*    parameter.
	*/ 
	inline bool operator< (const mnslp_ipfix_field_key& rhs) const
	{
		return (eno + ftype) < (rhs.eno + rhs.ftype ); 
	}

	/** 
	 * Assignment operator. 
	*/ 
	inline mnslp_ipfix_field_key& operator= (const mnslp_ipfix_field_key& param)
	{
		eno = param.eno;
		ftype = param.ftype;
		return *this;
	}
	
	/** 
	 * Not equal to operator. 
	*/ 
	inline bool operator != (const mnslp_ipfix_field_key &rhs) const
	{
		return ((eno != rhs.eno) || (ftype != rhs.ftype)); 
	}
	
	/** Convert the key field in a string.
	*/ 
	std::string to_string();

};

/**
 * \class mnslp_ipfix_value_field
 *
 *
 * \brief This class respresents the value a of field to be exchanged.
 * 
 * \author Andres Marentes
 *
 * \version 0.1 
 *
 * \date 2014/12/18 9:57:00
 *
 * Contact: la.marentes455@uniandes.edu.co
 *  
 */
class mnslp_ipfix_value_field
{

private:
	uint8_t value8; 		///< The field is int, and it has one character
	uint16_t value16;		///< The field is int, and it has two characters
	uint32_t value32;		///< The field is int or float, and it has four characters
	uint64_t value64;		///< The field is int or float, and it has eight characters
	int length;				///< The length of the field value in characters
	char *valuechar;		///< The value is a character string
	uint8_t *valuebyte;		///< The value is a byte string
	
public:
	
	/** 
	 * Contructor of the class, it copies another field value
	 */
	mnslp_ipfix_value_field(const mnslp_ipfix_value_field &param);
	
	/** 
	 * Contructor of the class, creates a field value empty
	 */
	mnslp_ipfix_value_field():
		value8(0), value16(0), value32(0), value64(0), 
		length(0), valuechar(NULL), valuebyte(NULL) 
		 {};
		
	/** 
	 * Destructor of the class
	 */
	~mnslp_ipfix_value_field();
	
	/** 
	 * Assigment operator. 
	 * @param param - value to copy.
	 */
	mnslp_ipfix_value_field& operator= (const mnslp_ipfix_value_field &param);
	
	/** 
	 * Set the value as one byte long  - Int
	 * @param _value - value to copy.
	 */
	inline void set_value_int8(uint8_t &_value){value8 =_value; length = 1;}

	/** 
	 * Set the value as two byte long - Int
	 * @param _value - value to copy.
	 */
	inline void set_value_int16(uint16_t &_value){value16 =_value; length = 2;}

	/** 
	 * Set the value as four byte long - Int
	 * @param _value - value to copy.
	 */
	inline void set_value_int32(uint32_t &_value){value32 =_value; length = 4;}

	/** 
	 * Set the value as eight byte long - Int
	 * @param _value - value to copy.
	 */
	inline void set_value_int64(uint64_t &_value){value64 =_value; length = 8;}

	/** 
	 * Set the value as four byte long - float
	 * @param _value - value to copy.
	 */
	inline void set_value_float32(uint32_t &_value){value32 =_value; length = 4;}

	/** 
	 * Set the value as eight byte long - float
	 * @param _value - value to copy.
	 */
	inline void set_value_float64(uint64_t &_value){value64 =_value; length = 8;}

	/** 
	 * Set the value as a variable string of type unit8. 
	 * @param _value - value to copy.
	 * 		  _lenght - number of bytes to copy
	 */
	void set_value_vunit8(uint8_t * _valuebyte, int _length);

	/** 
	 * Set the value as a variable string of type char. 
	 * @param _value - value to copy.
	 * 		  _lenght - number of characters to copy
	 */
	void set_value_vchar(char * _valuechar, int _length);
	
	
	/** 
	 * Obtain the value as one byte long  - Int
	 */
	inline uint8_t get_value_int8(){ return value8; }

	/** 
	 * Obtain the value as two byte long  - Int
	 */
	inline uint16_t get_value_int16(){ return value16; }

	/** 
	 * Obtain the value as four byte long  - Int
	 */
	inline uint32_t get_value_int32(){ return value32; }

	/** 
	 * Obtain the value as eight byte long  - Int
	 */
	inline uint64_t get_value_int64(){ return value64; }

	/** 
	 * Obtain the value as four byte long  - float
	 */
	inline uint32_t get_value_float32(){ return value32; }

	/** 
	 * Obtain the value as eight byte long  - float
	 */
	inline uint64_t get_value_float64(){ return value64; }

	/** 
	 * Obtain the value as string of characters
	 */
	inline char * get_value_string(){ return valuechar; }

	/** 
	 * Obtain the value as string of bytes
	 */
	inline uint8_t * get_value_byte(){ return valuebyte; }

	/** 
	 * Obtain the ip address represented as a byte string
	 */
	inline uint8_t * get_value_address(){ return valuebyte; }

	/** 
	 * Obtain the number of bytes required to represent the value
	 */
	inline int get_length(){ return length; }
	
	bool operator!= (const mnslp_ipfix_value_field &rhs) const;
	
	bool operator== (const mnslp_ipfix_value_field &rhs) const;
	
	/**
	 *Print the value on cout
	 */
	void print(void);
	
	/**
	 *Print the value in a string and return the string.
	 */
	std::string to_string(void);
	
};

/**
 * \class msnlp_ipfix_field_container
 *
 *
 * \brief This class is used to maintain all the fields that are possible to exchange
 * 
 * \author Andres Marentes
 *
 * \version 0.1 
 *
 * \date 2014/12/18 9:57:00
 *
 * Contact: la.marentes455@uniandes.edu.co
 *  
 */
class msnlp_ipfix_field_container
{
private:

	std::vector<mnslp_ipfix_field> fieldTypeList;	///< The field list 				  
	
public:
    /**
     * Contructor method.
     */ 
    msnlp_ipfix_field_container();
    
    /**
     * Destructor method.
     */ 
    ~msnlp_ipfix_field_container();

    /**
     * Insert all fields in the forward direction
     */ 
    void initialize_forward(void);
    
    /**
     * Insert all fields in the reverse direction
     */ 
    void initialize_reverse(void);
    
    /**
     * Get a field by its keys
     * @throw mnslp_ipfix_bad_argument - Field not found in the container
     */ 
    mnslp_ipfix_field get_field( int eno, int type );
        
    /**
     * Add a field into the listby given the field type
     * @throw mnslp_ipfix_bad_argument - Field already exists in the container
     */ 
    void AddFieldType(ipfix_field_type_t &param);

    /**
     * Add a field into the list by given the field type attributes
     * @throw mnslp_ipfix_bad_argument - Field already exists in the container
     */ 
    void AddFieldType(int _eno, int _ftype, ssize_t _length, 
					  int _coding, 
					  const std::string _name, 
					  const std::string _documentation );

    /**
     * Return the number of fields in the container
     */ 
    int get_num_fields(void);
    
    /**
     * Erase all fields in the container. This will destroy all fields inside.
     */ 
    inline void clear(void){ fieldTypeList.clear(); }
    
};

inline int msnlp_ipfix_field_container::get_num_fields(void)
{
	return fieldTypeList.size();
}


}

#endif // MNSLP_IPFIX_FIELDS_H
