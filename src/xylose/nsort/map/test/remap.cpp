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


#include <xylose/nsort/map/remap-_1D.h>
#include <xylose/nsort/map/remap-_2D.h>
#include <xylose/nsort/map/remap-_3D.h>

#include <xylose/Vector.h>

#define BOOST_TEST_MODULE  remap

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
                  "}";
  }
}


BOOST_AUTO_TEST_SUITE( remap );

BOOST_AUTO_TEST_CASE( remap_1D ) {
  using xylose::nsort::map::remap;
  using xylose::nsort::map::_1D;
  using xylose::nsort::map::pivot_ctor;
  typedef pivot_ctor< remap< _1D<0> > > map_t;
  map_t rmap(V3(0,0,0));

  Particle p;


  /* This for loop should check for a remapping of every value, one at a time.
   * It also ensures that the unmapped values continue to perform an identity
   * transformation.
   * */
  for (unsigned int ri = 0; ri < rmap.nval; ++ri) {
    for (unsigned int rv = 0; rv < rmap.nval; ++rv) {
      rmap.reset();
      rmap.m_remap[ri] = rv;
      unsigned int rii = 0;
//    for (p.x[2] = -1; p.x[2] <= 1.1; p.x[2] += 2) {
//      for (p.x[1] = -1; p.x[1] <= 1.1; p.x[1] += 2) {
          for (p.x[0] = -1; p.x[0] <= 1.1; p.x[0] += 2) {
            /* Just a dummy check to make sure that the remapped value matches
             * what is stored in the m_remap array. */
            BOOST_CHECK_EQUAL( rmap(p), rmap.m_remap[rii] );
            if (rii == ri)
              BOOST_CHECK_EQUAL( rmap(p), static_cast<int>(rv) );
            else
              BOOST_CHECK_EQUAL( rmap(p), static_cast<int>(rii) );
            ++rii;
//    } }
  } }     }


  /* Now check after re-mapping. */
  rmap.m_remap[1] = 0;
  p.x[0] = -1;
  BOOST_CHECK_EQUAL( rmap(p), 0 );

  p.x[0] =  1;
  BOOST_CHECK_EQUAL( rmap(p), 0 );

  /* Now check after re-mapping the other way. */
  rmap.m_remap[0] = 1;
  rmap.m_remap[1] = 1;
  p.x[0] = -1;
  BOOST_CHECK_EQUAL( rmap(p), 1 );

  p.x[0] =  1;
  BOOST_CHECK_EQUAL( rmap(p), 1 );
}

BOOST_AUTO_TEST_CASE( remap_2D ) {
  using xylose::nsort::map::remap;
  using xylose::nsort::map::_2D;
  using xylose::nsort::map::pivot_ctor;
  typedef pivot_ctor< remap< _2D<0,1> > > map_t;
  map_t rmap(V3(0,0,0));

  Particle p;

  /* This for loop should check for a remapping of every value, one at a time.
   * It also ensures that the unmapped values continue to perform an identity
   * transformation.
   * */
  for (unsigned int ri = 0; ri < rmap.nval; ++ri) {
    for (unsigned int rv = 0; rv < rmap.nval; ++rv) {
      rmap.reset();
      rmap.m_remap[ri] = rv;
      unsigned int rii = 0;
//    for (p.x[2] = -1; p.x[2] <= 1.1; p.x[2] += 2) {
        for (p.x[1] = -1; p.x[1] <= 1.1; p.x[1] += 2) {
          for (p.x[0] = -1; p.x[0] <= 1.1; p.x[0] += 2) {
            /* Just a dummy check to make sure that the remapped value matches
             * what is stored in the m_remap array. */
            BOOST_CHECK_EQUAL( rmap(p), rmap.m_remap[rii] );
            if (rii == ri)
              BOOST_CHECK_EQUAL( rmap(p), static_cast<int>(rv) );
            else
              BOOST_CHECK_EQUAL( rmap(p), static_cast<int>(rii) );
            ++rii;
//    }
  } }   } }


  /* Now check after re-mapping. */
  rmap.m_remap[0] = 0; rmap.m_remap[2] = 0; rmap.m_remap[3] = 1;
  p.x[0] = -1; p.x[1] = -1;
  BOOST_CHECK_EQUAL( rmap(p), 0 );

  p.x[0] =  1; p.x[1] = -1;
  BOOST_CHECK_EQUAL( rmap(p), 1 );

  p.x[0] = -1; p.x[1] =  1;
  BOOST_CHECK_EQUAL( rmap(p), 0 );

  p.x[0] =  1; p.x[1] =  1;
  BOOST_CHECK_EQUAL( rmap(p), 1 );
}

BOOST_AUTO_TEST_CASE( remap_3D ) {
  using xylose::nsort::map::remap;
  using xylose::nsort::map::_3D;
  using xylose::nsort::map::pivot_ctor;
  typedef pivot_ctor< remap< _3D<0,1,2> > > map_t;
  map_t rmap(V3(0,0,0));

  Particle p;

  /* This for loop should check for a remapping of every value, one at a time.
   * It also ensures that the unmapped values continue to perform an identity
   * transformation.
   * */
  for (unsigned int ri = 0; ri < rmap.nval; ++ri) {
    for (unsigned int rv = 0; rv < rmap.nval; ++rv) {
      rmap.reset();
      rmap.m_remap[ri] = rv;
      unsigned int rii = 0;
      for (p.x[2] = -1; p.x[2] <= 1.1; p.x[2] += 2) {
        for (p.x[1] = -1; p.x[1] <= 1.1; p.x[1] += 2) {
          for (p.x[0] = -1; p.x[0] <= 1.1; p.x[0] += 2) {
            /* Just a dummy check to make sure that the remapped value matches
             * what is stored in the m_remap array. */
            BOOST_CHECK_EQUAL( rmap(p), rmap.m_remap[rii] );
            if (rii == ri)
              BOOST_CHECK_EQUAL( rmap(p), static_cast<int>(rv) );
            else
              BOOST_CHECK_EQUAL( rmap(p), static_cast<int>(rii) );
            ++rii;
  } } } } }


  /* Check after re-mapping by hand a few values at a time */
  rmap.m_remap[0] = 2; rmap.m_remap[2] = 4; rmap.m_remap[4] = 6;
  rmap.m_remap[3] = 5; rmap.m_remap[5] = 3; rmap.m_remap[7] = 1;
  p.x[0] = -1; p.x[1] = -1; p.x[2] = -1;
  BOOST_CHECK_EQUAL( rmap(p), 2 );

  p.x[0] =  1; p.x[1] = -1; p.x[2] = -1;
  BOOST_CHECK_EQUAL( rmap(p), 1 );

  p.x[0] = -1; p.x[1] =  1; p.x[2] = -1;
  BOOST_CHECK_EQUAL( rmap(p), 4 );

  p.x[0] =  1; p.x[1] =  1; p.x[2] = -1;
  BOOST_CHECK_EQUAL( rmap(p), 5 );

  p.x[0] = -1; p.x[1] = -1; p.x[2] =  1;
  BOOST_CHECK_EQUAL( rmap(p), 6 );

  p.x[0] =  1; p.x[1] = -1; p.x[2] =  1;
  BOOST_CHECK_EQUAL( rmap(p), 3 );

  p.x[0] = -1; p.x[1] =  1; p.x[2] =  1;
  BOOST_CHECK_EQUAL( rmap(p), 6 );

  p.x[0] =  1; p.x[1] =  1; p.x[2] =  1;
  BOOST_CHECK_EQUAL( rmap(p), 1 );

}

BOOST_AUTO_TEST_SUITE_END();

