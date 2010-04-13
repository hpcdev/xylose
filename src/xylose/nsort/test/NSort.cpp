/*==============================================================================
 * Public Domain Contributions 2010 United States Government                   *
 * as represented by the U.S. Air Force Research Laboratory.                   *
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


#define BOOST_TEST_MODULE  NSort

#include <xylose/nsort/NSort.h>

#include <boost/test/unit_test.hpp>
#include <iostream>


BOOST_AUTO_TEST_SUITE( NSort );

BOOST_AUTO_TEST_CASE( c_array ) {
  const int len = 10;
  int v[len] = {1, 2, 0, 1, 2, 3, 0, 1, 2, 4};
  int ans[len] = {0, 0, 1, 1, 1, 2, 2, 2, 3, 4};
  xylose::nsort::NSort<> s(len);
  s.sort(static_cast<int*>(v), v+len);

  for (int i = 0; i < len; ++i)
    BOOST_CHECK_EQUAL( v[i], ans[i] );
}

BOOST_AUTO_TEST_CASE( std_vector ) {
  const int len = 10;
  int v[len] = {1, 2, 0, 1, 2, 3, 0, 1, 2, 4};
  int ans[len] = {0, 0, 1, 1, 1, 2, 2, 2, 3, 4};
  std::vector<int> sv(v, v+len);


  xylose::nsort::NSort<> s(len);
  s.sort(sv.begin(), sv.end());

  for (int i = 0; i < len; ++i)
    BOOST_CHECK_EQUAL( sv[i], ans[i] );
}

BOOST_AUTO_TEST_SUITE_END();

