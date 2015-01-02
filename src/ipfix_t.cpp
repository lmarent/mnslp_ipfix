/// ----------------------------------------*- mode: C; -*--
/// @file ipfix_t.cpp
/// Tools for processing IPFIX messages in NSIS metering.
/// ----------------------------------------------------------
/// $Id: ipfix_t.cpp 2558 2014-12-31 10:11:00 amarentes $
/// $HeadURL: https://./include/ipfix_t.cpp $
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


#include "ipfix_t.h"
#include <iostream>

namespace mnslp_ipfix
{

ipfix_t::ipfix_t():
sourceid(0), nrecords(0), offset(0), buffer_lenght(0), 
buffer(NULL), cs_tid(0), cs_bytes(0), cs_header(NULL), count(0), 
sysuptime(0), unixtime(0), length(0), exporttime(0), cs_offset(0)
{

   buffer = new char[IPFIX_DEFAULT_BUFLEN];
   buffer_lenght = IPFIX_DEFAULT_BUFLEN;

}



/**
* Destructor of the class
*/
ipfix_t::~ipfix_t()
{
	if (buffer != NULL)
		delete buffer;
}

ipfix_t &
ipfix_t::operator=(const ipfix_t & rhs)
{

	sourceid = rhs.sourceid;
	version = rhs.version;
	templates = rhs.templates;
	nrecords = rhs.nrecords;
	offset = rhs.offset;
	seqno = rhs.seqno;
	if (buffer_lenght > 0){
		buffer = new char[buffer_lenght];
		memcpy(buffer,rhs.buffer,rhs.offset);
	}
	
	cs_tid = rhs.cs_tid;
	cs_bytes = rhs.cs_bytes;
	cs_offset = rhs.cs_offset;
	// put the pointer the number of positions given by cs_header.
	
	if (rhs.cs_header != NULL)
		cs_header = (uint8_t*)(buffer) + cs_offset;
	else
		cs_header = NULL;

	count = rhs.count;
	sysuptime = rhs.sysuptime;
	unixtime = rhs.unixtime;
	
	length = rhs.length;
	exporttime = rhs.exporttime;
	
	return *this;
}


bool 
ipfix_t::operator== (const ipfix_t& rhs)
{	
	if ((sourceid != rhs.sourceid) || 
	   (version != rhs.version) ||
	   (templates != rhs.templates) ||
	   (nrecords != rhs.nrecords) ||
	   (offset != rhs.offset) ||
	   (buffer_lenght != rhs.buffer_lenght) ||
	   (seqno != rhs.seqno)  
	   )
		return false;
		
	if (version == IPFIX_VERSION_NF9)
		if ( (rhs.count != count ) ||
			 (rhs.sysuptime != sysuptime ) ||
			 (rhs.unixtime != unixtime ) )
			return false;
			
	if (version == IPFIX_VERSION)
		if ( (rhs.length != length) ||
			 (rhs.exporttime != exporttime) )
			return false;
		
	return true;
	    
}

bool 
ipfix_t::operator!= (const ipfix_t& rhs)
{
	return !(operator==(rhs));
}


void
ipfix_t::copy_raw_message(char * msg, size_t _offset)
{
	if (offset<= buffer_lenght){
		// release the memory assigned to the buffer
		delete buffer;
		buffer = new char[buffer_lenght];
		memcpy(buffer,msg,_offset);
		offset = _offset;
	}
	else
		throw mnslp_ipfix_bad_argument("buffer length not enough for the raw copy");
}

} // namespace mnslp_ipfix

