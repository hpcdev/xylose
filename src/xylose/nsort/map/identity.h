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




#ifndef xylose_nsort_map_identity_h
#define xylose_nsort_map_identity_h

namespace xylose {
  namespace nsort {
    namespace map {
      namespace tag {
        /** Simple tag class that can be used to detect this map. */
        struct identity {};
      }

     /** Identity nsort_map wrapper.  Useful as a default parameter in a templated
      * nsort::map wrap.
      * */
      template <class T>
      struct identity : T {
        typedef tag::identity tag;
        typedef T super;

        identity() {}
        template <class TT> identity(const TT & tt) : T(tt) {}
      };
    }/*namespace map */
  }/*namespace nsort */
}/* namespace xylose */


#endif // xylose_nsort_map_identity_h

