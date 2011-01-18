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


#ifndef xylose_xml_physical_parse_h
#define xylose_xml_physical_parse_h

#include <xylose/xml/Doc.h>
#include <physical/calc/Driver.h>

namespace xylose {
  namespace xml {
    inline void parse_item( runtime::physical::Quantity & out,
                            const Context & x) {
      using runtime::physical::calc::Driver;
      out = Driver::instance().eval( x.text() );
    }
  } /* namespace xylose::xml */
} /* namespace xylose */
#endif // xylose_xml_physical_parse_h
