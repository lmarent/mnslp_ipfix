/// ----------------------------------------*- mode: C++; -*--
/// @file mnslp_ipfix_data_record.cpp
/// Tools for processing IPFIX messages in NSIS metering.
/// ----------------------------------------------------------
/// $Id: mnslp_ipfix_data_record.cpp 2558 2014-11-14 14:11:00 amarentes $
/// $HeadURL: https://./src/mnslp_ipfix_data_record.c $
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

#include "mnslp_ipfix_data_record.h"
#include <iostream>
#include <sstream>



namespace mnslp_ipfix
{


mnslp_ipfix_data_record::mnslp_ipfix_data_record(uint16_t _template_id):
template_id(_template_id)
{

}

mnslp_ipfix_data_record::mnslp_ipfix_data_record(const mnslp_ipfix_data_record &param)
{

	template_id = param.template_id;
	field_data = param.field_data;
	
}

mnslp_ipfix_data_record::~mnslp_ipfix_data_record()
{

}

void 
mnslp_ipfix_data_record::insert_field(int eno, int ftype, 
									  mnslp_ipfix_value_field &value)
{
	mnslp_ipfix_field_key key = mnslp_ipfix_field_key(eno, ftype);	
	insert_field(key, value);

}


void 
mnslp_ipfix_data_record::insert_field(mnslp_ipfix_field_key &param, 
									  mnslp_ipfix_value_field &value)
{
	field_data.insert( std::pair<mnslp_ipfix_field_key, 
						mnslp_ipfix_value_field>(param,value) );
}


int 
mnslp_ipfix_data_record::get_num_fields()
{
	return field_data.size();
}


mnslp_ipfix_value_field 
mnslp_ipfix_data_record::get_field(mnslp_ipfix_field_key &param)
{
	std::map<mnslp_ipfix_field_key, mnslp_ipfix_value_field>::iterator it;
	it=field_data.find(param);
	if (it == field_data.end())
		throw mnslp_ipfix_bad_argument("Parameter field was not found");
	else	
		return it->second;
}

mnslp_ipfix_value_field 
mnslp_ipfix_data_record::get_field(int eno, int ftype)
{
	mnslp_ipfix_field_key key = mnslp_ipfix_field_key(eno, ftype);
	return get_field(key);
}


uint16_t 
mnslp_ipfix_data_record::get_length(mnslp_ipfix_field_key &param)
{
	mnslp_ipfix_value_field fieldvalue = get_field(param);
	return fieldvalue.get_length();
}

uint16_t 
mnslp_ipfix_data_record::get_length(int eno, int ftype)
{
	mnslp_ipfix_field_key key = mnslp_ipfix_field_key(eno, ftype);
	return get_length(key);
}

void 
mnslp_ipfix_data_record::clear()
{
	// free the memory assigned to data values.	
	field_data.clear();
}

std::string 
mnslp_ipfix_data_record::to_string()
{
	std::map<mnslp_ipfix_field_key, mnslp_ipfix_value_field>::iterator iter;
	std::string strToReturn;
	
	for (iter = field_data.begin(); iter != field_data.end(); ++iter) {
         mnslp_ipfix_field_key temp = iter->first;
         strToReturn.append(temp.to_string()); 
         strToReturn.append("=");
         strToReturn.append((iter->second).to_string());
    }
    return strToReturn;
}

bool 
mnslp_ipfix_data_record::operator== (mnslp_ipfix_data_record& rhs)
{

	std::map<mnslp_ipfix_field_key, mnslp_ipfix_value_field>::iterator iter;
	for (iter = field_data.begin(); iter != field_data.end(); ++iter) {
		mnslp_ipfix_field_key key = iter->first;
		mnslp_ipfix_value_field tmp = rhs.get_field(key);
		mnslp_ipfix_value_field tmp2 = iter->second;
		if (tmp != tmp2){
			return false;
		}
	}
	return true;
}


mnslp_ipfix_data_record& 
mnslp_ipfix_data_record::operator= (mnslp_ipfix_data_record& rhs)
{
	// Delete all field values in the container.
	field_data.clear();
	
	template_id = rhs.template_id;
	field_data = rhs.field_data;
	
	return *this;

}

bool 
mnslp_ipfix_data_record::operator!= (mnslp_ipfix_data_record& rhs)
{

	std::map<mnslp_ipfix_field_key, mnslp_ipfix_value_field>::iterator iter;
	for (iter = field_data.begin(); iter != field_data.end(); ++iter) {
		std::cout << "we are in != operator data record" << std::endl;
		mnslp_ipfix_field_key key = iter->first;
		mnslp_ipfix_value_field tmp = rhs.get_field(key);
		mnslp_ipfix_value_field tmp2 = iter->second;
		if (tmp != tmp2){
			std::cout << "Key:" << key.to_string() << "field 1:" 
					  << tmp.to_string() << " field 2:" 
					  << tmp2.to_string() << std::endl;
			return true;
		}
	}
	return false;
}

}
