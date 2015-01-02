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

#include "mnslp_ipfix_template.h"
#include "mnslp_ipfix_exception.h"
#include <iostream>
#include <stdexcept>      // std::out_of_range

namespace mnslp_ipfix
{

mnslp_ipfix_template::mnslp_ipfix_template(const mnslp_ipfix_template &rhs)
{
	type = rhs.type;
	tsend = rhs.tsend;
	tid = rhs.tid;
	maxfields = rhs.maxfields;
	datafields = rhs.datafields;
	scopefields = rhs.scopefields;
}

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
		if ((*iter).unknown_f == UNKNOWN )
			iter = datafields.erase(iter);
		else
			++iter;
	}
	
	// Delete unknown scopefields
	std::vector<ipfix_template_field_t>::iterator iter2;
	for (iter2 = scopefields.begin(); iter2 != scopefields.end(); ) {
		if ((*iter2).unknown_f == UNKNOWN )
			iter2 = scopefields.erase(iter2);
		else
			++iter2;
	}
	
}

/** If type == 0 Then data Field
 *  else scope field
*/
void mnslp_ipfix_template::add_field(uint16_t _flength, 
                                     ipfix_unknown_field_t _unknown_f, 
								     int _relay_f,
									 mnslp_ipfix_field  &_field)
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
mnslp_ipfix_template::operator== (const mnslp_ipfix_template& rhs)
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
				 ( lfs_item.elem != rhs_item.elem )){
				return false;
			}
		}
				
		for (int j = 0; j < scopefields.size(); j++){
			ipfix_template_field_t lfs_item = scopefields[j];
			ipfix_template_field_t rhs_item = rhs.scopefields[j];
			if ( ( lfs_item.flength != rhs_item.flength) ||
				 ( lfs_item.unknown_f != rhs_item.unknown_f) ||
				 ( lfs_item.relay_f != rhs_item.relay_f) ||
				 ( lfs_item.elem != rhs_item.elem))
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
mnslp_ipfix_template::operator!= (const mnslp_ipfix_template& rhs)
{
	return !(operator==(rhs));
}

mnslp_ipfix_template& 
mnslp_ipfix_template::operator=(const mnslp_ipfix_template &rhs)
{
	type = rhs.type;
	tsend = rhs.tsend;
	tid = rhs.tid;
	maxfields = rhs.maxfields;
	datafields = rhs.datafields;
	scopefields = rhs.scopefields;
	
	return *this;
}

mnslp_template_container::mnslp_template_container(const mnslp_template_container &rhs)
{
	std::map<uint16_t, mnslp_ipfix_template *>::const_iterator it;
	for( it = rhs.templateList.begin(); it != rhs.templateList.end(); ++it)
		templateList[it->first] = (it->second)->copy();
	
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
	
	for(std::map<uint16_t, mnslp_ipfix_template*>::iterator it = templateList.begin(); 
		it != templateList.end(); ++it) {
		if ( (it->first ==  templid ) )
			return true;		
	}
	return false;

}

mnslp_ipfix_template *
mnslp_template_container::get_template(uint16_t templid)
{

	std::map<uint16_t, mnslp_ipfix_template* >::iterator it;
	for( it=templateList.begin(); it != templateList.end(); ++it) {
		if ( it->first ==  templid  ){			
			return it->second;
		}		
	}
	throw mnslp_ipfix_bad_argument("Template not found in the container");

}


const mnslp_ipfix_template *
mnslp_template_container::get_const_template(const uint16_t templid) const
{

	std::map<uint16_t, mnslp_ipfix_template* >::const_iterator it;
	for( it=templateList.begin(); it != templateList.end(); ++it) {
		if ( it->first ==  templid  ){			
			return it->second;
		}		
	}
	throw mnslp_ipfix_bad_argument("Template not found in the container");

}


mnslp_ipfix_template *
mnslp_ipfix_template::copy(void) const
{
	mnslp_ipfix_template *q = NULL;
	q = new mnslp_ipfix_template(*this);
	return q;
}

void 
mnslp_template_container::delete_all_templates(void)
{
	std::map<uint16_t, mnslp_ipfix_template *>::iterator it;
	for( it=templateList.begin(); it != templateList.end(); ++it) {
		delete(it->second);
	}			
	templateList.clear();

}

 
mnslp_template_container::~mnslp_template_container(void)
{
	delete_all_templates();
}

bool 
mnslp_template_container::operator== (const mnslp_template_container& rhs)
{
	if (rhs.templateList.size() != templateList.size())
		return false;
	
	try
	{
		std::map<uint16_t, mnslp_ipfix_template *>::iterator it;
		for( it=templateList.begin(); it != templateList.end(); ++it) {
			const mnslp_ipfix_template *temp = rhs.get_const_template(it->first);
			if ( *(it->second) != *temp )
				return false;
		}
	}
	catch (mnslp_ipfix_bad_argument bad) 
	{
		// One of the templates was not found, so we have to return false.
		return false;
	}
	return true;
	
}

bool
mnslp_template_container::operator!= (const mnslp_template_container& rhs)
{
	return !(operator==(rhs));
}

mnslp_template_container&
mnslp_template_container::operator=(const mnslp_template_container &rhs)
{
	delete_all_templates();
	
	std::map<uint16_t, mnslp_ipfix_template *>::const_iterator it;
	for( it = rhs.templateList.begin(); it != rhs.templateList.end(); ++it)
		templateList[it->first] = (it->second)->copy();
	
	return *this;
	
}


}
