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

#ifndef xylose_integrate_rk5_h
#define xylose_integrate_rk5_h

#ifndef xylose_integrate_rk_h
#  include <xylose/integrate/rk.h>
#endif // xylose_integrate_rk_h

#include <stdexcept>

namespace xylose {
  namespace integrate {

    /** Integration done by an adaptive Runge-Kutta method yielding 5th order
     * accuracy.
     *
     * @tparam DerivativeFunctor
     *    The functor that calculates derivatives.  This functor must have a
     *    member function with the prototype:
     *<pre>
      void operator() ( const Vector<double,N> & x,
                        const double & t,
                        const double & dt,
                              Vector<double,N> & dxdt );
      </pre>
     *
     * @tparam RKTweak
     *    A hook to provide the user finer control over the rk integral driver.
     *    This also provides a mechanism for the user to apply a statistical
     *    force that can be separated from the normal forces.<br>
     *    [Default: NullRKTweak]
     */
    template < typename DerivativeFunctor, typename RKTweak >
    class RK<DerivativeFunctor,5u,RKTweak> {
      /* TYPEDEFS */
    public:
      /** Reflects whether a particular implementation is adaptive. */
      static const bool is_adaptive = true;


      /* MEMBER STORAGE */
    public:
      /** The DerivativeFunctor instance. */
      DerivativeFunctor derivs;

      /** The RKTweak instance. */
      RKTweak rkTweak;

      /** Error tolerance used for adaptive Runge-Kutta.  */
      double errmax;

      /** The number of attempts at performing the transport via rk5 to reach a
       * desired accuracy. */
      int number_tries;

      /** The number of rk5 retries after finding the accuracy desired was not
       * acheived. */
      int number_retries;



      /* MEMBER FUNCTIONS */
    public:
      /** Default constructor sets errmax to 1e-5 and requires that
       * DerivativeFunctor have a default constructor.
       */
      RK() : derivs(), rkTweak(),
             errmax(1e-5), number_tries(0), number_retries(0) { }

      /** Constructor sets errmax to 1e-5 and allows DerivativeFunctor to be
       * passed an argument into its constructor.
       * @param t
       *    An arbitrary argument for the DerivativeFunctor constructor.
       */
      template < typename T >
      RK( const T & t ) : derivs(t), rkTweak(),
                          errmax(1e-5), number_tries(0), number_retries(0) { }

      /** Integrate using an adaptive Runge-Kutta which yields 5th order
       * accuracy.
       * This driver computes the integral x(ti+dt) = x(ti) + Int[f, ti, ti+dt].
       * @param x
       *      Dependent variables; input x(ti), output x(ti+dt).
       * @param ti
       *      The starting time in the integral.
       * @param dt
       *      The integration length.
       * @param dt_step
       *      The sub-integration length to try for the adaptive method.
       */
      template <unsigned int ndim_>
      void operator() (       Vector<double,ndim_> & x,
                        const double & ti,
                        const double & dt,
                              double & dt_step );



    private:
      /* Fifth-order Runge-Kutta step with monitoring of local truncation error to ensure accuracy
       * and adjust stepsize.  
       *
       * @param p
       *    Input:  dependent variables (at time t)<br>
       *   Output:  dependent variables (at time t + dt_did)
       * @param dpdt
       *    derivatives of p(1:n) (at time t)
       * @param t
       *    Input:  current time before integration<br>
       *   Output:  current time after the integration
       * @param dt_try
       *    Input:  stepsize to first try<br>
       *   Output:  estimated next stepsize to try
       * @param eps
       *    required accuracy
       * @param p_scal
       *    error scaling for each of the dependent variables
       * @param dt_did
       *    actual stepsize accomplished
       *
       * @calls rkck
       *
       * NOTE:  I've simplified a lot of stuff that I didn't need for a dt>0 and
       * time-independent forces.  If dt < 0 is needed and time-dependent forces
       * are used, care must be taken to fix this.  Compare against the Numerical
       * Recipies for Fortran routines.
       */
      template < unsigned int ndim >
      void rkqs(       Vector<double,ndim> & p,
                 const Vector<double,ndim> & dpdt,
                       double & t,
                       double & dt_try,
                 const Vector<double,ndim> & p_scal,
                       double & dt_did,
                 const double & tf );
    
      /* Given values for n variables p and their derivatives dpdt known at t, use the fifth-order
       * Cash-Karp Runge-Kutta method to advance the solution over an interval h and return
       * the incremented variables as yout. Also return an estimate of the local truncation er-
       * ror in yout using the embedded fourth-order method. The user supplies the subroutine
       * derivs(p,t,dt,dpdt), which returns derivatives dpdt at p.
       */
      template < unsigned int ndim >
      void rkck( const Vector<double,ndim> & p,
                 const Vector<double,ndim> & D1,
                 const double & t,
                 const double & dt,
                       Vector<double,ndim> & p_out,
                       Vector<double,ndim> & p_err );
    };

  }/* namespace xylose::integrate */
}/* namespace xylose */

#include <xylose/integrate/rk5.cpp>

#endif // xylose_integrate_rk5_h
