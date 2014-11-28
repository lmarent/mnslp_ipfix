/// ----------------------------------------*- mode: C++; -*--
/// @file msnlp_ipfix_exception.h
/// Tools for processing IPFIX messages in NSIS metering.
/// ----------------------------------------------------------
/// $Id: msnlp_ipfix_exception.h 2558 2014-11-26 14:11:00 amarentes $
/// $HeadURL: https://./include/msnlp_ipfix_exception.h $
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

#ifndef MNSLP_IPFIX_EXCEPTION_H
#define MNSLP_IPFIX_EXCEPTION_H

#include <exception>

namespace mnslp_ipfix
{

class mnslp_ipfix_bad_argument: public std::exception
{

public:
	/** Constructor (C strings). Creates bad argument Exception
     *  @param message C-style string error message.
     *                 The string contents are copied upon construction.
     *                 Hence, responsibility for deleting the \c char* lies
     *                 with the caller. 
     */
	explicit mnslp_ipfix_bad_argument(const char* message);
	
	/** Constructor (C++ STL strings).
     *  @param message The error message.
     */
     explicit mnslp_ipfix_bad_argument(const std::string& message);
      
    /** Destructor.
     * Virtual to allow for subclassing.
     */
    virtual ~mnslp_ipfix_bad_argument() throw ();
    
    /** Returns a pointer to the (constant) error description.
     *  @return A pointer to a \c const \c char*. The underlying memory
     *          is in posession of the \c Exception object. Callers \a must
     *          not attempt to free the memory.
     */
    virtual const char* what() const throw ();
    
protected:
    /** Error message.
     */
    std::string msg_;
}; 

inline mnslp_ipfix_bad_argument::mnslp_ipfix_bad_argument(const char* message):
msg_(message)
{
}

inline mnslp_ipfix_bad_argument::mnslp_ipfix_bad_argument(const std::string& message):
msg_(message)
{
}

inline mnslp_ipfix_bad_argument::~mnslp_ipfix_bad_argument() throw ()
{
}

inline const char* mnslp_ipfix_bad_argument::what() const throw ()
{
	 return msg_.c_str();
}
}

#endif // MNSLP_IPFIX_EXCEPTION_H
