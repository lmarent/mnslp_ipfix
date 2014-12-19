/// ----------------------------------------*- mode: C; -*--
/// @file mnslp_ipfix_data_record.h
/// Tools for processing IPFIX messages in NSIS metering.
/// Export declarations of mnslp ipfix messages.
/// ----------------------------------------------------------
/// $Id: mnslp_ipfix_data_record.h 2558 2014-11-29 10:14:00 amarentes $
/// $HeadURL: https://./include/mnslp_ipfix_data_record.h $
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

#ifndef MNSLP_IPFIX_DATA_RECORD_H
#define MNSLP_IPFIX_DATA_RECORD_H

#include <map>
#include "mnslp_ipfix_fields.h"

namespace mnslp_ipfix
{

/**
 * \class mnslp_ipfix_data_record
 *
 *
 * \brief Maintains the data values within a record.
 *
 * This class is meant to maintain all values for a record. The fields in
 *   a record are related with a template.
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
class mnslp_ipfix_data_record
{ 

    uint16_t template_id;														///< template id which is associated with this data record.
    std::map<mnslp_ipfix_field_key, mnslp_ipfix_value_field > field_data;    	///< Data value map for every field 

public:
        
    /** 
     * Creates an data_record
     */
    mnslp_ipfix_data_record(uint16_t _template_id);
    
    /** 
	 * Contructor of the class, it copies from another data record.
	 */
	mnslp_ipfix_data_record(const mnslp_ipfix_data_record &param);
    
    ~mnslp_ipfix_data_record();
    
    inline uint16_t get_template_id(){ return template_id; }
    
    /**
     *  Insert a field value based on components of the field key (eno,ftype).
     */
    void insert_field(int eno, int ftype, mnslp_ipfix_value_field &value);
            
    /** 
     * Insert a field value based on the field key (param).
     */
    void insert_field(mnslp_ipfix_field_key &param, mnslp_ipfix_value_field &value);
        
    /** 
     * Return the number of fields with a value
     */
    int get_num_fields();
        
    /** 
     * Return the field value based on components of the field key
     */
    mnslp_ipfix_value_field get_field(int eno, int ftype);

    /** 
     * Return the field value based on the field key
     */
    mnslp_ipfix_value_field get_field(mnslp_ipfix_field_key &param);
    
    /** 
     * Return the length on bytes of the value based on components of the field key
     */
    uint16_t get_length(int eno, int ftype);

    /** 
     * Return the length on bytes of the value based on the field key
     */
    uint16_t get_length(mnslp_ipfix_field_key &param);
    
    /** 
     * delete all fields values inserted
     */
    void clear();
    
    /** 
     * Create an string representing all field values.
     */
    std::string to_string();
    
	/**
	 *  Equals to operator. 
	 *  It is equal when it has the same amount of field values and every value is equal.
	 */
	bool operator== (mnslp_ipfix_data_record& rhs);
	
	/** 
	 * Assignment operator. 
	*/ 
	mnslp_ipfix_data_record& operator= (mnslp_ipfix_data_record&);
	
	/** 
	* Not equal to operator. 
	*/ 
	bool operator != (mnslp_ipfix_data_record &rhs);
    
};


}

#endif // MNSLP_IPFIX_DATA_RECORD_H
