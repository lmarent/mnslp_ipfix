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
#include <stdexcept>      // std::out_of_range

namespace mnslp_ipfix
{

ipfix_template_field_t mnslp_ipfix_template::get_field(int i)
{
	if ( (i >= get_numfields()) || (i < 0) ){
		throw mnslp_ipfix_bad_argument("Invalid field number");
	}
	else
	{				
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

/** If type == 0 Then data Field
 *  else scope field
*/
void mnslp_ipfix_template::add_field(uint16_t _flength, 
                                     int _unknown_f, 
								     int _relay_f,
									 mnslp_ipfix_field * _field)
{
	ipfix_template_field_t t;
	
	t.flength = _flength;
	t.unknown_f = _unknown_f;
	t.relay_f = _relay_f;
	t.elem = _field;
			
	if (type == DATA_TEMPLATE)
		datafields.push_back(t);
	else
		scopefields.push_back(t);
}

bool
mnslp_ipfix_template::operator== (mnslp_ipfix_template& rhs)
{
	
	if (rhs.type != type)
		return false;
		
	if (rhs.tid != tid)	
		return false;
	
	if (rhs.maxfields != maxfields)	
		return false;
		
	try
	{
		for (int i = 0; i < datafields.size(); i++){
			ipfix_template_field_t lfs_item = datafields[i];
			ipfix_template_field_t rhs_item = rhs.datafields[i];
			if ( ( lfs_item.flength != rhs_item.flength) ||
				 ( lfs_item.unknown_f != rhs_item.unknown_f) ||
				 ( lfs_item.relay_f != rhs_item.relay_f) ||
				 ( *(lfs_item.elem) != *(rhs_item.elem))){
				return false;
			}
		}
				
		for (int j = 0; j < scopefields.size(); j++){
			ipfix_template_field_t lfs_item = scopefields[j];
			ipfix_template_field_t rhs_item = rhs.scopefields[j];
			if ( ( lfs_item.flength != rhs_item.flength) ||
				 ( lfs_item.unknown_f != rhs_item.unknown_f) ||
				 ( lfs_item.relay_f != rhs_item.relay_f) ||
				 ( *(lfs_item.elem) != *(rhs_item.elem)))
				return false;
		}
				
		return true;
	
	}
	catch(const std::out_of_range& oor)
	{
		return false;
	}
	
}

bool
mnslp_ipfix_template::operator!= (mnslp_ipfix_template& rhs)
{
	return !(operator==(rhs));
}


void mnslp_template_container::delete_template(mnslp_ipfix_template * param)
{
	std::string func = "delete_template";
	std::cout << "func:" << func << std::endl; 
	
	std::map<uint16_t, mnslp_ipfix_template *>::iterator it;
	for( it= templateList.begin(); it != templateList.end(); ++it) {
		if ( it->first == param->get_template_id() )
			break;
	}
	
	delete(it->second);
	templateList.erase(it);
	
	std::cout << "end func:" << func << std::endl; 
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

bool 
mnslp_template_container::operator== (mnslp_template_container& rhs)
{
	if (rhs.templateList.size() != templateList.size())
		return false;
	
	std::map<uint16_t, mnslp_ipfix_template *>::iterator it;
	for( it=templateList.begin(); it != templateList.end(); ++it) {
		if ( *(it->second) != *(rhs.get_template(it->first)) )
			return false;
	}
	
	return true;
	
}

bool
mnslp_template_container::operator!= (mnslp_template_container& rhs)
{
	return !(operator==(rhs));
}

}
