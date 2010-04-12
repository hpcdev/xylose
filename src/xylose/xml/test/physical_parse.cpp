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


#define BOOST_TEST_MODULE  XML_physical_parse

#include <xylose/xml/Doc.h>
#include <xylose/xml/physical_parse.h>
#include <xylose/strutil.h>

#include <physical/calc/Driver.h>

#include <boost/test/unit_test.hpp>

#include <ostream>
#include <sstream>

#ifndef XML_FILENAME
#  error The filename was supposed to already be defined on the command line
#endif

namespace {
  namespace xml = xylose::xml;
  using runtime::physical::Quantity;
  using xylose::to_string;
}

BOOST_AUTO_TEST_SUITE( XML_physical_parse_tests );//{

  BOOST_AUTO_TEST_CASE( parse ) {
    using runtime::physical::calc::Driver;
    Driver & calc = Driver::instance();
    /* clear the old symbols out */
    calc.symbols.clear();
    calc.addMathLib();
    calc.addPhysicalUnits();

    /* do imports to make things easier. */
    calc.exec( "from physical::constant import *;" );
    calc.exec( "from physical::unit import *;" );
    calc.exec( "from physical import 'unit::pi';" );

    xml::Doc doc(XML_FILENAME);

    xml::Context x = doc.find("//some-value");

    BOOST_CHECK_EQUAL( to_string( x.parse<Quantity>() ), "<10 m>" );
  }

BOOST_AUTO_TEST_SUITE_END();//}

