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


#include <xylose/nsort/map/uniform_grid.h>

#include <xylose/Vector.h>
#include <xylose/Dimensions.hpp>

#define BOOST_TEST_MODULE  uniform_grid

#include <boost/test/unit_test.hpp>
#include <iostream>


namespace {
  using xylose::Vector;
  using xylose::V3;
  using xylose::Dimensions;

  template < typename dims >
  struct UniformGrid;
  
  template < unsigned int dir0 >
  struct UniformGrid< Dimensions<dir0> > {
    Vector<double,3u> x0;
    Vector<double,3u> dx;

    UniformGrid( ) : x0( 0.0 ), dx (1.0) { }

    Vector<unsigned int,3U> size() const {
      Vector<unsigned int, 3u> retval(0u);
      retval[dir0] = 10u;
      return retval;
    }
  };

  template < unsigned int dir0,
             unsigned int dir1 >
  struct UniformGrid< Dimensions<dir0,dir1> >
    : UniformGrid< Dimensions<dir0> > {
    Vector<unsigned int,3U> size() const {
      Vector<unsigned int, 3u> retval(0u);
      retval[dir0] = 10u;
      retval[dir1] = 5u;
      return retval;
    }
  };

  template < unsigned int dir0,
             unsigned int dir1,
             unsigned int dir2 >
  struct UniformGrid< Dimensions<dir0,dir1,dir2> >
    : UniformGrid< Dimensions<dir0, dir1> > {
    Vector<unsigned int,3U> size() const {
      Vector<unsigned int, 3u> retval(0u);
      retval[dir0] = 10u;
      retval[dir1] = 5u;
      retval[dir2] = 2u;
      return retval;
    }
  };


  struct Particle {
    Vector<double, 3> x;
    Particle(const Vector<double,3> & x = 0.0 ) : x(x) {}
  };

  inline const Vector<double,3u> & position( const Particle & p ) {
    return p.x;
  }
  
}


BOOST_AUTO_TEST_CASE( map_uniform_grid_1D ) {
  using xylose::nsort::map::uniform_grid;

  {
    typedef Dimensions<0u> dims;
    UniformGrid< dims > grid;
    uniform_grid< UniformGrid<dims>, dims > map(grid);

    BOOST_CHECK_EQUAL( map( Particle( 0.0 ) ), 0 );

    grid.x0[0u] = -5.0;
    BOOST_CHECK_EQUAL( map( Particle( -6.0  ) ), 0 );
    BOOST_CHECK_EQUAL( map( Particle( -10.0 ) ), 0 );
    BOOST_CHECK_EQUAL( map( Particle( -5.0  ) ), 0 );
    BOOST_CHECK_EQUAL( map( Particle( -4.0  ) ), 1 );
    BOOST_CHECK_EQUAL( map( Particle( -3.0  ) ), 2 );
    BOOST_CHECK_EQUAL( map( Particle( -2.0  ) ), 3 );
    BOOST_CHECK_EQUAL( map( Particle( -1.0  ) ), 4 );
    BOOST_CHECK_EQUAL( map( Particle(  0.0  ) ), 5 );
    BOOST_CHECK_EQUAL( map( Particle(  1.0  ) ), 6 );
    BOOST_CHECK_EQUAL( map( Particle(  2.0  ) ), 7 );
    BOOST_CHECK_EQUAL( map( Particle(  3.0  ) ), 8 );
    BOOST_CHECK_EQUAL( map( Particle(  4.0  ) ), 9 );
    BOOST_CHECK_EQUAL( map( Particle(  5.0  ) ), 9 );
    BOOST_CHECK_EQUAL( map( Particle(  6.0  ) ), 9 );
    BOOST_CHECK_EQUAL( map( Particle(  10.0 ) ), 9 );

    grid.x0[0u] = -10.0;
    BOOST_CHECK_EQUAL( map( Particle( 0.0 ) ), 9 );
  }
}

BOOST_AUTO_TEST_CASE( map_uniform_grid_2D ) {
  using xylose::nsort::map::uniform_grid;

  {
    typedef Dimensions<0u,1u> dims;
    UniformGrid< dims > grid;
    uniform_grid< UniformGrid<dims>, dims > map(grid);

    BOOST_CHECK_EQUAL( map( Particle( 0.0 ) ), 0 );

    grid.x0[0u] = -5.0;
    BOOST_CHECK_EQUAL( map( Particle( V3(-6.0 , 0., 0. ) ) ), 0 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-10.0, 0., 0. ) ) ), 0 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-5.0 , 0., 0. ) ) ), 0 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-4.0 , 0., 0. ) ) ), 1 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-3.0 , 0., 0. ) ) ), 2 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-2.0 , 0., 0. ) ) ), 3 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-1.0 , 0., 0. ) ) ), 4 );
    BOOST_CHECK_EQUAL( map( Particle( V3( 0.0 , 0., 0. ) ) ), 5 );
    BOOST_CHECK_EQUAL( map( Particle( V3( 1.0 , 0., 0. ) ) ), 6 );
    BOOST_CHECK_EQUAL( map( Particle( V3( 2.0 , 0., 0. ) ) ), 7 );
    BOOST_CHECK_EQUAL( map( Particle( V3( 3.0 , 0., 0. ) ) ), 8 );
    BOOST_CHECK_EQUAL( map( Particle( V3( 4.0 , 0., 0. ) ) ), 9 );
    BOOST_CHECK_EQUAL( map( Particle( V3( 5.0 , 0., 0. ) ) ), 9 );
    BOOST_CHECK_EQUAL( map( Particle( V3( 6.0 , 0., 0. ) ) ), 9 );
    BOOST_CHECK_EQUAL( map( Particle( V3( 10.0, 0., 0. ) ) ), 9 );

    grid.x0[1u] = -2.5;
    BOOST_CHECK_EQUAL( map( Particle( V3(-5.0, -3.5, 0. ) ) ), 0 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-5.0, -2.5, 0. ) ) ), 0 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-5.0, -1.5, 0. ) ) ), 10 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-5.0, -0.5, 0. ) ) ), 20 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-5.0,  1.5, 0. ) ) ), 40 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-5.0,  2.5, 0. ) ) ), 40 );

    grid.x0[0u] = -10.0;
    BOOST_CHECK_EQUAL( map( Particle( 0.0 ) ), 29 );
  }
}

BOOST_AUTO_TEST_CASE( map_uniform_grid_3D ) {
  using xylose::nsort::map::uniform_grid;

  {
    typedef Dimensions<0u,1u,2u> dims;
    UniformGrid< dims > grid;
    uniform_grid< UniformGrid<dims>, dims > map(grid);

    BOOST_CHECK_EQUAL( map( Particle( 0.0 ) ), 0 );

    grid.x0[0u] = -5.0;
    BOOST_CHECK_EQUAL( map( Particle( V3(-6.0 , 0., 0. ) ) ), 0 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-10.0, 0., 0. ) ) ), 0 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-5.0 , 0., 0. ) ) ), 0 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-4.0 , 0., 0. ) ) ), 1 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-3.0 , 0., 0. ) ) ), 2 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-2.0 , 0., 0. ) ) ), 3 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-1.0 , 0., 0. ) ) ), 4 );
    BOOST_CHECK_EQUAL( map( Particle( V3( 0.0 , 0., 0. ) ) ), 5 );
    BOOST_CHECK_EQUAL( map( Particle( V3( 1.0 , 0., 0. ) ) ), 6 );
    BOOST_CHECK_EQUAL( map( Particle( V3( 2.0 , 0., 0. ) ) ), 7 );
    BOOST_CHECK_EQUAL( map( Particle( V3( 3.0 , 0., 0. ) ) ), 8 );
    BOOST_CHECK_EQUAL( map( Particle( V3( 4.0 , 0., 0. ) ) ), 9 );
    BOOST_CHECK_EQUAL( map( Particle( V3( 5.0 , 0., 0. ) ) ), 9 );
    BOOST_CHECK_EQUAL( map( Particle( V3( 6.0 , 0., 0. ) ) ), 9 );
    BOOST_CHECK_EQUAL( map( Particle( V3( 10.0, 0., 0. ) ) ), 9 );

    grid.x0[1u] = -2.5;
    BOOST_CHECK_EQUAL( map( Particle( V3(-5.0, -3.5, 0. ) ) ), 0 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-5.0, -2.5, 0. ) ) ), 0 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-5.0, -1.5, 0. ) ) ), 10 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-5.0, -0.5, 0. ) ) ), 20 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-5.0,  1.5, 0. ) ) ), 40 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-5.0,  2.5, 0. ) ) ), 40 );

    grid.x0[2u] = -1.0;
    BOOST_CHECK_EQUAL( map( Particle( V3(-5.0, -3.5, -2.0 ) ) ), 0 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-5.0, -3.5, -1.0 ) ) ), 0 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-5.0, -3.5,  0.0 ) ) ), 50 );
    BOOST_CHECK_EQUAL( map( Particle( V3(-5.0, -3.5,  1.0 ) ) ), 50 );

    grid.x0[0u] = -10.0;
    BOOST_CHECK_EQUAL( map( Particle( 0.0 ) ), 79 );
  }
}

