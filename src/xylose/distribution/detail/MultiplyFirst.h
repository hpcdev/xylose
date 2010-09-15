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

#ifndef xylose_distribution_detail_MultiplyFirst_h
#define xylose_distribution_detail_MultiplyFirst_h

namespace xylose {
  namespace distribution {
    namespace detail {

      struct MultiplyFirst {
        const double factor;
        MultiplyFirst(const double & factor) : factor(factor) { }
        template < typename Pair >
        void operator() ( Pair & p ) const {
          p.first *= factor;
        }
      };

    }/* namespace xylose::distribution::detail */
  }/* namespace xylose::distribution */
}/* namespace xylose */

#endif // xylose_distribution_detail_MultiplyFirst_h
