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


#ifndef xylose_xml_vector_parse_h
#define xylose_xml_vector_parse_h

#include <xylose/xml/Doc.h>

#include <vector>

namespace xylose {
  namespace xml {

    template < typename T >
    inline void parse_item( std::vector<T> & out, const Context & x ) {
      /* first make sure the vector is clean */
      out.clear();

      xml::Context::list x_list = x.eval("child::node()");// get all children
      xml::Context::list::iterator i = x_list.begin();
      xml::Context::list::iterator end = x_list.end();

      /* Now we iterate through the children trying parse_item<T> on each. */
      for ( ; i != end; ++i ) {
        const xml::Context & x1 = *i;
        try {
          T val = x1.template parse< T >();
          out.push_back( val );
        } catch ( const xml::error & e ) {
          /* we ignore this error and keep trying other children. */
        }
      }

    }
  } /* namespace xylose::xml */
} /* namespace xylose */
#endif // xylose_xml_vector_parse_h
