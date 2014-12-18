/// ----------------------------------------*- mode: C++; -*--
/// @file mnslp_ipfix_templates.cpp
/// Tools for processing IPFIX messages in NSIS metering.
/// ----------------------------------------------------------
/// $Id: mnslp_ipfix_templates.cpp 2558 2014-11-14 14:11:00 amarentes $
/// $HeadURL: https://./include/mnslp_ipfix_templates.cpp $
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

#include "mnslp_ipfix_templates.h"
#include "mnslp_ipfix_exception.h"
#include <iostream>

namespace mnslp_ipfix
{

#define TEMPLATE_READ16(b) ((*(b)<<8)|*((b)+1))
#define TEMPLATE_READ32(b) ((((((*(b)<<8)|*(b+1))<<8)|(*(b+2)))<<8)|*(b+3))



ipfix_template_field_t mnslp_ipfix_template::get_field(int i)
{
	if ( (i >= get_numfields()) || (i < 0) ){
		throw mnslp_ipfix_bad_argument("Invalid field number");
	}
	else
	{
		std::cout << "number of scope fields:" << get_number_scopefields() << std::endl;
		std::cout << "number of data fields:" << get_number_datafields() << std::endl;
				
		if (i < get_number_scopefields())
			return scopefields[i];
		else
			return datafields[i];
	}
}

void
mnslp_ipfix_template::remove_unknown_fields()
{
	// Delete unknown datafields
	std::vector<ipfix_template_field_t>::iterator iter;
	for (iter = datafields.begin(); iter != datafields.end(); ) {
		if ((*iter).unknown_f == 1 )
			iter = datafields.erase(iter);
		else
			++iter;
	}
	
	// Delete unknown scopefields
	std::vector<ipfix_template_field_t>::iterator iter2;
	for (iter2 = scopefields.begin(); iter2 != scopefields.end(); ) {
		if ((*iter2).unknown_f == 1 )
			iter2 = scopefields.erase(iter2);
		else
			++iter2;
	}
	
}

/** If type == 1 Then data Field
 *  else scope field
*/
void mnslp_ipfix_template::add_field(uint16_t _flength, int _unknown_f, 
								     int _relay_f, int type,  
									 mnslp_ipfix_field * _field)
{
	ipfix_template_field_t t;
	t.flength = _flength;
	t.unknown_f = _unknown_f;
	t.relay_f = _relay_f;
	t.elem = _field;
	
	std::cout << "type of template:" << type << std::endl;
	if (type == DATA_TEMPLATE)
		datafields.push_back(t);
	else
		scopefields.push_back(t);
}

void mnslp_template_container::delete_template(mnslp_ipfix_template * param)
{
	std::map<uint16_t, mnslp_ipfix_template *>::iterator it;
	for( it= templateList.begin(); it != templateList.end(); ++it) {
		if ( it->first == param->get_template_id() )
			break;
	}
	
	delete(it->second);
	templateList.erase(it);
}


void mnslp_template_container::delete_template(uint16_t templid)
{
	std::map<uint16_t, mnslp_ipfix_template *>::iterator it;
	for( it = templateList.begin(); it != templateList.end(); ++it) {
		if ( (it->first == templid) )
			break;
	}
	
	delete(it->second);
	templateList.erase(it);
}

bool mnslp_template_container::exists_template(uint16_t templid)
{
	
	for(std::map<uint16_t, mnslp_ipfix_template *>::iterator it = templateList.begin(); 
		it != templateList.end(); ++it) {
		if ( (it->first ==  templid ) )
			return true;		
	}
	return false;

}

mnslp_ipfix_template *
mnslp_template_container::get_template(uint16_t templid)
{

	std::map<uint16_t, mnslp_ipfix_template *>::iterator it;
	for( it=templateList.begin(); it != templateList.end(); ++it) {
		if ( it->first ==  templid  ){
			
			std::cout << "template found:" << "setid:" << templid <<  std::endl;
			std::cout << "Max fields:" << (it->second)->get_maxfields() << std::endl;
			std::cout << "Num fields:" << (it->second)->get_numfields() << std::endl;
			return it->second;
		}		
	}
	return NULL;

}


void 
mnslp_template_container::delete_all_templates(void)
{
	std::map<uint16_t, mnslp_ipfix_template *>::iterator it;
	for( it=templateList.begin(); it != templateList.end(); ++it) {
		delete (it->second);
	}
	templateList.clear();

}

 
mnslp_template_container::~mnslp_template_container(void)
{
	std::map<uint16_t, mnslp_ipfix_template *>::iterator it;
	for( it=templateList.begin(); it != templateList.end(); ++it) {
		delete (it->second);
	}
	templateList.clear();

}


}
