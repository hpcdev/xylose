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



#ifndef xylose_nsort_map__2D_h
#define xylose_nsort_map__2D_h

#include <xylose/nsort/map/_1D.h>
#include <xylose/nsort/map/depth_to_for2d.h>

namespace xylose {
  namespace nsort {
    namespace map {
      namespace tag {
        /** Simple tag class that can be used to detect this map. */
        template <unsigned int dir0, unsigned int dir1>
        struct _2D {};
      }

      /** maps x[_dir0],x[_dir1] to 0,1,2,3 */
      template <unsigned int _dir0, unsigned int _dir>
      struct _2D : _1D<_dir0> {
        /* TYPEDEFS */
      private:
        typedef _1D<_dir0> oneD;
      public:
        typedef tag::_2D<_dir0,_dir> tag;

        typedef void super;

        template <unsigned int _depth> 
        struct depth {
          typedef depth_to_for2d<_dir0,_dir,_depth> to;
        };
      
        /** The number of spatial dimensions. */
        static const unsigned int spatial_dimensions = 2u;
        /** The 2nd dimension direction */
        static const unsigned int dir1 = _dir;
      
      
        _2D() {}
        template <class T> _2D(const T & t) {}
      
        inline int getNumberValues() const { return 4; }
        template<class _Particle>
        inline int operator()(const _Particle & p) const {
          return oneD::operator()(p) + 2*(p.x[_dir] >= oneD::pivot[_dir]);
        }
      };
    }
  }
}

#endif // xylose_nsort_map__2D_h

