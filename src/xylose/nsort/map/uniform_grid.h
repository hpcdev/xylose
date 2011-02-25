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



#ifndef xylose_nsort_map_uniform_grid_h
#define xylose_nsort_map_uniform_grid_h

#include <xylose/Dimensions.hpp>
#include <xylose/nsort/map/w_species.h>

#include <cassert>

namespace xylose {
  namespace nsort {
    namespace map {

      /** Grid sorting map for unspecified dimensions that (by default) matches
       * the Uniform grid. */
      template < typename Uniform,
                 typename dims = Dimensions<0u,1u,2u> >
      struct uniform_grid;


      /** Grid sorting map for 1 (one) dimension that (by default) matches the
       * Uniform grid. */
      template < typename Uniform,
                 unsigned int _dir >
      struct uniform_grid<Uniform, Dimensions<_dir> > {
        typedef void super;

        const Uniform & g;

        uniform_grid( const Uniform & g ) : g(g) {
          assert( g.size()[_dir] > 0 );
        }

        uniform_grid( const Uniform * g ) : g(*g) {
          assert( g->size()[_dir] > 0 );
        }

        int getNumberValues() const {
          return g.size()[_dir];
        }

        template <class _Particle>
        int operator()(const _Particle & p) const {
          register int L = static_cast<int>(
                             ( position(p)[_dir] - g.x0()[_dir] ) / g.dx()[_dir]
                           );
          register int max_val = static_cast<int>(g.size()[_dir]) - 1;
          return std::max(0, std::min( max_val, L ) );
        }
      };

      /** Grid sorting map for 2 (two) dimensions that (by default) matches the
       * Uniform grid. */
      template < typename Uniform,
                 unsigned int dir0,
                 unsigned int _dir >
      struct uniform_grid<Uniform, Dimensions<dir0,_dir> >
        : uniform_grid<Uniform, Dimensions<dir0> > {
      private:
        typedef uniform_grid<Uniform, Dimensions<dir0> > oneD;
      public:

        uniform_grid( const Uniform & g ) : oneD( g ) {
          assert( g.size()[_dir] > 0 );
        }

        uniform_grid( const Uniform * g ) : oneD( *g ) {
          assert( g->size()[_dir] > 0 );
        }

        int getNumberValues() const {
          return this->g.size()[dir0] * this->g.size()[_dir];
        }

        template < typename  Particle >
        int operator() (const Particle & p) const {
          register int L = static_cast<int>(
                               ( position(p)[_dir] - this->g.x0()[_dir] )
                             / this->g.dx()[_dir]
                           );
          register int max_val = static_cast<int>(this->g.size()[_dir]) - 1;
          return oneD::operator()(p)
               + (  std::max(0, std::min( max_val, L ) )
                  * this->g.size()[dir0] );
        }
      };

      /** Grid sorting map for 3 (three) dimensions that (by default) matches
       * the Uniform grid. */
      template < typename Uniform,
                 unsigned int dir0,
                 unsigned int dir1,
                 unsigned int _dir >
      struct uniform_grid<Uniform, Dimensions<dir0,dir1,_dir> >
        : uniform_grid<Uniform, Dimensions<dir0,dir1> > {
      private:
        typedef uniform_grid<Uniform, Dimensions<dir0,dir1> > twoD;
      public:

        uniform_grid( const Uniform & g ) : twoD( g ) {
          assert( g.size()[_dir] > 0 );
        }

        uniform_grid( const Uniform * g ) : twoD( *g ) {
          assert( g->size()[_dir] > 0 );
        }

        int getNumberValues() const {
          return this->g.size()[dir0]
               * this->g.size()[dir1]
               * this->g.size()[_dir];
        }

        template < typename  Particle >
        int operator() (const Particle & p) const {
          register int L = static_cast<int>(
                               ( position(p)[_dir] - this->g.x0()[_dir] )
                             / this->g.dx()[_dir]
                           );
          register int max_val = static_cast<int>(this->g.size()[_dir]) - 1;
          return twoD::operator()(p)
               + (  std::max(0, std::min( max_val, L ) )
                  * (this->g.size()[dir0] * this->g.size()[dir1]) );
        }
      };

    }/* namespace xylose::nsort::map */
  }/* namespace xylose::nsort */
}/* namespace xylose */

#endif // xylose_nsort_map_uniform_grid_h
