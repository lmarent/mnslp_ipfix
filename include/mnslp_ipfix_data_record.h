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

class mnslp_ipfix_data_record
{

private:
    std::map<mnslp_ipfix_field_key, mnslp_ipfix_value_field > field_data;    	/* Data values for every field */
    std::map<mnslp_ipfix_field_key, uint16_t > field_length;	/* Variable length definition */

public:
    
    mnslp_ipfix_data_record();
    
    ~mnslp_ipfix_data_record();
    
    void insert_field(int eno, int ftype, mnslp_ipfix_value_field &value);
            
    void insert_field_length(int eno, int ftype, uint16_t length);

    void insert_field(mnslp_ipfix_field_key &param, mnslp_ipfix_value_field &value);

    void insert_field_length(mnslp_ipfix_field_key &param, uint16_t length);
        
    int get_num_fields();
    
    int get_num_field_length();
    
    mnslp_ipfix_value_field get_field(int eno, int ftype);

    mnslp_ipfix_value_field get_field(mnslp_ipfix_field_key &param);
    
    uint16_t get_length(int eno, int ftype);

    uint16_t get_length(mnslp_ipfix_field_key &param);
    
    void clear();
    
};

}

#endif // MNSLP_IPFIX_DATA_RECORD_H
