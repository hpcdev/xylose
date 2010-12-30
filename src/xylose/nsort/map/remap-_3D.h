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



#ifndef xylose_nsort_map_remap__3D_h
#define xylose_nsort_map_remap__3D_h

#include <xylose/nsort/map/_3D.h>
#include <xylose/nsort/map/remap_base.h>

namespace xylose {
  namespace nsort {
    namespace map {
      namespace tag {
        struct remap_3D {};
      }

      /** Specialization of remap for _3D. */
      template < unsigned int _dir0, unsigned int _dir1, unsigned int _dir2 >
      struct remap< _3D<_dir0,_dir1,_dir2> >
        : remap_base< _3D<_dir0,_dir1,_dir2>, 8u > {
        typedef map::tag::remap_3D tag;
        typedef remap_base< _3D<_dir0,_dir1,_dir2>, 8u > super;
        /** Default constructor required by wrapping components. */
        remap() : super() {}
        /** Single arg constructor required by wrapping components.*/
        template <class TT> remap(const TT & tt) : super(tt) {}
      };
    }
  }
}

#endif // xylose_nsort_map_remap__3D_h

