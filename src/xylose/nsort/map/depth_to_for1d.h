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


#ifndef xylose_nsort_map_depth_to_for1d_h
#define xylose_nsort_map_depth_to_for1d_h

namespace xylose {
  namespace nsort {
    namespace map {

      template < unsigned int _dir0,
                 unsigned int _depth >
      struct depth_to_for1d {
        static const unsigned int dir;
        static const unsigned int rdir;
      };

      template < unsigned int dir0 >
      struct depth_to_for1d<dir0,0u> {
        static const unsigned int dir = dir0;
        static const unsigned int rdir = dir0;
      };

    }
  }
}

#endif // xylose_nsort_map_depth_to_for1d_h
