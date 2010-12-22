/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2005-2008 Spencer E. Olson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *  
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *                                                                                 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.                                                                           .
 * 
 * Questions? Contact Spencer Olson (olsonse@umich.edu) 
 */

/** \file
 * Some utility functions to help convert from different types to/from std::string.
 *
 * Copyright 2005-2008 Spencer Olson.
 */

#ifndef xylose_strutil_h
#define xylose_strutil_h

#include <string>
#include <sstream>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <locale>

namespace xylose {

  struct string_error : std::runtime_error {
    typedef std::runtime_error super;
    string_error(const std::string & s) : super(s) {}
  };

  /** Convert any item to a string--requires that operator<<(T) exists. */
  template < typename T >
  inline std::string to_string( const T & Value ) {
    std::stringstream streamOut;
    streamOut << Value;
    return streamOut.str( );
  }

  //  specialization for string.
  template<>
  inline std::string to_string( const std::string & Value ) {
    return Value;
  }

  /** Convert any item from a string--requires that operator>>(T) exists. */
  template < typename T >
  inline T & from_string( const std::string & ToConvert, T & ReturnValue ) {
    std::stringstream streamIn(ToConvert);
    streamIn >> ReturnValue;
    if ( !streamIn )
      throw string_error("could not convert from string");
    return ReturnValue;
  }

  // specialization for string
  template<>
  inline std::string & from_string( const std::string & ToConvert,
                                          std::string & ReturnValue ) {
    return ReturnValue = ToConvert;
  }

  /** Convert any item from a string--requires that operator>>(T) exists. */
  template < typename T >
  inline T from_string( const std::string & ToConvert ) {
    T ReturnValue = T( );
    return from_string( ToConvert, ReturnValue );
  }

  // specialization for string
  template<>
  inline std::string from_string<std::string>( const std::string & ToConvert ) {
    return ToConvert;
  }

  /** Transform a string to lower case. */
  inline std::string tolower( const std::string& str,
                              const std::locale& loc = std::locale() ) 
  {
    std::string retval = str;
    std::use_facet< std::ctype<char> >(loc)
      .tolower( &*retval.begin(), &*retval.end() );
    return retval;
  }

  /** Transform a string to upper case. */
  inline std::string toupper( const std::string& str,
                              const std::locale& loc = std::locale() ) {
    std::string retval = str;
    std::use_facet< std::ctype<char> >(loc)
      .toupper( &*retval.begin(), &*retval.end() );
    return retval;
  }

  /** Strip leading whitespace from a string to create a new string. */
  inline std::string lstrip( const std::string & s,
                             const std::locale & loc = std::locale() ) {
    std::string::size_type pos = 0u;
    const std::string::size_type sz = s.size();
    while ( pos < sz && std::isspace(s[pos], loc) )
      ++pos;

    if ( pos >= sz )
      return "";
    else
      return s.substr( pos );
  }

  /** Strip trailing whitespace from a string to create a new string. */
  inline std::string rstrip( const std::string & s,
                             const std::locale & loc = std::locale() ) {
    std::string::size_type epos = s.size();
    while ( epos > 0u && std::isspace(s[epos - 1u], loc) )
      --epos;

    if ( epos == 0u )
      return "";
    else
      return s.substr( 0u, epos );
  }

  /** Strip leading/trailing whitespace from a string to create a new string. */
  inline std::string strip( const std::string & s,
                            const std::locale & loc = std::locale() ) {
    return lstrip( rstrip( s, loc ), loc );
  }

}/* namespace xylose */

#endif // xylose_strutil_h
