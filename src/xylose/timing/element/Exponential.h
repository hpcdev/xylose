// -*- c++ -*-
// $Id$
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


#ifndef xylose_timing_element_Exponential_h
#define xylose_timing_element_Exponential_h

#include <xylose/timing/element/Base.h>

#include <xylose/power.h>
#include <xylose/compat/math.hpp>

#include <limits>


namespace xylose {
  namespace timing {
    namespace element {

      /** Exponential rise/fall timing element. 
       * This timing element will change its value over the length of the element
       * (dt) from val_i to val_f.  The exponent and reverse parameters determine
       * the type of change (i.e. the type of exponential rise/fall).
       */
      struct Exponential : timing::element::Base {
        /* TYPEDEFS */
        typedef timing::element::Base super;

        /* MEMBER STORAGE */
        /** Exponent of change. */
        double exponent;
        /** Boolean to specify whether \f$ (1-z)^{k} \f$ (reverse==true) or 
         * \f$ z^{k} \f$ (reverse==false) is computed for the exponential rise/fall. 
         */
        bool reverse;
        /** Initial value of this interval. */
        double val_i;
        /** Final value of this interval. */
        double val_f;



        /* MEMBER FUNCTIONS */
        /** Constructor. */
        Exponential( const double & dt    = 0.0,
                     const double & expnt = 1.0,
                     const double & vi    = 0.0,
                     const double & vf    = 1.0 ) :
          super(dt),
          exponent( std::abs(expnt) ),
          reverse(expnt < 0.0),
          val_i(vi), val_f(vf) { }

        /** Required virtual destructor.  This one is a NO-OP. */
        virtual ~Exponential() {}

        /** Calculates the value of this timing element. */
        virtual double getValue(const double & t_rel) {
          /* to avoid imaginary numbers: */
          const double eps10 = 10. * std::numeric_limits<double>::epsilon();

          const double tau = (t_rel / dt);
          using xylose::fast_pow;

          if (t_rel >= dt)
            return val_f;
          else if (t_rel <= 0.0)
            return val_i;
          else if (reverse)
            return val_f + (  (val_i - val_f) * fast_pow((1.0 - tau) + eps10, exponent));
          else /* forward */
            return val_i + (  (val_f - val_i) * fast_pow(tau, exponent));
        }

        /** Create a clone. */
        virtual Base * clone() const {
          return new Exponential(*this);
        }
      };

    }/* namespace xylose::timing::element */
  }/* namespace xylose::timing */
}/* namespace xylose */

#endif // xylose_timing_element_Exponential_h
