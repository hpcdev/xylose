// -*- c++ -*-
// $Id: RKIntegrator.h,v 1.1 2005/04/19 17:23:21 olsonse Exp $
/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2005-2009 Spencer E. Olson
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
 * $Log: RKIntegrator.h,v $
 * Revision 1.1  2005/04/19 17:23:21  olsonse
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
 *
 */

/** \file
 * Some c++ abstracting for integrating functions, starting of course with
 * Runge-Kutta.
 *
 * Copyright 2005-2009 Spencer E. Olson
 */

#ifndef xylose_integrate_rk2_h
#define xylose_integrate_rk2_h

#ifndef xylose_integrate_rk_h
#  include <xylose/integrate/rk.h>
#endif // xylose_integrate_rk_h

#include <xylose/integrate/detail/Derivs.h>

namespace xylose {
  namespace integrate {

    /** Runge-Kutta implementation for 2nd order. */
    template < typename DerivativeFunctor,
               typename Targ0 >
    struct RK<DerivativeFunctor,2u,Targ0> {
      /* TYPEDEFS */
      /** Reflects whether a particular implementation is adaptive. */
      static const bool is_adaptive = false;


      /* MEMBER STORAGE */
      /** The DerivativeFunctor instance. */
      DerivativeFunctor derivs;


      /* MEMBER FUNCTIONS */
      /** Default constructor. */
      RK() : derivs() { }

      /** Constructor allows DerivativeFunctor to be passed an argument into its
       * constructor. 
       * @param t
       *    An arbitrary argument for the DerivativeFunctor constructor.
       */
      template < typename T >
      RK( const T & t ) : derivs(t) { }

      /** Integrate using Runge-Kutta which yields 2nd order accuracy.
       * This driver computes the integral x(t+dt) = x(t) + Int[f, t, t+dt].
       * @param x
       *      Dependent variables; input x(t), output x(t+dt).
       * @param t
       *      The starting time in the integral.
       * @param dt
       *      The integration length.
       */
      template < unsigned int ndim,
                 typename Optional >
      inline void operator() (       Vector<double,ndim> & x,
                               const double & t,
                               const double & dt,
                                     Optional & opt ) {
        Vector<double,ndim> x_tmp, D1, D2;
        detail::Derivs<Optional> call;

        // find D1
        call( derivs, x, t, dt, D1, opt );

        // find D2
        x_tmp = x + dt * D1;
        call( derivs, x_tmp, t+dt, dt, D2, opt );

        // now weight the sum
        x += 0.5 * dt * ( D1 + D2 );
      }

      template < unsigned int ndim >
      inline void operator() (       Vector<double,ndim> & x,
                               const double & t,
                               const double & dt ) {
        detail::NoOp noop;
        this->operator()( x, t, dt, noop );
      }
    };

  }/* namespace xylose::integrate */
}/* namespace xylose */

#endif // xylose_integrate_rk2_h
