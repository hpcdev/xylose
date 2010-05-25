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

#ifndef xylose_integrate_rk4_h
#define xylose_integrate_rk4_h

#ifndef xylose_integrate_rk_h
#  include <xylose/integrate/rk.h>
#endif // xylose_integrate_rk_h

namespace xylose {
  namespace integrate {

    /** Runge-Kutta implementation for 4th order. */
    template < typename DerivativeFunctor,
               typename Targ0 >
    struct RK<DerivativeFunctor,4u,Targ0> {
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

      /** Integrate using Runge-Kutta which yields 4th order accuracy.
       * This driver computes the integral x(ti+dt) = x(ti) + Int[f, ti, ti+dt].
       * @param x
       *      Dependent variables; input x(ti), output x(ti+dt).
       * @param ti
       *      The starting time in the integral.
       * @param dt
       *      The integration length.
       * @param dummy_argument
       *      Unused for this implementation.
       */
      template <unsigned int ndim>
      inline void operator() (       Vector<double,ndim> & p,
                               const double & t,
                               const double & dt,
                               const double & dummy_argument = 0.0 ) {
        Vector<double,ndim> p_tmp, D1, D2, D3, D4;

        const double dth = 0.5 * dt;
        const double th = t + dth;

        // find D1
        derivs(p, t, dt, D1);

        // find D2
        p_tmp = p + dth * D1;
        derivs(p_tmp, th, dt, D2);

        // find D3
        p_tmp = p + dth * D2;
        derivs(p_tmp, th, dt, D3);

        // find D4
        p_tmp = p + dt * D3;
        derivs(p_tmp, t+dt, dt, D4);

        // now weight the sum
        p += 0.16666666666666666667 * dt * ( D1 + 2.0*(D2 + D3) + D4 );
      }
    };

  }/* namespace xylose::integrate */
}/* namespace xylose */

#endif // xylose_integrate_rk4_h
