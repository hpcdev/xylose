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


#ifndef xylose_nsort_map_get_dims_h
#define xylose_nsort_map_get_dims_h

#include <boost/static_assert.hpp>

namespace xylose {
  namespace nsort {
    namespace map {

      template < typename _Dimensions, unsigned int ndims = _Dimensions::ndims >
      struct get_dims {
        BOOST_STATIC_ASSERT(sizeof(_Dimensions) == 0);
      };

      template < typename _Dimensions >
      struct get_dims<_Dimensions,1u> {
        typedef _1D< _Dimensions::dir0 > type;
      };

      template < typename _Dimensions >
      struct get_dims<_Dimensions,2u> {
        typedef _2D< _Dimensions::dir0, _Dimensions::dir1 > type;
      };

      template < typename _Dimensions >
      struct get_dims<_Dimensions,3u> {
        typedef _3D< _Dimensions::dir0, _Dimensions::dir1, _Dimensions::dir2 > type;
      };
    }
  }
}

#endif // xylose_nsort_map_get_dims_h
