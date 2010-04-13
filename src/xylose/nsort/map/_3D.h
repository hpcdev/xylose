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



#ifndef xylose_nsort_map__3D_h
#define xylose_nsort_map__3D_h

#include <xylose/nsort/map/_2D.h>
#include <xylose/nsort/map/depth_to_for3d.h>

namespace xylose {
  namespace nsort {
    namespace map {
      namespace tag {
        /** Simple tag class that can be used to detect this map. */
        template <unsigned int dir0, unsigned int dir1, unsigned int dir2>
        struct _3D {};
      }

      /** maps x[_dir0],x[_dir1],x[_dir2] to 0,1,2,3,4,5,6,7 */
      template <unsigned int _dir0, unsigned int _dir1, unsigned int _dir>
      struct _3D : _2D<_dir0,_dir1> {
        /* TYPEDEFS */
      private:
        typedef _2D<_dir0,_dir1> twoD;
      public:
        typedef tag::_3D<_dir0,_dir1,_dir> tag;
        typedef void super;

        template <unsigned int _depth> 
        struct depth {
          typedef depth_to_for3d<_dir0,_dir1,_dir,_depth> to;
        };
      
        /** The number of spatial dimensions. */
        static const unsigned int spatial_dimensions = 3u;
        /** The 3rd dimension direction */
        static const unsigned int dir2 = _dir;

      
        _3D() {}
        template <class T> _3D(const T & t) {}
      
        inline int getNumberValues() const { return 8; }
        template<class _Particle>
        inline int operator()(const _Particle & p) const {
          return twoD::operator()(p) + 4*(p.x[_dir] >= twoD::pivot[_dir]);
        }
      };
    }
  }
}

#endif // xylose_nsort_map__3D_h

