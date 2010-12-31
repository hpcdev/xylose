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
#include <xylose/nsort/map/remap.h>
#include <xylose/nsort/map/w_species.h>

#include <xylose/Vector.h>
#include <xylose/Dimensions.hpp>

#define BOOST_TEST_MODULE  map_species

#include <boost/test/unit_test.hpp>

namespace {
  using xylose::Vector;
  using xylose::V3;
  using xylose::Dimensions;

  struct Particle {
    Vector<double, 3u> x;
    unsigned int species;
    Particle(const Vector<double,3u> & x = 0.0,
             const unsigned int & species = 0  ) : x(x), species(species) {}
  };

  inline const Vector<double,3u> & position( const Particle & p ) {
    return p.x;
  }

  const unsigned int & species( const Particle & p ) {
    return p.species;
  }

}

BOOST_AUTO_TEST_CASE( map_1D_w_species ) {
  using xylose::nsort::map::pivot;
  using xylose::nsort::map::w_species;
  typedef w_species< pivot< Dimensions<0u> > > map_t;
  map_t map( std::make_pair(1u, V3(0,0,0)) );


  /* make sure the dimensionality is correct. */
  BOOST_CHECK_EQUAL( static_cast<int>(map_t::dimensions::ndims), 1 );

  Particle p;
  p.x[0] = -1;
  BOOST_CHECK_EQUAL( map(p), 0 );

  p.x[0] =  1;
  BOOST_CHECK_EQUAL( map(p), 1 );
}

BOOST_AUTO_TEST_CASE( remap_1D_w_species ) {
  using xylose::nsort::map::remap;
  using xylose::nsort::map::pivot;
  using xylose::nsort::map::w_species;
  typedef w_species< remap< pivot< Dimensions<0u> > > > map_t;
  map_t rmap( std::make_pair(1u, V3(0,0,0)) );

  Particle p;
  p.x[0] = -1;
  BOOST_CHECK_EQUAL( rmap(p), 0 );

  p.x[0] =  1;
  BOOST_CHECK_EQUAL( rmap(p), 1 );
}

