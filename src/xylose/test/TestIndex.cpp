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


#include <xylose/Index.hpp>

#define BOOST_TEST_MODULE Index 
#include <boost/test/unit_test.hpp>

namespace {

BOOST_AUTO_TEST_CASE( increment )
{
  xylose::Index< 2, true > test(0);
  BOOST_CHECK_EQUAL( test.mSegment, 0u );
  BOOST_CHECK_EQUAL( test.mPosition, 0u );

  ++test;
  BOOST_CHECK_EQUAL( test.mSegment, 0u );
  BOOST_CHECK_EQUAL( test.mPosition, 1u );

  ++test;
  BOOST_CHECK_EQUAL( test.mSegment, 1u );
  BOOST_CHECK_EQUAL( test.mPosition, 0u );
}

} // namespace anon

