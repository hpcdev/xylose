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


#ifndef xylose_random_RandBase
#define xylose_random_RandBase

#include <xylose/Vector.h>

#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <stdint.h>

namespace xylose {
  namespace random {
    namespace detail {
      using xylose::Vector;

      /** Base class to provide generators that have a (uint32_t) (*randInt)()
       * function with various other functions.
       *
       * An iheriting class MUST implement the following:
       * - data types, static ints:
       *    - static const unsigned int seed_length;
       *    .
       * - functions:
       *    - uint32_t randInt();
       *    - void seed(const xylose::Vector<uint32_t, seed_length> &);
       *    - void seed(const uint32_t &);
       *    .
       * An iheriting class SHOULD implement the following:
       * - data types, static ints:
       *    - static const unsigned int state_length;
       *    - typedef xylose::Vector<uint32_t,seed_length> SeedVector;
       *    - typedef xylose::Vector<uint32_t,state_length> StateVector;
       *    .
       * - functions:
       *    - const StateVector & getState() const;
       *    - void save(StateVector &);
       *    - void load(const StateVector &);
       *    .
       * The iheriting class CAN implement the following:
       * - data types, static ints:
       *    - static const uint32_t rand_max;
       *    .
       */
      template < typename sub >
      struct RandBase {
        /* TYPEDEFS */
        /** The maximum value of randInt().  Sub-classes should override this
         * with their own value if needed.  sub::rand_max will always be used
         * when trying to obtain the value of this.  */ 
        static const uint32_t rand_max = static_cast<uint32_t>(-1);



        /* MEMBER FUNCTIONS */
        inline double rand();       /**< Real nubmer in [0,1]. */
        inline double randExc();    /**< Real nubmer in [0,1). */
        inline double randDblExc(); /**< Real nubmer in (0,1). */

        /** Typed number in [0,x], where x depends on template parameter T.
         * The value of x depends on the type givin via the template parameter T.
         * This list shows the value of x for some types of T:
         * - uint32_t :  (2^32) - 1
         * - int32_t  :  (2^31) - 1
         * - double   :  (1.0)
         * - float    :  (1.0f)
         * .
         */
        template < typename T > inline T randT();

        /** (Re)seed with /dev/urandom or time() and clock(). 
         * If /dev/urandom is not accessible, then the results of time() and
         * clock() are hashed together to generate a seed. */
        inline void seed();

      protected:
        /** auto-initialize with /dev/urandom or time() and clock(). */
        RandBase() { seed(); }

        /** Do NOT initialize with /dev/urandom or time() and clock(). */
        RandBase(bool) { }

        /** Hash together a time value and a clock value to generate a seed. */
        inline uint32_t hash( time_t t, clock_t c );
      };



      template < typename sub >
      inline double RandBase<sub>::rand() {
        const double r_scale = 1.0 / static_cast<double>(sub::rand_max);
        register uint32_t r = static_cast<sub&>(*this).randInt();
        return r * r_scale;
      }

      template < typename sub >
      inline double RandBase<sub>::randExc() {
        const double rexc_scale = 1. / (static_cast<double>(sub::rand_max) + 1.);
        register uint32_t r = static_cast<sub&>(*this).randInt();
        return r * rexc_scale;
      }

      template < typename sub >
      inline double RandBase<sub>::randDblExc() {
        const double rexc_scale = 1. / (static_cast<double>(sub::rand_max) + 1.);
        register uint32_t r = static_cast<sub&>(*this).randInt();
        return (static_cast<double>(r) + 0.5) * rexc_scale;
      }




      /** Helper class for randT<T>. */
      template < typename T >
      struct RandT {
        template < typename Generator >
        inline T operator() ( Generator & g ) {
          return static_cast<T>(g.rand());
        }
      };

      /** Helper class for randT<uint32_t>. */
      template <>
      struct RandT<uint32_t> {
        template < typename Generator >
        inline uint32_t operator() ( Generator & g ) {
          register uint32_t r = g.randInt();
          return r;
        }
      };

      /** Helper class for randT<int32_t>. */
      template <>
      struct RandT<int32_t> {
        template < typename Generator >
        inline int32_t operator() ( Generator & g ) {
          register uint32_t r = g.randInt();
          return std::abs(static_cast<int32_t>(r));
        }
      };

      template < typename sub >
      template < typename T >
      inline T RandBase<sub>::randT() {
        return RandT<T>()(*this);
      }

      /* This implementation was taken from the BSD licensed MTRand from Richard
       * Wagner. */
      template < typename sub >
      inline void RandBase<sub>::seed() {
      	// Seed the generator with an array from /dev/urandom if available
      	// Otherwise use a hash of time() and clock() values
      	
      	// First try getting an array from /dev/urandom
      	FILE* urandom = fopen( "/dev/urandom", "rb" );
      	if( urandom ) {
          Vector<uint32_t,sub::seed_length> bigSeed;
      		register uint32_t *s = &bigSeed[0];
      		register int i = sub::seed_length;
      		register bool success = true;
      		while( success && i-- )
      			success = fread( s++, sizeof(uint32_t), 1, urandom );
      		fclose(urandom);
      		if( success ) {
            static_cast<sub&>(*this).seed( bigSeed );
            return;
          }
      	}

      	// Was not successful, so use time() and clock() instead
      	static_cast<sub&>(*this).seed( hash( time(NULL), clock() ) );
      }

      /* This implementation was taken from the BSD licensed MTRand from Richard
       * Wagner. */
      template < typename sub >
      inline uint32_t RandBase<sub>::hash( time_t t, clock_t c ) {
      	// Get a uint32_t from t and c
      	// Better than uint32_t(x) in case x is floating point in [0,1]
      	// Based on code by Lawrence Kirby (fred@genesis.demon.co.uk)

      	static uint32_t differ = 0;  // guarantee time-based seeds will change

      	uint32_t h1 = 0;
      	unsigned char *p = (unsigned char *) &t;
      	for( size_t i = 0; i < sizeof(t); ++i ) {
      		h1 *= UCHAR_MAX + 2U;
      		h1 += p[i];
      	}
      	uint32_t h2 = 0;
      	p = (unsigned char *) &c;
      	for( size_t j = 0; j < sizeof(c); ++j ) {
      		h2 *= UCHAR_MAX + 2U;
      		h2 += p[j];
      	}
      	return ( h1 + differ++ ) ^ h2;
      }

    }/* namespace xylose::random::detail */
  }/* namespace xylose::random */
}/* namespace xylose */

#endif // xylose_random_RandBase
