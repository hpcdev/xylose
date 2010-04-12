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


#include <xylose/Stack.hpp>

#define BOOST_TEST_MODULE Stack
#include <boost/test/unit_test.hpp>

namespace {

typedef xylose::Stack< int > IntStack;

BOOST_AUTO_TEST_CASE( construct )
{
  IntStack stack;

  BOOST_CHECK_EQUAL( stack.size(), 0u );
  BOOST_CHECK_EQUAL( stack.capacity(), 128u );
}

BOOST_AUTO_TEST_CASE( back )
{
  IntStack stack;
  for ( int i = 0; i < 5; ++i ) {
    stack.push_back( i );
    BOOST_CHECK_EQUAL( stack.back(), i );
  }
}

BOOST_AUTO_TEST_CASE( push_back )
{
  IntStack stack;

  int i;

  for ( i = 0; i < 128; ++i ) {
    stack.push_back( i );
  }

  BOOST_CHECK_EQUAL( stack.size(), 128u );
  BOOST_CHECK_EQUAL( stack.capacity(), 128u );
  
  stack.push_back( i++ );
  BOOST_CHECK_EQUAL( stack.size(), 129u );
  BOOST_CHECK_EQUAL( stack.capacity(), 256u );

  // Cut down on Boost.Test logging by only logging failures.
  BOOST_TEST_MESSAGE( "BEGIN Failure Only Logging" );
  for ( i = 0; i < 129; ++i ) {
    if ( ! ( stack[i] == i ) ) {
      BOOST_CHECK_EQUAL( stack[i], i );
    }
  }
  BOOST_TEST_MESSAGE( "END Failure Only Logging" );
}

BOOST_AUTO_TEST_CASE( reserve )
{
  IntStack stack;

  BOOST_CHECK_EQUAL( stack.capacity(), 128u );

  stack.reserve( 64 );
  BOOST_CHECK_EQUAL( stack.capacity(), 128u );

  stack.reserve( 200 );
  BOOST_CHECK_EQUAL( stack.capacity(), 200u );
  stack.reserve( 128 );
  BOOST_CHECK_EQUAL( stack.capacity(), 200u );
}

} // namespace anon

