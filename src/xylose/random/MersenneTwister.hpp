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


// This code evolved and was ported to c++ from code with the following header:
/* 
   A C-program for MT19937, with initialization improved 2002/2/10.
   Coded by Takuji Nishimura and Makoto Matsumoto.
   This is a faster version by taking Shawn Cokus's optimization,
   Matthe Bellew's simplification, Isaku Wada's real version.

   Before using, initialize the state by using init_genrand(seed) 
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

#ifndef xylose_random_MersenneTwister
#define xylose_random_MersenneTwister

#include <xylose/random/detail/RandBase.hpp>
#include <xylose/Vector.h>

#include <algorithm>

#include <stdint.h>

namespace xylose {
  namespace random {

    class MersenneTwister : public detail::RandBase<MersenneTwister> {
      /* TYPEDEFS */
    public:
      /** Length of SeedVector. */
      static const unsigned int seed_length = 624;

      /** Length of state vector. */
      static const unsigned int state_length = 624 + 1;

      /** SeedVector type. */
      typedef xylose::Vector<uint32_t, seed_length> SeedVector;

      /** StateVector type. */
      typedef xylose::Vector<uint32_t, state_length> StateVector;

    protected:
      /** Length of internal state vector. */
      static const int internal_state_length = 624; // must be signed

      /** InternalStateVector type. */
      typedef xylose::Vector<uint32_t, internal_state_length> InternalStateVector;

      typedef detail::RandBase<MersenneTwister> super;

      /** Period parameter. */
      static const int M = 397; // must be signed

      /* Period parameters */  
      /** constant vector a. */
      static const unsigned int matrix_a = 0x9908b0dfUL;
      /** most significant w-r bits. */
      static const unsigned int umask = 0x80000000UL;
      /** least significant r bits. */
      static const unsigned int lmask = 0x7fffffffUL;



      /* MEMBER STORAGE */
    protected:
      InternalStateVector state;
      uint32_t *next;
      int left;



      /* MEMBER FUNCTIONS */
    public:
      /** auto-initialize with /dev/urandom or time() and clock(). */
      MersenneTwister( ) : super() { }

      /**  Constructor with explicit seed value given. */
      MersenneTwister( const uint32_t & iseed /* = 5489UL */ ) : super(false) {
        seed(iseed);
      }

      /**  Constructor with explicit vector seed value given. */
      template < unsigned int seed_len >
      MersenneTwister( const xylose::Vector<uint32_t, seed_len> & vseed )
        : super(false) {
        seed(vseed);
      }

      /**  Constructor with explicit state given. */
      MersenneTwister( const StateVector & in_state ) : super(false) {
        using std::copy;
        copy( in_state.val, in_state.val + internal_state_length, state.val );
        left = in_state[internal_state_length];
      }

      /* initializes state with a seed */
      void seed( const uint32_t & s ) {
        state[0]= s & 0xffffffffUL;
        for ( int j=1; j < internal_state_length; ++j ) {
          state[j] = 1812433253UL * (state[j-1] ^ (state[j-1] >> 30)) + j; 
          /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
          /* In the previous versions, MSBs of the seed affect   */
          /* only MSBs of the array state[].                        */
          /* 2002/01/09 modified by Makoto Matsumoto             */
        }
        left = 1;
      }

      /** Initialize by an array with array-length.
       * init_key is the array for initializing keys.
       * key_length is its length.
       * slight change for C++, 2004/2/26.
       */
      template < unsigned int key_length >
      void seed( const xylose::Vector<uint32_t, key_length> & init_key ) {
        seed(19650218UL); // not sure why we seed like this first.
        int i = 1;
        for ( int k = ( internal_state_length > key_length ?
                        internal_state_length : key_length ),
                  j = 0;
                  k; --k ) {
          state[i] = (state[i] ^ ((state[i-1] ^ (state[i-1] >> 30)) * 1664525UL))
                   + init_key[j] + j; /* non linear */

          ++i; ++j;

          if ( i >= internal_state_length  ) {
            state[0] = state[internal_state_length-1];
            i = 1;
          }

          if ( j >= key_length )
            j = 0;
        }

        for ( int k= internal_state_length - 1; k; --k ) {
          state[i] = (state[i] ^ ((state[i-1] ^ (state[i-1] >> 30)) * 1566083941UL))
                   - i; /* non linear */
          ++i;
          if ( i >= internal_state_length ) {
            state[0] = state[internal_state_length-1];
            i = 1;
          }
        }

        state[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
        left = 1;
      }

      void next_state() {
        uint32_t *p = &state[0];

        // /* if init_genrand() has not been called, */
        // /* a default initial seed is used         */
        // if (finished_initialization==0)
        //   init_genrand(5489UL);

        left = internal_state_length;
        next = &state[0];
        
        for ( int j=internal_state_length-M+1; --j; ++p )
          *p = p[M] ^ twist(p[0], p[1]);

        for ( int j = M; --j; ++p ) 
          *p = p[M-internal_state_length] ^ twist(p[0], p[1]);

        *p = p[M-internal_state_length] ^ twist(p[0], state[0]);
      }

      /* generates a random number on [0,0xffffffff]-interval */
      uint32_t randInt() {

        if (--left == 0)
          next_state();
        register uint32_t y = *next++;

        /* Tempering */
        y ^= (y >> 11);
        y ^= (y << 7) & 0x9d2c5680UL;
        y ^= (y << 15) & 0xefc60000UL;
        y ^= (y >> 18);

        return y;
      }

      /** Get the external representation of the current state vector. */
      StateVector getState() const {
        StateVector out_state;
        using std::copy;
        copy( state.val, state.val + internal_state_length, out_state.val );
        out_state[internal_state_length] = left;
        return out_state;
      }
      
      /** Save the StateVector to an external storage. */
      void save( StateVector & s ) const {
        using std::copy;
        copy( state.val, state.val + internal_state_length, s.val );
        s[internal_state_length] = left;
      }
      
      /** Load the StateVector from an external storage. */
      void load( const StateVector & s ) {
        using std::copy;
        copy( s.val, s.val + internal_state_length, state.val );
        left = s[internal_state_length];
      }

    protected:
      uint32_t mixbits( const uint32_t & u, const uint32_t & v ) {
        return (u & umask) | (v & lmask);
      }

      uint32_t twist( const uint32_t & u, const uint32_t & v ) {
        return (mixbits(u,v) >> 1) ^ (v&1UL ? matrix_a : 0UL);
      }

    };

  }/* namespace xylose::random */
}/* namespace xylose */

#endif // xylose_random_MersenneTwister
