/// ----------------------------------------*- mode: C++; -*--
/// @file mnslp_ipfix_templates.h
/// Tools for processing IPFIX messages in NSIS metering.
/// ----------------------------------------------------------
/// $Id: mnslp_ipfix_templates.h 2558 2014-11-26 14:11:00 amarentes $
/// $HeadURL: https://./include/mnslp_ipfix_templates.h $
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

/* ipfix templates
 */

#include <vector>
#include "mnslp_ipfix_fields.h"

namespace mnslp_ipfix
{

typedef enum 
{
    DATA_TEMPLATE, OPTION_TEMPLATE
} ipfix_templ_type_t;

typedef struct
{
    uint16_t            flength;           /* less or eq. elem->flength  */
    int                 unknown_f;         /* set if unknown elem */
    int                 relay_f;           /* just relay no, encoding (exp.) */
    mnslp_ipfix_field   *elem;
} ipfix_template_field_t;

class mnslp_ipfix_template
{
    private:
		ipfix_templ_type_t     				type;  /* data or option template   */
		time_t                  			tsend; /* time of last transmission */
		uint16_t                			tid;
		int 								maxfields; 		/* Maximum number of fields */
		std::vector<ipfix_template_field_t> datafields;
		std::vector<ipfix_template_field_t> scopefields;
	public:
		
		inline mnslp_ipfix_template(){}
		
		inline ~mnslp_ipfix_template(){}
		
		inline void set_id(uint16_t _tid){ tid = _tid; }
		
		inline void set_type(ipfix_templ_type_t _type){ type= _type; }
		
		inline void set_time_send(time_t _tsend){ tsend = _tsend; }
		
		inline void set_maxfields(int _maxfields){ maxfields = _maxfields; }
		
		inline ipfix_templ_type_t get_type(void)
		{
			return type;
		}
		
		inline time_t get_tsend(void)
		{
			return tsend;
		}
		
		inline uint16_t get_template_id(void)
		{
			return tid;
		}
		
		inline int get_number_datafields(void)
		{
			return (int) datafields.size();
		}
		
		inline int get_number_scopefields(void)
		{
			return (int) scopefields.size();
		}
		
		inline int get_numfields(void)
		{
			return (int)  ( datafields.size() + scopefields.size());
		}
		
		inline int get_maxfields(void)
		{
			return maxfields;
		}
		
		/** type == 1 Then data Field
		 * else scope field
		 */
		inline void add_field(uint16_t _flength, int _unknown_f, 
								  int _relay_f, int type,  mnslp_ipfix_field * _field)
		{
			ipfix_template_field_t t;
			t.flength = _flength;
			t.unknown_f = _unknown_f;
			t.relay_f = _relay_f;
			t.elem = _field;
			if (type == 1)
				datafields.push_back(t);
			else
				scopefields.push_back(t);
		}
		
		ipfix_template_field_t get_field(int i);
			
};

class mnslp_template_container
{
	private:
		std::vector<mnslp_ipfix_template> templateList;
	
	public:
    
		inline void add_template(mnslp_ipfix_template &param )
		{
			templateList.push_back(param);
		}
		
		void delete_template(mnslp_ipfix_template * param);
};

}
