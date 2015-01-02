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

#ifndef MNSLP_IPFIX_TEMPLATE_H
#define MNSLP_IPFIX_TEMPLATE_H

#include <vector>
#include <map>
#include "mnslp_ipfix_field.h"

namespace mnslp_ipfix
{

typedef enum 
{
    DATA_TEMPLATE, OPTION_TEMPLATE
} ipfix_templ_type_t;

typedef enum
{
	KNOWN, UNKNOWN
} ipfix_unknown_field_t;	

/**
 * \class ipfix_template_field_t
 *
 * \brief This class contains the list of fields belonging to a template
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
typedef struct
{
    uint16_t            	flength;           /* less or eq. elem->flength  */
    ipfix_unknown_field_t   unknown_f;         /* set if unknown elem */
    int                 	relay_f;           /* just relay no, encoding (exp.) */
    mnslp_ipfix_field   	elem;
} ipfix_template_field_t;

/**
 * \class mnslp_ipfix_template
 *
 * \brief This class represents a template.
 * 
 *  We conceptualize a template as a set of field definitions. The field set 
 *  is composed by two subsets: a data set field and a scope field set. 
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
class mnslp_ipfix_template
{
    private:
		ipfix_templ_type_t     				type;  			///< data or option template 
		time_t                  			tsend; 			///< time of last transmission
		uint16_t                			tid;			///< Template id
		int 								maxfields; 		///< Maximum number of fields
		std::vector<ipfix_template_field_t> datafields;		///< Subset of data fields
		std::vector<ipfix_template_field_t> scopefields;	///< Subset of scope fields
	public:
		
		/**
		 * Constructor for the class mnslp_ipfix_template
		 */
		inline mnslp_ipfix_template():tsend(0){}
		
		/**
		 * Constructor from the data of another template
		 */
		mnslp_ipfix_template(const mnslp_ipfix_template &rhs); 
		
		/**
		 * Destructor for the class mnslp_ipfix_template
		 */
		inline ~mnslp_ipfix_template(){}
		
		/**
		 * Set the id of the template
		 * @param _tid - template id.
		 */
		inline void set_id(uint16_t _tid){ tid = _tid; }
		
		/**
		 * Set the type of the template ( data or option)
		 * @param _type - Template type.
		 */
		inline void set_type(ipfix_templ_type_t _type){ type = _type; }
		
		/**
		 * Set the last datetime in which it was send
		 * @param _tsend - Send time
		 */
		inline void set_time_send(time_t _tsend){ tsend = _tsend; }
		
		/**
		 * Set the maximum number of fields that the field set can have
		 * @param _maxfields 	- Field maximum cardinality
		 */
		inline void set_maxfields(int _maxfields){ maxfields = _maxfields; }
		
		/**
		 * Get the type of the template ( data or option)
		 */
		inline ipfix_templ_type_t get_type(void)
		{
			return type;
		}
		
		/**
		 * Get the last datetime in which it was send
		 */
		inline time_t get_tsend(void)
		{
			return tsend;
		}
		
		/**
		 * Get the template id
		 */
		inline uint16_t get_template_id(void)
		{
			return tid;
		}
		
		/**
		 * Get the number of data fields in the template
		 */
		inline int get_number_datafields(void)
		{
			return (int) datafields.size();
		}
		
		/**
		 * Get the number of scope fields in the template
		 */
		inline int get_number_scopefields(void)
		{
			return (int) scopefields.size();
		}
		
		/**
		 * Get the total number of fields in the template
		 */
		inline int get_numfields(void)
		{
			return (int)  ( datafields.size() + scopefields.size());
		}
		
		/**
		 * Get the maximum number of fields that can be associated to the template
		 */
		inline int get_maxfields(void)
		{
			return maxfields;
		}
		
		/**
		 * Add a new field to the template
		 */
		void add_field(uint16_t _flength, 
					   ipfix_unknown_field_t _unknown_f, 
					   int _relay_f,  
					   mnslp_ipfix_field  &_field);
		
		/**
		 * Get a field located in the position i
		 * The order is scope and then data fields 
		 * @param i - position of the field to get. 
		 */
		ipfix_template_field_t get_field(int i);
		
		/**
		 * Remove all fields market with the unknown tag.
		 */
		void remove_unknown_fields();
		
		/**
		*  Equals to operator. 
		*  It is equal when it has the same amount of fields and every 
		*    field is in the same position and equal.
		*/
		bool operator== (const mnslp_ipfix_template& rhs);

		/**
		*  Not equal to operator. 
		*  It is not equal when it does not have the same amount of fields 
		*    and exists a different field or it has a different position.
		*/
		bool operator!= (const mnslp_ipfix_template& rhs);	
		
		/** Assignment operator. It sets the values for the template 
		 * 						 from another template.
		*  @param  the template to copy from.
		*/
		mnslp_ipfix_template& operator= (const mnslp_ipfix_template&);
		
		/** 
		 * Creates a new object returning its pointer. 
		 * The data to be used for the new template is taken from the
		 * instance.
		*/
		mnslp_ipfix_template * copy(void) const;
						
};

/**
 * \class mnslp_template_container
 *
 * \brief This class stores all templates in a message
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
class mnslp_template_container
{
	private:
		std::map<uint16_t, mnslp_ipfix_template *> templateList; ///< List of templates
	
	protected:
		
		/**
		 * Get the template with id equal to templid
		 * @param templid	- id of the template to look for
		 */
		const mnslp_ipfix_template * get_const_template(const uint16_t templid) const;

	
	public:
    
		/**
		 * Constructor for the mnslp_template_container class
		 */
		inline mnslp_template_container(void){}

		/** 
		 * Constructor from the data of another template container
		 */
		mnslp_template_container(const mnslp_template_container &rhs);
		
		/**
		 * Destructor for the mnslp_template_container class
		 */
		~mnslp_template_container(void);
    
		/**
		 * Add a new template to the container
		 */
		inline void add_template(mnslp_ipfix_template *param )
		{
			templateList.insert ( std::pair<uint16_t, mnslp_ipfix_template *>(param->get_template_id(),param) );
		}
				
		/**
		 * delete all templates from the container
		 */
		void delete_all_templates(void);
		
		/**
		 * delete the template with id equal to templid
		 * @param templid	- id of the template to delete
		 */
		void delete_template(uint16_t templid);
		
		/**
		 * verify whether a template with id equal to templid exists or not
		 * @param templid	- id of the template to look for
		 */
		bool exists_template(uint16_t templid);
		
		/**
		 * Get the template with id equal to templid
		 * @param templid	- id of the template to look for
		 */
		mnslp_ipfix_template * get_template(uint16_t templid);
				
		/**
	    * Return the number of templates included
	    */
	    inline int get_num_templates(void){  return templateList.size(); }
	    
	    /**
		*  Equals to operator. 
		*  It is equal when it has the same amount of templates and every 
		*    template is equal.
		*/
		bool operator== (const mnslp_template_container& rhs);
		
		/**
		*  Not equal to operator. 
		*  It is not equal when it does not have the same amount of templates and at least 
		*    one field is not a template.
		*/
		bool operator!= (const mnslp_template_container& rhs);
		
		/** Assignment operator. It sets the values for the template container 
		 * 						 from another template container.
		*  @param  the template container to copy from.
		*/
		mnslp_template_container& operator= (const mnslp_template_container&);				
		
		typedef std::map<uint16_t, mnslp_ipfix_template *>::const_iterator templ_iterator;
		
		templ_iterator begin() const throw() { return templateList.begin(); }
		templ_iterator end() const throw() { return templateList.end(); }
};

}

#endif // MNSLP_IPFIX_TEMPLATE_H
