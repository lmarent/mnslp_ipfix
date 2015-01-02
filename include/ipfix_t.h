/// ----------------------------------------*- mode: C; -*--
/// @file ipfix_t.h
/// Tools for processing IPFIX messages in NSIS metering.
/// ----------------------------------------------------------
/// $Id: ipfix_t.h 2558 2014-12-31 10:11:00 amarentes $
/// $HeadURL: https://./include/ipfix_t.h $
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

#ifndef MNSLP_IPFIX_T_H
#define MNSLP_IPFIX_T_H	

#include "mnslp_ipfix_template.h"
#include "mnslp_ipfix_field.h"


namespace mnslp_ipfix
{


#define IPFIX_DEFAULT_BUFLEN 		1400
#define IPFIX_VERSION_NF9           0x09
#define IPFIX_VERSION               0x0A

	
/**
 * \class ipfix_t
 *
 *
 * \brief This class contains the raw information required for a messsage
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
class ipfix_t
{
    public: 
		int              		 sourceid;    ///< domain id of the exporting process.
		int              		 version;     ///< ipfix version to export.
		mnslp_template_container templates;   ///< list of templates. 

		char       *buffer;          		  ///< output buffer - the actual message when exported is put it here.
		int         nrecords;        		  ///< no. of records in buffer.
		size_t      offset;           		  ///< Number of bytes in the buffer, output buffer fill level.
		int 		buffer_lenght;    		  ///< output buffer allocated length, it is readjusted as more memory is needed.
		uint32_t    seqno;            		  ///< sequence no. of next message.

		int        cs_tid;            		  ///< template id of current dataset 
		int        cs_bytes;          		  ///< size of current set
		int 	   cs_offset;				  ///< offset required for the current set
		uint8_t    *cs_header;        		  ///< start of current set

        /* -------   Header nf9 fields */
        /* nf9 fields */
        uint16_t   count;       			  ///< total number of record in this packet
        uint32_t   sysuptime;   			  ///< sysuptime in milliseconds
        uint32_t   unixtime;    			  ///< seconds since 1970 

        /* Header ipfix fields */
        uint16_t   length;      			  ///< total number of record in this packet
        uint32_t   exporttime;  			  ///< seconds since 1970
		

	
	/**
	 * Constructor of the class
	 */
	ipfix_t();
	
	/**
	 * Destructor of the class
	 */
	~ipfix_t();
	
	/**
	 * Assignment operator
	 */
	ipfix_t & operator=(const ipfix_t & rhs);
	
	/**
	*  Equals to operator. 
	*  It is equal when it has the same amount of templates and every 
	*    template is equal.
	*/
	bool operator== (const ipfix_t& rhs);
		
	/**
	*  Not equal to operator. 
	*  It is not equal when it does not have the same amount of templates and at least 
	*    one field is not a template.
	*/
	bool operator!= (const ipfix_t& rhs);
	
	/**
	 * Copy the definition of a message into the buffer. It is assumed that 
	 * the message is in the correct ipfix message
	 */
	void copy_raw_message(char * msg, size_t _offset);

};

} // mnslp_ipfix

#endif // MNSLP_IPFIX_T_H
