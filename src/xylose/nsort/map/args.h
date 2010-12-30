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




#ifndef xylose_nsort_map_args_h
#define xylose_nsort_map_args_h

#include <utility>

namespace xylose {
  namespace nsort {
    namespace map {

      template < typename T1,
                 typename T2,
                 typename T3,
                 typename T4,
                 typename T5,
                 typename T6,
                 typename T7,
                 typename T8,
                 typename T9 >
      inline std::pair<T1,
               std::pair<T2,
                 std::pair<T3,
                   std::pair<T4,
                     std::pair<T5,
                       std::pair<T6,
                         std::pair<T7,
                           std::pair<T8,T9>
             > > > > > > >
      make_arg( const T1 & t1,
                const T2 & t2,
                const T3 & t3,
                const T4 & t4,
                const T5 & t5,
                const T6 & t6,
                const T7 & t7,
                const T8 & t8,
                const T9 & t9 ) {
        return std::make_pair(t1,
                 std::make_pair(t2,
                   std::make_pair(t3,
                     std::make_pair(t4,
                       std::make_pair(t5,
                         std::make_pair(t6,
                           std::make_pair(t7,
                             std::make_pair(t8,t9)
               )))))));
      }

      template < typename T1,
                 typename T2,
                 typename T3,
                 typename T4,
                 typename T5,
                 typename T6,
                 typename T7,
                 typename T8 >
      inline std::pair<T1,
               std::pair<T2,
                 std::pair<T3,
                   std::pair<T4,
                     std::pair<T5,
                       std::pair<T6,
                         std::pair<T7,T8>
             > > > > > >
      make_arg( const T1 & t1,
                const T2 & t2,
                const T3 & t3,
                const T4 & t4,
                const T5 & t5,
                const T6 & t6,
                const T7 & t7,
                const T8 & t8 ) {
        return std::make_pair(t1,
                 std::make_pair(t2,
                   std::make_pair(t3,
                     std::make_pair(t4,
                       std::make_pair(t5,
                         std::make_pair(t6,
                           std::make_pair(t7,t8)
               ))))));
      }

      template < typename T1,
                 typename T2,
                 typename T3,
                 typename T4,
                 typename T5,
                 typename T6,
                 typename T7 >
      inline std::pair<T1,
               std::pair<T2,
                 std::pair<T3,
                   std::pair<T4,
                     std::pair<T5,
                       std::pair<T6,T7>
             > > > > >
      make_arg( const T1 & t1,
                const T2 & t2,
                const T3 & t3,
                const T4 & t4,
                const T5 & t5,
                const T6 & t6,
                const T7 & t7 ) {
        return std::make_pair(t1,
                 std::make_pair(t2,
                   std::make_pair(t3,
                     std::make_pair(t4,
                       std::make_pair(t5,
                         std::make_pair(t6,t7)
               )))));
      }

      template < typename T1,
                 typename T2,
                 typename T3,
                 typename T4,
                 typename T5,
                 typename T6 >
      inline std::pair<T1,
               std::pair<T2,
                 std::pair<T3,
                   std::pair<T4,
                     std::pair<T5,T6>
             > > > >
      make_arg( const T1 & t1,
                const T2 & t2,
                const T3 & t3,
                const T4 & t4,
                const T5 & t5,
                const T6 & t6 ) {
        return std::make_pair(t1,
                 std::make_pair(t2,
                   std::make_pair(t3,
                     std::make_pair(t4,
                       std::make_pair(t5,t6)
               ))));
      }

      template < typename T1,
                 typename T2,
                 typename T3,
                 typename T4,
                 typename T5 >
      inline std::pair<T1,
               std::pair<T2,
                 std::pair<T3,
                   std::pair<T4,T5>
             > > >
      make_arg( const T1 & t1,
                const T2 & t2,
                const T3 & t3,
                const T4 & t4,
                const T5 & t5 ) {
        return std::make_pair(t1,
                 std::make_pair(t2,
                   std::make_pair(t3,
                     std::make_pair(t4,t5)
               )));
      }

      template < typename T1,
                 typename T2,
                 typename T3,
                 typename T4 >
      inline std::pair<T1,
               std::pair<T2,
                 std::pair<T3,T4>
             > >
      make_arg( const T1 & t1,
                const T2 & t2,
                const T3 & t3,
                const T4 & t4 ) {
        return std::make_pair( t1, std::make_pair(t2, std::make_pair(t3,t4) ) );
      }

      template < typename T1,
                 typename T2,
                 typename T3 >
      inline std::pair<T1, std::pair<T2,T3> >
      make_arg( const T1 & t1, const T2 & t2, const T3 & t3 ) {
        return std::make_pair( t1, std::make_pair(t2,t3) );
      }

      template < typename T1,
                 typename T2 >
      inline std::pair<T1, T2>
      make_arg( const T1 & t1, const T2 & t2 ) {
        return std::make_pair( t1, t2 );
      }

    }/*namespace map */
  }/*namespace nsort */
}/*namespace xylose */


#endif // xylose_nsort_map_args_h

