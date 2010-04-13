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



#ifndef xylose_nsort_map__1D_h
#define xylose_nsort_map__1D_h

#include <xylose/nsort/map/position_base.h>
#include <xylose/nsort/map/depth_to_for1d.h>

namespace xylose {
  namespace nsort {
    namespace map {
      namespace tag {
        /** Simple tag class that can be used to detect this map. */
        template <unsigned int dir>
        struct _1D {};
      }

      /** maps x<pivot to 0 and x>= pivot to 1.
       * This sort-map <i>does</i> dictate that new children are to be created,
       * provided the given set of rules (supplied by info) are followed.  
       * */
      template <unsigned int _dir>
      struct _1D : pivot {
        /* TYPEDEFS */
        typedef tag::_1D<_dir> tag;
        typedef void super;

        template <unsigned int _depth> 
        struct depth {
          typedef depth_to_for1d<_dir,_depth> to;
        };
      
        /** The number of spatial dimensions. */
        static const unsigned int spatial_dimensions = 1u;
        /** The 1st dimension direction */
        static const unsigned int dir0 = _dir;


        _1D() {}
        template <class T> _1D(const T & t) {}
      
        inline int getNumberValues() const { return 2; }
        template<class _Particle>
        inline int operator()(const _Particle & p) const {
          return p.x[_dir] >= pivot[_dir];
        }
      };
    }
  }/*namespace nsort */
}/*namespace xylose */

#endif // xylose_nsort_map__1D_h
