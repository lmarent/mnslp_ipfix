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

void mnslp_template_container::delete_template(mnslp_ipfix_template * param)
{
	
	int index = 0;
	for(std::vector<mnslp_ipfix_template>::iterator it = templateList.begin(); 
		it != templateList.end(); ++it) {
		if ( ((*it).get_type() ==  param->get_type()) && 
			 ((*it).get_template_id() ==  param->get_template_id()) )
			break;
		index = index + 1;
	}
	
	templateList.erase(templateList.begin()+ index);
}

}
