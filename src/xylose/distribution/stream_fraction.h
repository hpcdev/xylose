// -*- c++ -*-
// $Id: Distribution.h,v 1.3 2005/05/12 04:27:29 olsonse Exp $
/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2004-2008 Spencer Olson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *  
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *                                                                                 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.                                                                           .
 * 
 * Questions? Contact Spencer Olson (olsonse@umich.edu) 
 */

/*
 * $Log: Distribution.h,v $
 * Revision 1.3  2005/05/12 04:27:29  olsonse
 * Fixed to for Intel 8.1 compilers.
 * Found (using intel compiler) and fixed an array overflow in BField::potential.
 * Didn't find it earlier because the array is on the stack for the function.
 *
 * Added fmacros.h file to simplify mixing fortran code with others.
 * Added alias function names for Fortran interoperability.
 *
 * Revision 1.2  2005/04/19 17:23:21  olsonse
 * Added new RKIntegrator wrapper class to allow for generic integration
 * templates.
 *
 * Also added trapfe library to help with trapping floating point exceptions.
 *
 * Fixed Distribution inverter finally (hopefull).  It no longer truncates the
 * distribution or reads from bogus memory.
 *
 * Added rk2 integrator (modified Euler) to rk.F.
 *
 * Various other fixes.
 *
 * Revision 1.1  2005/01/14 14:09:31  olsonse
 * Fixed documentation on memory.h, msh.h, options.h.
 * Moved new files Distribution.[hC] listutil.h readData.h from dsmc code.
 *
 * Revision 1.1.1.1  2005/01/08 04:57:25  olsonse
 * Initial CVS import (finally).
 *
 *
 */

/** \file
 * Stream fraction calculations.
 *
 * Copyright 2004-2008 Spencer Olson.
 */

#ifndef xylose_distribution_stream_fraction_h
#define xylose_distribution_stream_fraction_h

#include <xylose/compat/math.hpp>

namespace xylose {
  namespace distribution {

    /** The fraction of average flux of a thermalized group of particles that are
     * moving up-stream.
     * The trivial case for v0 == 0 gives 0.5.
     *
     * @param v0
     *     v0 must be > 0.0; just call the correct function to get the correct
     *     fraction.
     * @param beta
     *     m/(2kT).
     *
     * Note that return value of this function is negative.  Also note that
     * upstream_fraction + downstream_fraction == 1.
     *
     * @see downstream_fraction.
     */
    inline double upstream_fraction(const double & v0, const double & beta) {
      using std::exp;
      using std::sqrt;
      double x = v0 * sqrt(beta);
      if (x == 0)
        return 0.5;

      return 0.5 * ( -exp(-SQR(x))/(sqrt(M_PI) * x) + (1.0 - erf(x)) );
    }

    /** The fraction of average flux of a thermalized group of particles that are
     * moving down-stream.
     * The trivial case for v0 == 0 gives 0.5.
     *
     * @param v0
     *     v0 must be > 0.0; just call the correct function to get the correct
     *     fraction.
     * @param beta
     *     m/(2kT).
     *
     * Note that return value of this function is negative.  Also note that
     * upstream_fraction + downstream_fraction == 1.
     *
     * @see upstream_fraction.
     */
    inline double downstream_fraction(const double & v0, const double & beta) {
      using std::exp;
      using std::sqrt;
      double x = v0 * sqrt(beta);
      if (x == 0)
        return 0.5;

      return 0.5 * (  exp(-SQR(x))/(sqrt(M_PI) * x) + (1.0 + erf(x)) );
    }

  }/* namespace xylose::distribution */
}/* namespace xylose */

#endif // xylose_distribution_stream_fraction_h
