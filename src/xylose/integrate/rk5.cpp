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

#include <xylose/integrate/rk5.h>
#include <xylose/strutil.h>

#include <limits>
#include <sstream>
#include <stdexcept>
#include <cmath>


namespace xylose {
  namespace integrate {

    template < typename DxDt, typename RKTweak >
    template < unsigned int ndim >
    void RK<DxDt,5u,RKTweak>::operator() (       Vector<double,ndim> & x,
                                           const double & ti,
                                           const double & dt,
                                                 double & dt_step ) {
      /* ensure that dt and dt_step have the same sign */
      dt_step = copysign(dt_step,dt);

      double  t = ti;
      double tf = ti + dt;

      /* direction of integration. */
      const double dir = copysign(1.0,dt);

      Vector<double,ndim> x_cal, dxdt;
      double  dt_step_next = dt_step,
              dt_step_current  = 0.0;

      int     truncated_step = 0;
      const double TINY = 1e-30;

      const double eps = std::numeric_limits<double>::epsilon();
      /** The 1.0 + minimum fraction of total current time to allow stepping. */
      const double TIME_COMP_EPS = 1.0 + ( 10.0 * eps );

      /* In this while-loop test, we are trying to avoid having time-steps
       * that are too small.  This might occur if the integration is nearly
       * complete and the truncated_step will cause a very very tiny
       * timestep that causes timestep underrun.  We are just going to throw
       * away anytiming that is as close as 10*eps*t. */
      while ( (t*dir*TIME_COMP_EPS) <  (tf*dir) ) {
        if ( ((t+dt_step)*dir) > (tf*dir) ) {
           // If stepsize can overshoot, decrease.
          dt_step = copysign(tf-t,dt);

          truncated_step = 1;
        } else {
          truncated_step = 0;
        }

        dt_step_current = dt_step;
        rkTweak.rkTweakFirst(x, t, (const double&)dt_step_current, dt_step);

        derivs(x, t, dt_step, dxdt);

        for (unsigned int i = 0; i < ndim; i++) {
          /* Scaling used to monitor accuracy. This
           * general-purpose choice can be modified if need be. */
          x_cal[i] = std::abs(x[i]) + std::abs( dt_step*dxdt[i] ) + TINY;
        }

        // time is accumulated in this function
        double told = t;
        rkqs(x, dxdt, t, dt_step, x_cal, dt_step_current, tf);


        if(std::abs(t - told) <= std::abs(t*1.5*eps)) {
          //std::stringstream ostr;
          //ostr << "stepsize underrun (" << dt_step_current
          //                              << " truncated=="<< truncated_step
          //                              << ", tried "<< dt_step
          //                              << ", next "<< dt_step_next ")"
          //        "at pos ("<<x<<") "
          //        "at t ("<<t<<"; old:"<<told<<") to tf ("<<tf<<")",
          throw std::runtime_error("stepsize underrun ("+to_string(dt_step)+")");
        }

        /* Allow user to provide a call back to adjust the next time step if
         * needed.
         * The user should be WARNED that any increases could cause the next
         * call to rkqs to spend too much time decreasing it again (so as to
         * meet the requested error).
         * This function can also serve as the source of a statistical force.
         *
         * TODO:  If we wanted to apply the 'Strang' type of splitting
         * here, we would do two RKTweak applications of
         * time-step/2.0:  one before the call to derivs at the beginning
         * of the while loop and one right here.
         */
        rkTweak.rkTweakSecond(x, t, (const double&)dt_step_current, dt_step);

        if ( truncated_step == 0 || std::abs(dt_step) < std::abs(dt_step_current)) {
          /* don't increase the stepsize beyond the previous corrected value
           * if we've truncated this last step.  This will likely occur after
           * EVERY set of dt time steps. */
          dt_step_next = dt_step;
        }
      }

      // We are now finished, so return
      dt_step = dt_step_next;
    }


    template < typename DxDt, typename RKTweak >
    template < unsigned int ndim >
    void RK<DxDt,5u,RKTweak>::rkqs(       Vector<double,ndim> & p,
                                    const Vector<double,ndim> & dpdt,
                                          double & t,
                                          double & dt_try,
                                    const Vector<double,ndim> & p_scal,
                                          double & dt_did,
                                    const double & tf ) {

      using xylose::fast_pow;
      using std::max;
      using std::abs;

      static const double SAFETY = 0.9,
                          PGROW = -.2,
                          PSHRNK = -.25,
                          ERRCON = 1.89e-4;

      // The value ERRCON equals (5/SAFETY)**(1/PGROW), see use below.
      double dt=dt_try;   // Set stepsize to the initial trial value.
      Vector<double,ndim> p_tmp, p_err;

      double maxerr = 0.0;
      while (true) {
        rkck(p,dpdt,t,dt,p_tmp,p_err); //   Take a step.
        ++number_tries;

        //  Evaluate accuracy.
        Vector<double,ndim> frac = compDiv(p_err, p_scal);
        maxerr = max( frac.save_fabs() ) / this->errmax;

        if ( maxerr <= 1.0 )
          break; /* FINISHED -- EXIT THE WHILE LOOP */

        // Truncation error too large, reduce stepsize.
        double dt_temp = SAFETY * dt * fast_pow(maxerr, PSHRNK);
        dt = copysign( max(abs(dt_temp), 0.1*abs(dt) ), dt);    // No more than a factor of 10.
        // now set the absolute time
        double tnew = t + dt;
        if ( tnew == t ) {
          std::ostringstream ostr;
          ostr << "stepsize (" << tnew << " = " << t << " + " << dt << ") underflow in rkqs at:\n"
               << "trying to reach t_f = " << tf << '\n'
               << "p[0:n]:  " << p;

          throw std::runtime_error( ostr.str() );
        }
        ++number_retries;
      }/* while */

      // Step succeeded. Compute size of next step.
      if ( maxerr > ERRCON )
        dt_try = SAFETY * dt * fast_pow(maxerr, PGROW);
      else  // No more than a factor of 5 increase.
        dt_try = 5.0 * dt;

      dt_did = dt;
      t = t + dt; // set the absolute time
      p = p_tmp;
    }


    template < typename DxDt, typename RKTweak >
    template < unsigned int ndim >
    void RK<DxDt,5u,RKTweak>::rkck( const Vector<double,ndim> & p,
                                    const Vector<double,ndim> & D1,
                                    const double & t,
                                    const double & dt,
                                          Vector<double,ndim> & p_out,
                                          Vector<double,ndim> & p_err ) {
      // Cash-Karp parameters for embedded Runge-Kutta
      // a[i] are sub-time step fractions
      // b[i,j] are the jth weights for computing the ith derivative (D[i])
      // c[i] are the weights of D[i] for calculating the p[i](t+dt)
      // dc[i] = c[i] - c*[i]
      static const double
                 a2=0.2,           a3=0.3,        a4=0.6,          a5=1.0,             a6=0.875,

                 b21=.2,
                 b31=3./40.,       b32=9./40.,
                 b41=.3,           b42=-.9,       b43=1.2,
                 b51=-11./54.,     b52=2.5,       b53=-70./27.,    b54=35./27.,
                 b61=1631./55296., b62=175./512., b63=575./13824., b64=44275./110592., b65=253./4096.,

                 c1=37./378.,      c3=250./621.,  c4=125./594.,    c6=512./1771.,

                 dc1 = c1 - 2825./27648.,
                 dc3 = c3 - 18575./48384.,
                 dc4 = c4 - 13525./55296.,
                 dc5 =    - 277./14336.,
                 dc6 = c6 - 0.25;

      Vector<double,ndim> p_tmp, D2, D3, D4, D5, D6;

      // Note:  The placement of dt* within or without parans is to minimize the
      // number of multiplications for n > 1.

      // D1 is given as input

      // find D2
      p_tmp = p + ( dt*b21 ) * D1;
      derivs(p_tmp, t + a2*dt, dt, D2);

      // find D3
      if ( ndim >= 3u )
        p_tmp = p + (dt*b31)*D1 + (dt*b32)*D2;
      else
        p_tmp = p + dt * ( b31*D1 + b32*D2 );
      derivs(p_tmp, t + a3*dt, dt, D3);

      // find D4
      if ( ndim >= 4u )
        p_tmp = p + (dt*b41)*D1 + (dt*b42)*D2 + (dt*b43)*D3;
      else
        p_tmp = p + dt*( b41*D1 + b42*D2 + b43*D3 );
      derivs(p_tmp, t + a4*dt, dt, D4);

      // find D5
      if ( ndim >= 5u )
        p_tmp = p + (dt*b51)*D1 + (dt*b52)*D2 + (dt*b53)*D3 + (dt*b54)*D4;
      else
        p_tmp = p + dt * ( b51*D1 + b52*D2 + b53*D3 + b54*D4 );
      derivs(p_tmp, t + a5*dt, dt, D5);

      // find D6
      if ( ndim >= 6u )
        p_tmp = p + (dt*b61)*D1 + (dt*b62)*D2 + (dt*b63)*D3 + (dt*b64)*D4 + (dt*b65)*D5;
      else
        p_tmp = p + dt * ( b61*D1 + b62*D2 + b63*D3 + b64*D4 + b65*D5);
      derivs(p_tmp, t + a6*dt, dt, D6);

      // Accumulate increments with proper weights.
      if ( ndim >= 5u )
        p_out  = p + (dt*c1)*D1 + (dt*c3)*D3 + (dt*c4)*D4 + (dt*c6)*D6;
      else
        p_out  = p + dt * ( c1*D1 + c3*D3 + c4*D4 + c6*D6 );

      // Estimate error as difference between 5th and embedded 4th order methods
      if ( ndim >= 6u )
        p_err  = (dt*dc1)*D1 + (dt*dc3)*D3 + (dt*dc4)*D4 + (dt*dc5)*D5 + (dt*dc6)*D6;
      else
        p_err  = dt * ( dc1*D1 + dc3*D3 + dc4*D4 + dc5*D5 + dc6*D6 );
    }

  }/* namespace xylose::integrate */
}/* namespace xylose */

