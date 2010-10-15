/*==============================================================================
 * Public Domain Contributions 2010 United States Government                   *
 * as represented by the U.S. Air Force Research Laboratory.                   *
 * Portions copyright Copyright (C) 2010 Stellar Science                       *
 *                                                                             *
 * This file is part of xylose                                                 *
 *                                                                             *
 * This program is free software: you can redistribute it and/or modify it     *
 * under the terms of the GNU Lesser General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or (at your  *
 * option) any later version.                                                  *
 *                                                                             *
 * This program is distributed in the hope that it will be useful, but WITHOUT *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
 * License for more details.                                                   *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                             *
 -----------------------------------------------------------------------------*/


#include <xylose/strutil.h>

#define BOOST_TEST_MODULE strutil
#include <boost/test/unit_test.hpp>

#include <ostream>
#include <istream>

namespace {

  struct A {
    int value;
    A() : value (42) { }
  };

  std::ostream & operator<< ( std::ostream & out, const A & a ) {
    return out << a.value;
  }

  std::istream & operator>> ( std::istream & in, A & a ) {
    return in >> a.value;
  }

  bool operator== ( const A & lhs, const A & rhs ) {
    return lhs.value == rhs.value;
  }

  BOOST_AUTO_TEST_CASE( to_string_tests ) {
    using xylose::to_string;
    BOOST_CHECK_EQUAL( to_string(1),    "1"   );
    BOOST_CHECK_EQUAL( to_string(1.1f), "1.1" );
    BOOST_CHECK_EQUAL( to_string(1.2),  "1.2" );
    BOOST_CHECK_EQUAL( to_string('a'),  "a" );
    BOOST_CHECK_EQUAL( to_string(A()),  "42" );
    BOOST_CHECK_EQUAL( to_string("hi there"),  "hi there" );
  }

  BOOST_AUTO_TEST_CASE( from_string_tests ) {
    using xylose::from_string;
    BOOST_CHECK_EQUAL( from_string< int >      ("1"),    1    );
    BOOST_CHECK_EQUAL( from_string< float >    ("1.1"),  1.1f );
    BOOST_CHECK_EQUAL( from_string< double >   ("1.2"),  1.2  );
    BOOST_CHECK_EQUAL( from_string< char >     ("a"),    'a'  );
    BOOST_CHECK_EQUAL( from_string< A >        ("42"),    A() );
    BOOST_CHECK_EQUAL( from_string<std::string>("hi there"),  "hi there" );

    { int         ans; BOOST_CHECK_EQUAL( from_string("1",        ans), 1    ); }
    { float       ans; BOOST_CHECK_EQUAL( from_string("1.1",      ans), 1.1f ); }
    { double      ans; BOOST_CHECK_EQUAL( from_string("1.2",      ans), 1.2  ); }
    { char        ans; BOOST_CHECK_EQUAL( from_string("a",        ans), 'a'  ); }
    { A           ans; BOOST_CHECK_EQUAL( from_string("42",       ans), A()  ); }
    { std::string ans; BOOST_CHECK_EQUAL( from_string("hi there", ans), "hi there" ); }
  }

  BOOST_AUTO_TEST_CASE( case_changes ) {
    using xylose::tolower;
    using xylose::toupper;
    BOOST_CHECK_EQUAL( tolower("AAA"),    "aaa" );
    BOOST_CHECK_EQUAL( tolower("AzA"),    "aza" );
    BOOST_CHECK_EQUAL( tolower("zzA"),    "zza" );
    BOOST_CHECK_EQUAL( tolower("zzA1"),   "zza1" );
    BOOST_CHECK_EQUAL( tolower("zzA1A"),  "zza1a" );
    BOOST_CHECK_EQUAL( tolower("zzA1Az"), "zza1az" );

    BOOST_CHECK_EQUAL( toupper("aaa"),    "AAA" );
    BOOST_CHECK_EQUAL( toupper("aZa"),    "AZA" );
    BOOST_CHECK_EQUAL( toupper("ZZa"),    "ZZA" );
    BOOST_CHECK_EQUAL( toupper("ZZa1"),   "ZZA1" );
    BOOST_CHECK_EQUAL( toupper("ZZa1a"),  "ZZA1A" );
    BOOST_CHECK_EQUAL( toupper("ZZa1aZ"), "ZZA1AZ" );
  }

  BOOST_AUTO_TEST_CASE( strip_tests ) {
    using xylose::lstrip;
    using xylose::rstrip;
    using xylose::strip;
    BOOST_CHECK_EQUAL( lstrip("\na"       ), "a" );
    BOOST_CHECK_EQUAL( rstrip(  "a\n \t\r"), "a" );
    BOOST_CHECK_EQUAL(  strip("\na\n \t\r"), "a" );
  }

} // namespace anon

