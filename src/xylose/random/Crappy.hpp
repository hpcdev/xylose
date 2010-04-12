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


#ifndef xylose_random_Crappy
#define xylose_random_Crappy

/** \file Old Crappy random number generator
 * From "Random number generators: good ones are hard to find", S. Park and K.
 * Miller, Communications of ACM, October 1988, pp 1192-1201.  This is from page
 * 1195, and is to work on any system for which maxint is 2**31-1 or larger. Due
 * earlier to Schrage, as cited by P&M.
                                                                     
 * Note: OK values for iseed are 1 through 2**31-2. Give it 0 or 2*31-1 and it
 * will return the same values thereafter!
                                                                     
 * C version 6/91, Bruce Langdon.
                                                                     
 * Algorithm replaces seed by mod(a*seed,m). First represent seed = q*hi + lo.
 * Then a*seed = a*q*hi + lo = (m - r)*hi + a*lo = (a*lo - r*hi) + m*hi, and new
 * seed = a*lo - r*hi unless negative; if so, then add m. 

*/

#include <xylose/random/detail/RandBase.hpp>

#include <xylose/Vector.h>

#include <cmath>
#include <stdint.h>


namespace xylose {
  namespace random {

    /** Random number generator. */
    class Crappy : public detail::RandBase<Crappy> {
      /* TYPEDEFS */
    public:
      /** Number of seed elements in SeedVector. */
      static const unsigned int seed_length = 5u;
      static const unsigned int state_length = 6u;
      typedef xylose::Vector<uint32_t, seed_length> SeedVector;
      typedef xylose::Vector<int32_t, state_length> StateVector;
     
      /** Maximum value of randInt.  This overrides the RandBase::rand_max. */
      static const int32_t rand_max = 1073741824;
      static const int32_t mod = rand_max;
      static const int32_t a = 1103515245;
      static const int32_t b = 12345;


    protected:
      typedef detail::RandBase<Crappy> super;


      /* MEMBER STORAGE */
    private:
      /** StateVector. */
      StateVector state;

    
      /* MEMBER FUNCTIONS */
    public:
      /** Constructor. */
      Crappy() : super() { }

      /**  Constructor with explicit scalar seed value given. */
      Crappy( const uint32_t & iseed ) : super(false) {
        seed(iseed);
      }

      /**  Constructor with explicit vector seed value given. */
      Crappy( const SeedVector & vseed ) : super(false) {
        seed(vseed);
      }

      /**  Constructor with explicit state given. */
      Crappy( const StateVector & state ) : super(false), state(state) { }

      /** Seed using a single value. */
      void seed( int32_t iseed ) {
        #if 0
        THIS IS COMMENTED OUT BECAUSE THE OLD Crappy VERSION DID NOT DO THIS AND
        WE NEED TO COMPARE THE UPDATE.  AFTER THIS COMPARISON, WE CAN REENABLE
        THIS LINE.
        /* We need to make sure that iseed is positive, because this generator
         * works on ints and not uints. */
        iseed = std::abs(iseed);
        #endif

        /* First time, fill in the state */

        // Burn the first 100 lines
        for ( unsigned int ii = 0; ii < 100; ++ii )
          iseed = simple_rand(iseed);

        state[0] = simple_rand(iseed);

        for ( unsigned int i = 1; i < (state_length-1); ++i )
          state[i] = simple_rand(state[i-1]);

        state[state_length-1] = 0;

        /* FOLLOWING THE OLD Crappy VERSION, WE BURN THROUGH THE FIRST NUMBER
         * HERE: */
        static_cast<void>( randInt() );
      }

      /** (Re)seed the generator from a vector of length seed_length. */
      void seed( const SeedVector & vseed ) {
        /* We need to make sure that the seed is positive, because this
         * generator works on ints and not uints. */
        for ( unsigned int i = 0; i < seed_length; ++i )
          state[i] = std::abs( static_cast<int32_t>(vseed[i]) );

        state[state_length-1] = 0;

        /* The first number is often bad if automatic seeding is done.  Thus WE
         * BURN THROUGH ANOTHER NUMBER HERE.  I've tested the generator for
         * seeding with a single uint32_t from from the constructor over the
         * complete range of valid uint32_t numbers.  There are no problems with
         * that seeding type, but only when we seed the entire vector.  There
         * must be a more restrictive limit on the values of the items in the
         * state vector that what I've coded up above. */
        static_cast<void>( randInt() );
      }

      /** Get a const reference to the StateVector. */
      const StateVector & getState() const {
        return state;
      }

      /** Save the StateVector to an external storage. */
      void save( StateVector & s ) {
        s = state;
      }

      /** Load the StateVector from an external storage. */
      void load( const StateVector & s ) {
        state = s;
      }
    
      /** Very simple, but not good, generator. */
      int32_t simple_rand(int32_t seed) {
        return static_cast<int32_t>( static_cast<uint32_t>(a*seed + b) % mod );
      }

      /** integer in [1,2^31 - 2].
       * TODO:  fix documentation; is the range correct?
       */
      uint32_t randInt() {
        int32_t & st_last = state[state_length-1];

        uint32_t chosen = simple_rand(st_last);
        st_last = ((state_length-1)*chosen)/mod;
        chosen = state[st_last];
        state[st_last] = simple_rand(chosen);

        return chosen;
      }

    };

  }/* namespace xylose::random */
}/* namespace xylose */

#endif // xylose_random_Crappy
