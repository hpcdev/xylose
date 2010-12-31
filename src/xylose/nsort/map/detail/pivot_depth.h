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


#ifndef xylose_nsort_map_detail_pivot_depth_h
#define xylose_nsort_map_detail_pivot_depth_h

#include <xylose/Dimensions.hpp>

namespace xylose {
  namespace nsort {
    namespace map {
      namespace detail {

        template < typename dimensions,
                   unsigned int _depth >
        struct pivot_depth;



        /** One dimensional pivot depth. */
        template < unsigned int dir0 >
        struct pivot_depth< Dimensions<dir0>, 0u > {
          static const unsigned int dir = dir0;
          static const unsigned int rdir = dir0;
        };



        /** Two dimensional pivot depth, first level. */
        template < unsigned int dir0,
                   unsigned int dir1 >
        struct pivot_depth< Dimensions<dir0,dir1>, 0u > {
          static const unsigned int dir = dir0;
          static const unsigned int rdir = dir1;
        };

        /** Two dimensional pivot depth, second level. */
        template < unsigned int dir0,
                   unsigned int dir1 >
        struct pivot_depth< Dimensions<dir0,dir1>, 1u > {
          static const unsigned int dir = dir1;
          static const unsigned int rdir = dir0;
        };



        /** Three dimensional pivot depth, first level. */
        template < unsigned int dir0,
                   unsigned int dir1,
                   unsigned int dir2 >
        struct pivot_depth< Dimensions<dir0,dir1,dir2>, 0u > {
          static const unsigned int dir = dir0;
          static const unsigned int rdir = dir2;
        };

        /** Three dimensional pivot depth, second level. */
        template < unsigned int dir0,
                   unsigned int dir1,
                   unsigned int dir2 >
        struct pivot_depth< Dimensions<dir0,dir1,dir2>, 1u > {
          static const unsigned int dir = dir1;
          static const unsigned int rdir = dir1;
        };

        /** Three dimensional pivot depth, third level. */
        template < unsigned int dir0,
                   unsigned int dir1,
                   unsigned int dir2 >
        struct pivot_depth< Dimensions<dir0,dir1,dir2>, 2u > {
          static const unsigned int dir = dir2;
          static const unsigned int rdir = dir0;
        };

      }
    }
  }
}

#endif // xylose_nsort_map_detail_pivot_depth_h
