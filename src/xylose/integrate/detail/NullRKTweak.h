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

#ifndef xylose_integrate_detail_NullRKTweak_h
#define xylose_integrate_detail_NullRKTweak_h

#include <xylose/Vector.h>

namespace xylose {
  namespace integrate {
    namespace detail {

      /** A NOOP class for RK<DxDt,5u,RkTweak> (5th order adaptive Runge-Kutta).*/
      struct NullRKTweak {
        /** The first rkTweak is called at the beginning of the RK driver. */
        template <unsigned int ndim_>
        inline void rkTweakFirst( Vector<double,ndim_> & x,
                                  const double & t,
                                  const double & dt_step_current,
                                  double & dt_step_next ) {}

        /** The second rkTweak is called at the end of the RK driver. */
        template <unsigned int ndim_>
        inline void rkTweakSecond( Vector<double,ndim_> & x,
                                   const double & t,
                                   const double & dt_step_current,
                                   double & dt_step_next ) {}
      };

    }/* namespace xylose::integrate::detail */
  }/* namespace xylose::integrate */
}/* namespace xylose */

#endif // xylose_integrate_detail_NullRKTweak_h
