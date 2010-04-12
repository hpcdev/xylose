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


#include <xylose/Singleton.hpp>

#define BOOST_TEST_MODULE Singleton
#include <boost/test/unit_test.hpp>

// run this test through valgrind to ensure we are not leaking the instance!

namespace {

class Test : public xylose::Singleton< Test > {
public:
  int x;
private:
  Test() : x( 42 ) {}
  ~Test() {}
  
  friend class xylose::Singleton< Test >;
};

BOOST_AUTO_TEST_CASE( constructor ) {
  BOOST_CHECK_EQUAL( Test::instance()->x, 42 );
}

BOOST_AUTO_TEST_CASE( instance ) {
  BOOST_CHECK_EQUAL( Test::instance(), Test::instance() );
}

} // namespace anon

