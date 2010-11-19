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


#ifndef xylose_random_test_generate_files_hpp
#define xylose_random_test_generate_files_hpp

#include <xylose/Timer.h>
#include <xylose/Vector.h>

#include <boost/cstdint.hpp>
#include <boost/test/unit_test.hpp>

#include <string>
#include <fstream>

namespace xylose {
  namespace random {
    namespace test {

      using boost::uint32_t;

      inline bool generate_requested() {
        const std::string gen_option( "--generate-random" );

        namespace buf = boost::unit_test::framework;
        for ( int i = 0; i < buf::master_test_suite().argc; ++i ) {
          if ( gen_option == buf::master_test_suite().argv[i] )
            return true;
        }

        return false;
      }


      template < typename RNG >
      inline Vector<uint32_t, RNG::seed_length>
      makeSeedVector( RNG & seedRng ) {
        Vector<uint32_t, RNG::seed_length> retval;
        for ( unsigned int i = 0; i < RNG::seed_length; ++i )
          retval[i] = seedRng.randInt();

        return retval;
      }


      /** Generate files that can be used to validate the generator using
       * external and more involved tools (such as George Marsaglia's DIEHARD
       * Battery of Tests. */
      template < typename RNG >
      inline void generate_files( const std::string & label,
                                  const int & total_rolls ) {
        if ( ! generate_requested() )
          return;

        {
          RNG r;

          std::ofstream out( ("test_" + label + ".dat").c_str() );
          for ( int i = 0; i < total_rolls; ++i) {
            uint32_t val = r.randInt();
            out.write((char*)&val, sizeof(uint32_t));
          }
          out.close();
        }

        {
          RNG r;
          std::ofstream out( ("test_" + label + "_randExc.dat").c_str() );
          for ( int i = 0; i < total_rolls; ++i)
            out << r.randExc() << '\n';
          out.close();
        }

        {
          const int n_generators = 100000;
          const int max_iter = total_rolls / n_generators;

          RNG sr;
          std::vector<RNG> rv;
          for ( int i = 0; i < n_generators; ++i )
            rv.push_back( RNG( makeSeedVector( sr ) ) );

          std::ofstream out( ("test_" + label + "-intlv.dat").c_str() );
          for ( int i = 0; i < max_iter; ++i) {
            for (int n = 0; n < n_generators; ++n) {
              uint32_t val = rv[n].randInt();
              out.write((char*)&val, sizeof(uint32_t));
            }
          }
          out.close();
        }
      }




      /** Time a random number generator. */
      template < typename RNG >
      inline void timeRNG() {
        RNG r;
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


      /** Test randExc for a random number generator. */
      template < typename RNG >
      inline void testRandExc( const int & total_rolls, bool test_first_roll ) {
        RNG r;
        for ( int i = 0; i < total_rolls; ++i) {
          double n = r.randExc();
          if ( n >= 1 || n < 0 )
            BOOST_CHECK_MESSAGE( n >= 0 && n < 1,
              __FILE__ "got a bad rand nubmer (" <<n <<")  on "<<i<<"th roll"
            );

          if ( test_first_roll ) {
            n = RNG( static_cast<uint32_t>(i) ).randExc();
            if ( n >= 1 || n < 0 )
              BOOST_CHECK_MESSAGE( n >= 0 && n < 1,
                __FILE__ "got a bad rand nubmer (" <<n <<") for seed ("<<i<<')'
              );
          }
        }
      }

      /** Run the tests defined above. */
      template < typename RNG >
      inline void run( const std::string & label,
                       const bool & test_first_roll = false,
                       const int & total_rolls = 6000000 ) {
        BOOST_TEST_MESSAGE(
          "This program is for generating binary output files from the \n"
          "'"<<label<<"' random number generator.  If the command line option\n"
          "     --generate-random\n"
          "is given, two files generated which can be used as input to a\n"
          "random number testing package (such as DIEHARD by George \n"
          "Marsaglia).  The two files generated are:\n"
          "       test_"<<label<<".dat\n"
          "           A sequence of random numbers generated by a single \n"
          "           '"<<label<<"' instance that has been seeded with the \n"
          "           default seed values\n"
          "       test_"<<label<<"-intlv.dat\n"
          "           A sequence of random numbers generated by a set of\n"
          "           '"<<label<<"' instances.  For each iteration, all the\n"
          "           generators are sequentially queried such that each\n"
          "           number in the file is from the same RNG generation as\n"
          "           the numbers around it.\n"
        );

        generate_files<RNG>(label, total_rolls);
        testRandExc<RNG>(total_rolls, test_first_roll);
        timeRNG<RNG>();
      }

    }/* namespace xylose::random::test */
  }/* namespace xylose::random */
}/* namespace xylose */

#endif // xylose_random_test_generate_files_hpp
