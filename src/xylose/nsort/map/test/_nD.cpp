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


#include <xylose/nsort/map/_1D.h>
#include <xylose/nsort/map/_2D.h>
#include <xylose/nsort/map/_3D.h>

#include <xylose/Vector.h>

#define BOOST_TEST_MODULE  map_nD_tests

#include <boost/test/unit_test.hpp>
#include <iostream>


namespace {
  using xylose::Vector;
  using xylose::V3;
  struct Particle {
    typedef std::vector<Particle> list;
  
    Vector<double, 3> x;
    Vector<double, 3> v;
  
    Particle(const Vector<double,3> & x = 0.0,
             const Vector<double,3> & v = 0.0 ) : x(x), v(v) {}
  };
  
  std::ostream & operator<< (std::ostream & out, const Particle & p) {
    return out << "{x: (" << p.x[0] << ", " << p.x[1] << ", " << p.x[2] << "), "
                   "v: (" << p.v[0] << ", " << p.v[1] << ", " << p.v[2] << ") "
                  "} ";
  }
}


BOOST_AUTO_TEST_SUITE( map_nD );

BOOST_AUTO_TEST_CASE( map_1D ) {
  using xylose::nsort::map::_1D;
  using xylose::nsort::map::pivot_ctor;
  typedef pivot_ctor< _1D<0> > map_t;
  map_t map(V3(0,0,0));

  /* make sure the dimensionality is correct. */
  int j = map.spatial_dimensions;
  int i = map_t::spatial_dimensions;
  BOOST_CHECK_EQUAL( i, 1 );
  BOOST_CHECK_EQUAL( j, 1 );

  Particle p;

  /* check what the pivot point gets mapped to... */
  p.x[0] = 0;
  BOOST_CHECK_EQUAL( map(p), 1 );

  /* Check map */
  p.x[0] = -1;
  BOOST_CHECK_EQUAL( map(p), 0 );

  p.x[0] =  1;
  BOOST_CHECK_EQUAL( map(p), 1 );
}

BOOST_AUTO_TEST_CASE( map_2D ) {
  using xylose::nsort::map::_2D;
  using xylose::nsort::map::pivot_ctor;
  typedef pivot_ctor< _2D<0,1> > map_t;
  map_t map(V3(0,0,0));

  /* make sure the dimensionality is correct. */
  int j = map.spatial_dimensions;
  int i = map_t::spatial_dimensions;
  BOOST_CHECK_EQUAL( i, 2 );
  BOOST_CHECK_EQUAL( j, 2 );


  Particle p;

  /* check what the pivot point gets mapped to... */
  p.x[0] = 0;  p.x[1] = 0;
  BOOST_CHECK_EQUAL( map(p), 3 );


  /* Check map */
  p.x[0] = -1; p.x[1] = -1;
  BOOST_CHECK_EQUAL( map(p), 0 );

  p.x[0] =  1; p.x[1] = -1;
  BOOST_CHECK_EQUAL( map(p), 1 );

  p.x[0] = -1; p.x[1] =  1;
  BOOST_CHECK_EQUAL( map(p), 2 );

  p.x[0] =  1; p.x[1] =  1;
  BOOST_CHECK_EQUAL( map(p), 3 );
}

BOOST_AUTO_TEST_CASE( map_3D ) {
  using xylose::nsort::map::_3D;
  using xylose::nsort::map::pivot_ctor;
  typedef pivot_ctor< _3D<2,1,0> > map_t;
  map_t map(V3(0,0,0));

  /* make sure the dimensionality is correct. */
  int j = map.spatial_dimensions;
  int i = map_t::spatial_dimensions;
  BOOST_CHECK_EQUAL( i, 3 );
  BOOST_CHECK_EQUAL( j, 3 );


  Particle p;

  /* check what the pivot point gets mapped to... */
  p.x[0] = 0;  p.x[1] = 0; p.x[2] = 0;
  BOOST_CHECK_EQUAL( map(p), 7 );


  /* Check map:
   * Note that the order here "_3D<2,1,0>" provides for a division by x then y
   * then z as is used in the Octree code.  */
  p.x[0] = -1; p.x[1] = -1; p.x[2] = -1;
  BOOST_CHECK_EQUAL( map(p), 0 );

  p.x[0] = -1; p.x[1] = -1; p.x[2] =  1;
  BOOST_CHECK_EQUAL( map(p), 1 );

  p.x[0] = -1; p.x[1] =  1; p.x[2] = -1;
  BOOST_CHECK_EQUAL( map(p), 2 );

  p.x[0] = -1; p.x[1] =  1; p.x[2] =  1;
  BOOST_CHECK_EQUAL( map(p), 3 );

  p.x[0] =  1; p.x[1] = -1; p.x[2] = -1;
  BOOST_CHECK_EQUAL( map(p), 4 );

  p.x[0] =  1; p.x[1] = -1; p.x[2] =  1;
  BOOST_CHECK_EQUAL( map(p), 5 );

  p.x[0] =  1; p.x[1] =  1; p.x[2] = -1;
  BOOST_CHECK_EQUAL( map(p), 6 );

  p.x[0] =  1; p.x[1] =  1; p.x[2] =  1;
  BOOST_CHECK_EQUAL( map(p), 7 );
}

BOOST_AUTO_TEST_SUITE_END();

