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

#ifndef xylose_distribution_detail_pair_h
#define xylose_distribution_detail_pair_h

#include <istream>
#include <ostream>
#include <utility>

namespace xylose {
  namespace distribution {
    namespace detail {

      /** This is a proxy std::pair class that allows me to define << and >>
       * outside of the std namespace.  Doing this allows ADL to find these
       * operators defined in this local name space. */
      template < typename T0, typename T1 >
      struct pair : std::pair<T0,T1> { };

      std::istream & operator>> ( std::istream & in, pair<double,double> & p ) {
        return in >> p.first >> p.second;
      }

      std::ostream & operator<< ( std::ostream & out,
                                const pair<double,double> & p ) {
        return out << p.first << ' ' << p.second;
      }

    }/* namespace xylose::distribution::detail */
  }/* namespace xylose::distribution */
}/* namespace xylose */

#endif // xylose_distribution_detail_pair_h
