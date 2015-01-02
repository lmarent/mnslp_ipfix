/// ----------------------------------------*- mode: C; -*--
/// @file mnslp_ipfix_message.h
/// Tools for processing IPFIX messages in NSIS metering.
/// Export declarations of mnslp ipfix messages.
/// ----------------------------------------------------------
/// $Id: mnslp_ipfix_message.h 2558 2014-11-26 14:11:00 amarentes $
/// $HeadURL: https://./include/mnslp_ipfix_message.h $
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


/*
** ipfix.h - export declarations of mnslp_ipfix
**
** Copyright System and Computing Engineering, Universidad de los Andes
**
** $Id: mnslp_ipfix_message.h 152 2014-11-26 14:52:00 $
**
*/
#ifndef MNSLP_IPFIX_MESSAGE_H
#define MNSLP_IPFIX_MESSAGE_H

#include <inttypes.h>
#include <map>


#include "ipfix_def.h"
#include "mnslp_ipfix_field.h"
#include "mnslp_ipfix_data_record.h"
#include "mnslp_ipfix_template.h"
#include "ipfix_t.h"

#ifndef ENOTSUP
#define ENOTSUP EOPNOTSUPP
#endif


namespace mnslp_ipfix
{
	
/*------ structs ---------------------------------------------------------*/

/** netflow9 header format
 **  0                   1                   2                   3
 **    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 **   |       Version Number          |            Count              |
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 **   |                           sysUpTime                           |
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 **   |                           UNIX Secs                           |
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 **   |                       Sequence Number                         |
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 **   |                        Source ID                              |
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */   
/** ipfix header format
 **  0                   1                   2                   3
 **    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 **   |       Version Number          |            Length             |
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 **   |                         Export Time                           |
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 **   |                       Sequence Number                         |
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 **   |                     Observation Domain ID                     |
 **   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */   

#define IPFIX_HDR_BYTES_NF9         20
#define IPFIX_SETID_TEMPLATE_NF9    0
#define IPFIX_SETID_OPTTEMPLATE_NF9 1

#define IPFIX_HDR_BYTES             16
#define IPFIX_SETID_TEMPLATE        2
#define IPFIX_SETID_OPTTEMPLATE     3
#define IPFIX_FT_VARLEN             65535
#define IPFIX_FT_NOENO              0
#define IPFIX_EFT_VENDOR_BIT        0x8000

#define IPFIX_PORTNO                4739
#define IPFIX_TLS_PORTNO            4740

#define IPFIX_DFLT_TEMPLRESENDINT   30
#define IPFIX_DFLT_TEMPLLIFETIME    300


/**
 * \class export_fields_t
 *
 * \brief This structure is used to create a template associated to the message 
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
typedef struct {
    int       eno;		/* IPFIX enterprize number, 0 for standard element */
    uint16_t  ienum;		/* IPFIX information element number */
    uint16_t  length;		/* length of this element in bytes - use 65535 for varlen elements */
} export_fields_t;


/**
 * \class mnslp_ipfix_message
 *
 * \brief This class is the factory class to export and import a IPFIX messsage
 * 
 * This class is the factory class to export and import a IPFIX messsage
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
class mnslp_ipfix_message
{
	
   private:
	   ipfix_t * 							message;					///< Header Information, templates and end buffer for the message
	   msnlp_ipfix_field_container 			g_ipfix_fields;				///< Collection of fields that can be exchanged
	   time_t             					g_tstart;					///< datetime when the message starts for processing
	   uint16_t           					g_lasttid;                  ///< last sequence id givem
	   std::vector<mnslp_ipfix_data_record> data_list; 				///< List of data record asociated with the message
	   bool                             	encode_network;				///< convert to network encoding 
	
   protected:
	
	   /**
	    * Alloc memory for buffers and pointers used to construct the message
	    */
	   void init( int sourceid, int ipfix_version );
	   
	   /**
	    * Release memory for buffers and pointers used to construct the message
	    */
	   void close( void );
	   
	   /**
	    * Adds a field of vendor type to the collection of fields
	    */
	   void add_vendor_information_elements( ipfix_field_type_t *fields );
	   
	   /**
	    * creates and add a new template for the messsage
	    * @param  nfields	- The number of fields to include in the template
	    * @return 			- Template id.
	    */
	   uint16_t new_template( int nfields );
	   
	   /**
	    * Finish the current record set assigning the lenght and te template id.
	    */
	   void finish_cs( void );
	   
	   /**
	    * Write the message header. When the data associated with the message is empty 
	    * it does not do anything.
	    */
	   void _write_hdr(void );
	   	   
	   /**
	    * This method is used for completing the message once it is ready. 
	    */
	   void _output_flush( void );
	   
	   /**
	    * This method reallocates memory for the output buffer.
	    * @param additional - the number of additional bytes to allocate.
	    */
	   void allocate_additional_memory(size_t additional);
	   
	   /**
	    * This method write into the buffer the template definition.
	    * @param templ - pointer to the template object to write.
	    */
	   void _write_template( mnslp_ipfix_template  *templ );
	   			   
	   /**
	    * This method parse the header of a IPFIX message that is on mes character string
	    * @param mes - Character string containing the message
	    * 		 offset - Lenght of character string
	    */
	   void mnslp_ipfix_parse_hdr( char *mes, int offset );
	   
	   /**
	    * parse and construct a template reading characters in buf.
	    * @param setid - template id
	    * 		 buf   - character string to read
	    * 		 len   - Number of characters to read as part of template
	    * 		 nread - Number of characters read ( output).
	    */
	   void mnslp_ipfix_decode_trecord( int setid,
								const char     *buf,
								size_t         len,
								int            *nread );
	   
	   /**
	    * parse and read a field value from characters in buf.
	    * @param templ 	- template pointer associated to the record data which the field belongs to
	    * 		 buf2  	- character string to read
	    * 		 buflen - Number of characters to read
	    * 		 nread 	- Number of read characters ( output).
	    */
	   void read_field(mnslp_ipfix_template *templ, 
					   const uint8_t  *buf2, 
					   size_t   buflen, 
					   size_t   *nread);

	   /**
	    * parse and read a record set  from characters in buf.
	    * @param templ 	- template pointer associated to the record data
	    * 		 buf2  	- character string to read
	    * 		 buflen - Number of characters to read
	    * 		 nread 	- Number of read characters ( output).
	    */
	   void mnslp_ipfix_decode_datarecord( mnslp_ipfix_template *templ,
								   char      		  *buf, 
								   int                buflen,
								   int                *nread );
	   
	   /**
	    * parse a message from characters in buf.
	    * @param buffer			- character string to read
	    * 		 message_length - Message length in characters
	    * 		 nread 	- Number of read characters ( output).
	    * @return number of read characters 
	    */
	   int mnslp_ipfix_import(char *buffer, size_t message_length );
	   
	   /**
	    * Find a return a reference to a template with id: templid
	    * @param templid - Id of the template
	    * @return reference to the template, if not found, it returns NULL.
	    */
	   mnslp_ipfix_template * get_template(uint16_t templid);
	   
	   /**
	    * Return the number of templates included
	    */
	    int get_num_templates(void);
	   
   public:	

	   /**
	    * Create a new class mnslp_ipfix_message
	    * @param By default it sets the version in IPFIX and encode in true, 
	    * 		    the source id is set to 0.
	    */
	   mnslp_ipfix_message();

	   /**
	    * Create a new class mnslp_ipfix_message
	    * @param sourceid  		 - Device source id of the message 
	    * 		 ipfix_version 	 - message version. 
	    *  		 _encode_network - establish whether the message is going to be network encoded or not. 
	    */
	   mnslp_ipfix_message( int sourceid, int ipfix_version, bool _encode_network);
	   
	   /**
	    * Create a new class mnslp_ipfix_message
	    * @param param  		 - buffer containing the message.
	    * 		 message_length	 - length of the message.
	    *  		 _decode_network - establish whether the message is going to be network decoded or not. 
	    */
	   mnslp_ipfix_message(char * param, size_t message_length, bool _decode_network);
	   
	   /**
	    * Destructor.
	    */
	   ~mnslp_ipfix_message(void);
	      
	   /**
	    * creates and add a new data template for the messsage
	    * @param  nfields	- The number of fields to include in the template
	    * @return template id  
	    */
	   uint16_t new_data_template( int nfields );
                              
	   /**
	    * creates and add a new option template for the messsage
	    * @param  nfields	- The number of fields to include in the template
	    */
	   uint16_t  new_option_template( int nfields );
                   
	   /**
	    * Get a field definition from the container field
	    * @param eno 		- Enterprise field number id
	    * 		 type		- field type.
	    */
	   mnslp_ipfix_field get_field_definition( int eno, int type );
                                
	   /**
	    * add a data field to the message within a template
	    * @param templid 	- Pointer to the template
	    * 		 eno		- Enterprise field number id
	    *        type		- Field type.
	    * 	     length		- Field length 
	    */
	   void  add_field( uint16_t templid,
                       uint32_t eno,
                       uint16_t type, 
                       uint16_t length );
                      
	   /**
	    * add a scope field to the message within a template
	    * @param templ 		- Pointer to the template
	    * 		 eno		- Enterprise field number id
	    *        type		- Field type.
	    * 	     length		- Field length 
	    */
	   void  add_scope_field( uint16_t templid,
                             uint32_t eno,
                             uint16_t type, 
                             uint16_t length );
                            
	   /**
	    * delete a template associated to the message.
	    * @param templid 		- template id to delete
	    */
	   void delete_template( uint16_t templid );
	   
	   /**
	    * delete all template associated to the message.
	    */
	   void delete_all_templates();
	   
	   /**
	    * Take a list of field types and create a template containing all those fields.
	    * @param fields 	- list of field types to include
	    * 		 nfields 	- number of fields to include
	    * @return template id  
	    */
	   uint16_t make_template( export_fields_t *fields, 
							   int nfields );
	   
	   /**
	    * Include set data record to the message 
	    * @param templ 	- Pointer to the template associated to the record data set
	    * 		 data 	- record data set to be included. It must have the same amount
	    * 				  of fields as the template.
	    */
	   void include_data( uint16_t templid, 
						  mnslp_ipfix_data_record &data );
	   
	   /**
	    * Export the message into an internal buffer. It only exports data associated
	    * with the template given as parameter.
	    * @param templ 	- Pointer to the template
	    */
	   void output( uint16_t templid );
					
	   /**
	    * Get the internal buffer that was exported
	    */
	   char * get_message(void) const;

	   /**
	    * Get the length of the internal buffer that was exported
	    */
	   int get_offset(void) const;
	   
	   /**
	   *  Equals to operator. 
	   *  It is equal when it has the same amount of templates and 
	   *  data records, and the information inside is equal. The order in
	   * the data records must be same too.
	   */
	   bool operator== (const mnslp_ipfix_message& rhs) const;

	   /**
	    * Assignment operator. 
	    */
	   mnslp_ipfix_message &operator=(const mnslp_ipfix_message &other);

	   	   
};


}

#endif // MNSLP_IPFIX_MESSAGE_H

