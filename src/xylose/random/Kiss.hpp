#ifndef xylose_random_Kiss
#define xylose_random_Kiss

#include <xylose/random/detail/RandBase.hpp>

#include <xylose/Vector.h>

#include <stdint.h>

namespace xylose {
  namespace random {

    /** Keep-it-simple-stupid random number generator.
     * This generator was taken from the DIEHARD package which includes a battery of
     * tests for random number generator.  
     *
     * @author George Marsaglia
     *  email: geo@stat.fsu.edu
     *
     *
     * The original documentation for the fortran version from G. Marsaglia:<br>
     * \verbatim
         :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
         ::    This program creates the binary file kiss.32, containing     ::
         ::  11+ megabytes of integers from the generator KISS, which com-  ::
         ::  bines three simple generators.  The acronym KISS means         ::
         ::               Keep It Simple Stupid                             ::
         ::  and the idea is to use simple, fast, individually promising    ::
         ::  generators to get a composite that will be fast, easy to code  ::
         ::  have a very long period and pass all the tests put to it.      ::
         ::  The three components of KISS are                               ::
         ::               x(n)=a*x(n-1)+1 mod 2^32                          ::
         ::               y(n)=y(n-1)(I+L^13)(I+R^17)(I+L^5),               ::
         ::               z(n)=2*z(n-1)+z(n-2) +carry mod 2^32              ::
         ::  The y's are a shift register sequence on 32bit binary vectors  ::
         ::  period 2^32-1; see the description in executing makesupr.exe.  ::
         ::  The z's are a simple multiply-with-carry sequence with period  ::
         ::  2^63+2^32-1.  The period of KISS is thus                       ::
         ::       2^32*(2^32-1)*(2^63+2^32-1) > 2^127                       ::
         ::  KISS is particularly well suited for assembler programming,    ::
         ::  where it takes about 200 nanosecs with a Pentium 120.          ::
         ::    It seems to pass all tests and is highly recommended  for    ::
         ::   speed and simplicity (for generators with that long a period) ::
         :: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ::
     * \endverbatim
     */
    class Kiss : public detail::RandBase<Kiss> {
      /* TYPEDEFS */
    public:
      /** Number of seed elements in SeedVector. */
      static const unsigned int seed_length = 4u;
      /** Number of state elements in StateVector. */
      static const unsigned int state_length = 5u;
      /** SeedVector type. */
      typedef xylose::Vector<uint32_t, seed_length> SeedVector;
      /** StateVector type. */
      typedef xylose::Vector<uint32_t, state_length> StateVector;

    protected:
      typedef detail::RandBase<Kiss> super;
      static const uint32_t a = 69069u;

      /* MEMBER STORAGE */
      /** State vector.
       * The elements are:
       * - state[0] : x
       * - state[1] : y
       * - state[2] : z
       * - state[3] : w
       * - state[4] : carry
       * .
       */
      StateVector state;

    public:
      /* MEMBER FUNCTIONS */
      /** auto-initialize with /dev/urandom or time() and clock(). */
	    Kiss() : super() {}

      /**  Constructor with explicit seed values given. */
      Kiss( const uint32_t & _x,
            const uint32_t & _y = 0u,
            const uint32_t & _z = 0u,
            const uint32_t & _w = 0u ) : super(false) {
        seed(_x,_y,_z,_w);
      }

      /**  Constructor with explicit vector seed value given. */
      Kiss( const SeedVector & vseed ) : super(false) {
        seed(vseed);
      }

      /**  Constructor with explicit state given. */
      Kiss( const StateVector & state ) : super(false), state(state) { }

      /** (Re)seed the generator. */
      void seed( const uint32_t & _x,
                 uint32_t _y = 0u,
                 uint32_t _z = 0u,
                 uint32_t _w = 0u ) {
        if(!_y) _y = _x + 1u;
        if(!_z) _z = _x + 2u;
        if(!_w) _w = _x + 3u;

        state[0] = _x;
        state[1] = _y;
        state[2] = _z;
        state[3] = _w;
        state[4] = 0u; /* carry set to zero */
      }

      /** (Re)seed the generator from a vector of length seed_length(4). */
      void seed( const SeedVector & s ) {
        VNCAST(uint32_t,seed_length,state.val) = s;
        state[4] = 0u; /* carry set to zero */
      }

      /** integer in [0,2^32 - 1]. */
      uint32_t randInt() {
        uint32_t &x     = state[0],
                 &y     = state[1],
                 &z     = state[2],
                 &w     = state[3],
                 &carry = state[4];

        x = a*x + 1u;
        y ^= y << 13u;
        y ^= y >> 17u;
        y ^= y <<  5u;

        register uint32_t k = (z >> 2u) + (w >> 3u);
        register uint32_t m = w + w + z + carry;
        z = w;
        w = m;
        carry = k >> 30u;

        return x + y + w;
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

    };

  }/* namespace xylose::random */
}/* namespace xylose */

#endif // xylose_random_Kiss
