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

#define BOOST_TEST_MODULE random_Crappy

#include <xylose/random/test/common.hpp>
#include <xylose/random/Crappy.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/cstdint.hpp>

#include <vector>

namespace {
  using  boost::int32_t;
  using boost::uint32_t;
}

BOOST_AUTO_TEST_CASE( generation ) {
  namespace XRNG = xylose::random;
  XRNG::test::run< XRNG::Crappy >("crappy", true);

  {
    xylose::make_vector<uint32_t,5u> mkV5;
    /* This was known at one point in time to cause failure. */
    XRNG::Crappy r(
      mkV5( 2981107794, 1283166457, 3823313073, 2005073031, 2954216106 )
    );

    BOOST_CHECK( static_cast<int32_t>(r.randInt()) >= 0 );
  }

}

