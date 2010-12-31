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


#include <xylose/nsort/map/pivot.h>

#include <xylose/Vector.h>
#include <xylose/Dimensions.hpp>

#define BOOST_TEST_MODULE  pivot_tests

#include <boost/test/unit_test.hpp>
#include <iostream>


namespace {
  using xylose::Vector;
  using xylose::V3;
  using xylose::Dimensions;

  struct Particle {
    Vector<double, 3> x;
    Particle(const Vector<double,3> & x = 0.0 ) : x(x) {}
  };

  inline const Vector<double,3u> & position( const Particle & p ) {
    return p.x;
  }
  
}


BOOST_AUTO_TEST_CASE( pivot_1D ) {
  using xylose::nsort::map::pivot;
  typedef pivot< Dimensions<0u> > map_t;
  map_t map(V3(0,0,0));

  /* make sure the dimensionality is correct. */
  BOOST_CHECK_EQUAL( static_cast<int>(map_t::dimensions::ndims), 1 );

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

BOOST_AUTO_TEST_CASE( pivot_2D ) {
  using xylose::nsort::map::pivot;
  typedef pivot< Dimensions<0u,1u> > map_t;
  map_t map(V3(0,0,0));

  /* make sure the dimensionality is correct. */
  BOOST_CHECK_EQUAL( static_cast<int>(map_t::dimensions::ndims), 2 );

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

BOOST_AUTO_TEST_CASE( pivot_3D ) {
  using xylose::nsort::map::pivot;
  typedef pivot< Dimensions<2u,1u,0u> > map_t;
  map_t map(V3(0,0,0));

  /* make sure the dimensionality is correct. */
  BOOST_CHECK_EQUAL( static_cast<int>(map_t::dimensions::ndims), 3 );

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

