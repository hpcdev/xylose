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


#ifndef xylose_nsort_tweak_Null_h
#define xylose_nsort_tweak_Null_h

namespace xylose {
  namespace nsort {
    namespace tweak {
      /** Default NSort tweaker does nothing. */
      struct Null {
        template < typename Map >
        inline void tweakNSort( Map & map,
                                const int * const bin,
                                const int & n_values ) const {}
      };
    }/* namespace tweak */
  }/* namespace nsort */
}/* namespace xylose */

#endif // xylose_nsort_tweak_Null_h
