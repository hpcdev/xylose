// -*- c++ -*-
// $Id: random.C,v 1.3 2005/05/12 04:27:50 olsonse Exp $
/*
 * Copyright 1997-2004 Spencer Olson
 *
 * $Log: random.C,v $
 * Revision 1.3  2005/05/12 04:27:50  olsonse
 * Fixed to for Intel 8.1 compilers.
 *
 * Added fmacros.h file to simplify mixing fortran code with others.
 * Added alias function names for Fortran interoperability.
 *
 * Revision 1.2  2005/04/19 17:23:39  olsonse
 * Various other fixes for new compilers.
 *
 * Revision 1.1.1.1  2005/01/08 04:27:25  olsonse
 * Initial import
 *
 * Revision 1.2  2004/04/13 15:54:15  labrat
 * Simplified and cleaned up gauss_deviate which is used to create the input
 * beam.
 * gauss_deviate now accepts only one parameter (sigma of distribution).
 *
 * Revision 1.1.1.1  2003/05/18 02:05:10  olsonse
 * Initial import of atomguide into cvs.
 * This marks the change from a running version that did not really do cells
 * correctly, to one where cells will correctly be done (hopefully).
 *
 *
 */

#ifndef xylose_random_gaussianDeviate_h
#define xylose_random_GaussianDeviate_h

#include <xylose/random/Kiss.hpp>

namespace xylose {
  namespace random {

    template < typename RNG = xylose::random::Kiss >
    class GaussianDeviate {
      /* MEMBER STORAGE */
    private:
      int iset;
      double gset, csigma;

    public:
      /** Reference to the random number generator. */
      RNG & rng;


      /* STATIC STORAGE */
    public:
      static RNG global_rng;


      /* MEMBER FUNCTIONS */
    public:
      GaussianDeviate( RNG & rng = global_rng )
        : iset(0), gset(0), csigma(0), rng(rng) { }

      /** The following is for getting a gaussian distribution of random
       * numbers.  This function uses a Box-Muller transformation to get the normal
       * deviates.
       * This function returns a normally distributed deviate with zero
       * mean and arbitrary variance, using ????? as the source of uniform deviates.
       *@memo Random Gaussian Deviate
      */
      double operator() ( const double & sigma ) {
        if ( iset == 1 && sigma == csigma ) { // We have an extra deviate handy, so
          iset = 0; // Unset flag.
          return gset; // and return the extra deviate.
        } else {
          double fac, r, v1, v2;
          csigma = sigma;

          do {
            /* We don't have an extra deviate handy, so
             * pick two uniform number in the square extending from -1 to +1
             * in each direction.  See if they are in the unit circle, and
             * if they are not, try again.
            */
            v1 = (2.0*rng.rand() - 1.0);
            v2 = (2.0*rng.rand() - 1.0);
            r = v1 * v1 + v2 * v2;
          } while ( r >= 1.0 );
          fac = sqrt( -2.0 * log( r ) / r );
          /* Now make the Box-Muller transformation to get two normal deviates.
           * Return one and save the other for the next time.
           * We will attempt to rescale the deviate to the
           * range given.
          */
          gset = v1 * fac * csigma;
          iset = 1; // Set flag.
          return v2 * fac * csigma;
        }
      }// operator()

    };

    template < typename RNG >
    RNG GaussianDeviate<RNG>::global_rng;

  }/* namespace xylose::random */
}/* namespace xylose */

#endif // xylose_random_GaussianDeviate_h
