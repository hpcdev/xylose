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



#ifndef xylose_nsort_map_pivot_h
#define xylose_nsort_map_pivot_h

#include <xylose/Vector.h>
#include <xylose/Dimensions.hpp>
#include <xylose/nsort/map/detail/pivot_depth.h>

namespace xylose {
  namespace nsort {
    namespace map {
      namespace tag {
        /** Simple tag class that can be used to detect this map. */
        template < typename dims >
        struct pivot {};
      }

      /** maps x<pivot to 0 and x>= pivot to 1. */
      template < typename dimensions = Dimensions<0u,1u,2u> >
      struct pivot;



      /** Specialization of pivot map for 1 (one) dimension.  */
      template < unsigned int _dir >
      struct pivot< Dimensions<_dir> > {
        /* TYPEDEFS */
        typedef Dimensions<_dir> dimensions;
        typedef map::tag::pivot< dimensions > tag;
        typedef void super;

        template <unsigned int _depth>
        struct depth {
          typedef detail::pivot_depth< dimensions, _depth > to;
        };

        static const unsigned int number_values = 2u;


        /* MEMBER STORAGE */
        Vector<double,3> point;


        /* MEMBER FUNCTIONS */
        pivot( const Vector<double,3> & point  = 0.0 ) : point( point ) { }

        int getNumberValues() const { return number_values; }

        template < typename Particle >
        int operator()(const Particle & p) const {
          return position(p)[_dir] >= point[_dir];
        }
      };


      /** Specialization of pivot map for 2 (two) dimensions.  */
      template < unsigned int dir0,
                 unsigned int _dir >
      struct pivot< Dimensions<dir0,_dir> > : pivot< Dimensions<dir0> > {
        /* TYPEDEFS */
      private:
        typedef pivot< Dimensions<dir0> > oneD;
      public:
        typedef Dimensions<dir0,_dir> dimensions;
        typedef map::tag::pivot< dimensions > tag;

        template <unsigned int _depth>
        struct depth {
          typedef detail::pivot_depth< dimensions, _depth > to;
        };

        static const unsigned int number_values = 4u;


        /* MEMBER FUNCTIONS */
        pivot( const Vector<double,3> & point = 0.0 ) : oneD( point ) { }

        int getNumberValues() const { return number_values; }

        template < typename Particle >
        int operator()(const Particle & p) const {
          return oneD::operator()(p)
               + 2 * ( position(p)[_dir] >= oneD::point[_dir] );
        }
      };


      /** Specialization of pivot map for 2 (two) dimensions.  */
      template < unsigned int dir0,
                 unsigned int dir1,
                 unsigned int _dir >
      struct pivot< Dimensions<dir0,dir1,_dir> >
        : pivot< Dimensions<dir0,dir1> > {
        /* TYPEDEFS */
      private:
        typedef pivot< Dimensions<dir0,dir1> > twoD;
      public:
        typedef Dimensions<dir0,dir1,_dir> dimensions;
        typedef map::tag::pivot< dimensions > tag;

        template <unsigned int _depth>
        struct depth {
          typedef detail::pivot_depth< dimensions, _depth > to;
        };

        static const unsigned int number_values = 8u;


        /* MEMBER FUNCTIONS */
        pivot( const Vector<double,3> & point = 0.0 ) : twoD( point ) { }

        int getNumberValues() const { return number_values; }

        template < typename Particle >
        int operator()(const Particle & p) const {
          return twoD::operator()(p)
               + 4 * ( position(p)[_dir] >= twoD::point[_dir] );
        }
      };

    }
  }/*namespace nsort */
}/*namespace xylose */

#endif // xylose_nsort_map_pivot_h
