/* This implementation of the poisson random variate is taken from Numpy and has
 * the following copyright and licencing notice:
 * ----- BEGIN ORIGINAL NOTICE -----
 * Copyright 2005 Robert Kern (robert.kern@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * ----- END ORIGINAL NOTICE -----
 *
 * Note that modifications were made to make this use a similar interface to
 * xylose::random::GaussianDeviate.
 */

#ifndef xylose_random_PoissonianDeviate_h
#define xylose_random_PoissonianDeviate_h

#include <xylose/random/Kiss.hpp>
#include <xylose/compat/math.hpp>

#include <limits>


namespace xylose {
  namespace random {


    template < typename RNG = xylose::random::Kiss >
    class PoissonianDeviate {
      /* TYPEDEFS */
      /** Poisson distribution computed by repeated multiplication of uniform variates.
       * Devroye, Luc. _Non-Uniform Random Variate Generation_.
       *  Springer-Verlag, New York, 1986.
       *  http://cgm.cs.mcgill.ca/~luc/rnbookindex.html
       */
      struct PoissonMult {
        unsigned long long operator() ( const double & lam, RNG & rng ) const {
          double enlam = exp(-lam);
          unsigned long long X = 0u;
          double prod = 1.0;

          while (true) {
            prod *= rng.rand();

            if (prod > enlam)
              X += 1u;
            else
              return X;
          }
        }
      };

      /** Poisson distribution computer by the PTRS algorithm.
       * @see Hoermann, W. The Transformed Rejection Method for Generating Poisson
       * Random Variables. Insurance: Mathematics and Economics, (to appear).
       * http://citeseer.csail.mit.edu/151115.html
       */
      struct PoissonPTRS {
        unsigned long long operator() ( const double & lam, RNG & rng ) const {
          using std::log;
          using std::sqrt;
          using ::lgamma;

          double loglam = log(lam);
          double b = 0.931 + 2.53 * sqrt(lam);
          double a = -0.059 + 0.02483 * b;
          double ln_invalpha = log( 1.1239 + 1.1328/(b-3.4) );
          double vr = 0.9277 - 3.6224/(b-2);

          while (true) {
            double U = rng.rand() - 0.5;
            double V = rng.rand();
            double us = 0.5 - std::abs(U);
            unsigned long long retval
              = static_cast<unsigned long long>( (2*a/us + b)*U + lam + 0.43 );

            if ((us >= 0.07) && (V <= vr))
              return retval;

            if ((retval < 0) || ((us < 0.013) && (V > us)))
              continue;

            if ((log(V) + ln_invalpha - log(a/(us*us)+b)) <=
                (-lam + retval*loglam - lgamma(retval+1)))
              return retval;
          }
        }
      };


      /* MEMBER STORAGE */
    private:
      PoissonMult mult;
      PoissonPTRS ptrs;

    public:
      /** Reference to the random number generator. */
      RNG & rng;


      /* STATIC STORAGE */
    public:
      static RNG global_rng;

      static const unsigned long long lambda_max;


      /* MEMBER FUNCTIONS */
    public:
      PoissonianDeviate( RNG & rng = global_rng )
        : rng(rng) { }

      /* Poisson distribution with mean=lam.
       * When lam < 10, a basic algorithm using repeated multiplications of uniform
       * variates is used (Devroye p. 504).
       * When lam >= 10, algorithm PTRS from (Hoermann 1992) is used.
       */
      unsigned long long operator() ( const double & lam ) const {
        if      (lam < 10.0)
          return mult(lam, rng);
        else if (lam <= 0.0)
          return 0u;
        else if (lam > lambda_max)
          /* just return lambda if we can't really represent the full
           * distribution in the dynamic range of the unsigned long long. */
          return lam;
        else
          return ptrs(lam, rng);
      }

    };

    template < typename RNG >
    RNG PoissonianDeviate<RNG>::global_rng;

    template < typename RNG >
    const unsigned long long PoissonianDeviate<RNG>::lambda_max =
        std::numeric_limits<unsigned long long>::max() - 
        static_cast<unsigned long long>(
          10.0* std::sqrt( std::numeric_limits<unsigned long long>::max() )
        );

  }/* namespace xylose::random */
}/* namespace xylose */

#endif // xylose_random_PoissonianDeviate_h
