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


#define BOOST_TEST_MODULE TypedFactory
#include <boost/test/unit_test.hpp>

#include <xylose/Factory.hpp>
#include <xylose/TypedFactory.hpp>

#include <memory>

namespace {

class Base {
public:
  Base() {}
  virtual ~Base() {}
  virtual int value() { return 0; }
};

typedef xylose::Factory< Base, int > BaseFactory;

class Derived : public Base {
public:
  Derived( const int& ) {}
  virtual ~Derived() {}
  virtual int value() { return 1; }
};

typedef xylose::TypedFactory< Base, Derived, int > DerivedFactory;

BOOST_AUTO_TEST_CASE( create ) {
  std::auto_ptr< BaseFactory > factory( new DerivedFactory );
  std::auto_ptr< Base > object( factory->create( 0 ) );
  BOOST_CHECK_EQUAL( 1, object->value() );
}

} // namespace

