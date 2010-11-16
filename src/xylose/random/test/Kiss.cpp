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

#define BOOST_TEST_MODULE random_Kiss

#include <xylose/random/Kiss.hpp>
#include <xylose/Timer.h>

#include <boost/test/unit_test.hpp>
#include <boost/cstdint.hpp>

#include <fstream>
#include <vector>
#include <iostream>

namespace {
  using boost::uint32_t;
}


BOOST_AUTO_TEST_CASE( generation ) {
  BOOST_TEST_MESSAGE(
    "This program is for generating binary output files from the Kiss random\n"
    "number generator.  There are two files generated which can be used as\n"
    "input to a random number testing package (such as DIEHARD by George\n"
    "Marsaglia).  The two files generated are:\n"
    "       test_kiss.dat\n"
    "           A sequence of random numbers generated by a single Kiss\n"
    "           instance that has been seeded with the default seed values\n"
    "           (Default seeds are 1,2,3,4).\n"
    "       test_kiss-intlv.dat\n"
    "           A sequence of random numbers generated by a set of Kiss\n"
    "           instances.  For each iteration, all the generators are\n"
    "           sequentially queried such that each number in the file\n"
    "           is from the same RNG generation as the numbers around it.\n"
  );


  using xylose::random::Kiss;

  const int total_rolls = 6000000;

  {
    Kiss r;
    std::ofstream out("test_kiss.dat");
    for ( int i = 0; i < total_rolls; ++i) {
      uint32_t val = r.randInt();
      out.write((char*)&val, sizeof(uint32_t));
    }
    out.close();
  }

  {
    Kiss r;
    for ( int i = 0; i < total_rolls; ++i) {
      double n = r.randExc();
      if ( n >= 1 || n < 0 )
        BOOST_CHECK_MESSAGE( n >= 0 && n < 1,
          __FILE__ "got a bad rand nubmer (" <<n <<")  on "<<i<<"th roll"
        );
    }
  }

  {
    Kiss r;
    std::ofstream out("test_kiss_randExc.dat");
    for ( int i = 0; i < total_rolls; ++i)
      out << r.randExc() << '\n';
    out.close();
  }

  {
    const int n_generators = 100000;
    const int max_iter = total_rolls / n_generators;

    Kiss sr;
    std::vector<Kiss> rv;
    for (int i = 0; i < n_generators; ++i) {
      rv.push_back(
        Kiss( sr.randInt(),
                  sr.randInt(),
                  sr.randInt(),
                  sr.randInt()
        )
      );
    }

    std::ofstream out("test_kiss-intlv.dat");
    for ( int i = 0; i < max_iter; ++i) {
      for (int n = 0; n < n_generators; ++n) {
        uint32_t val = rv[n].randInt();
        out.write((char*)&val, sizeof(uint32_t));
      }
    }
    out.close();
  }

  {
    Kiss r;
    unsigned long junk = 0u;
    xylose::Timer timer;
    timer.start();
    for ( unsigned long i = 0; i < 100000000UL; ++i ) {
      junk ^= r.randInt();
    }
    timer.stop();

    /* make sure that junk isn't optimized away */
    BOOST_CHECK_EQUAL( junk, junk );

    BOOST_TEST_MESSAGE(
      "Generation rate:  "
      << (1e2 / timer.dt) << " million per second \n"
         "                  "
      << (1e2 / timer.dt_cpu_time) << " million per cpu second"
    );
  }

}

