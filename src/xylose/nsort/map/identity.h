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

#include <utility>

namespace xylose {
  namespace nsort {
    namespace map {
      namespace tag {
        /** Simple tag class that can be used to detect this map. */
        struct identity {};
      }

     /** Identity nsort::map wrapper.  Useful as a default parameter in a templated
      * nsort::map wrap.
      *
      * @tparam T
      *   The underlying nsort map to inherit.
      *
      * @tparam consume_argument
      *   A boolean parameter to specify whether this identity class should
      *   expect to consume constructor arguments or not.  <br>
      *   [Default false]
      * */
      template < typename T, bool consume_argument = false >
      struct identity;

      /** Identity nsort::map wrapper that does NOT consume constructor
       * arguments. */
      template < typename T >
      struct identity<T,false> : T {
        typedef map::tag::identity tag;
        typedef T super;

        identity() {}
        template < typename TT > identity(const TT & tt) : T(tt) { }
      };

      /** Identity nsort::map wrapper that does consume one constructor
       * argument. */
      template < typename T >
      struct identity<T,true> : T {
        typedef map::tag::identity tag;
        typedef T super;

        identity() {}

        template < typename T1,
                   typename T2 >
        identity( const std::pair<T1,T2> & args )
          : T(args.second) { }
      };
    }/*namespace map */
  }/*namespace nsort */
}/* namespace xylose */


#endif // xylose_nsort_map_identity_h

