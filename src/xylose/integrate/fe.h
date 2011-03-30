/*==============================================================================
 * Public Domain Contributions 2010 United States Government                   *
 * as represented by the U.S. Air Force Research Laboratory.                   *
 *                                                                             *
 * This file is part of xylose                                                 *
 *                                                                             *
 * This program is free software: you can redistribute it and/or modify it     *
 * under the terms of the GNU Lesser General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or (at your  *
 * option) any later version.                                                  *
 *                                                                             *
 * This program is distributed in the hope that it will be useful, but WITHOUT *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
 * License for more details.                                                   *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                             *
 -----------------------------------------------------------------------------*/

/** \file
 * Implementation of forward Euler integration.
 */

#ifndef xylose_integrate_fe_h
#define xylose_integrate_fe_h

#include <xylose/integrate/Integrator.h>
#include <xylose/integrate/detail/Derivs.h>
#include <xylose/Vector.h>

namespace xylose {
  namespace integrate {

    /** Forward Euler integrator. */
    template < typename DerivativeFunctor >
    struct FE : Integrator {
      /* MEMBER STORAGE */
      /** The DerivativeFunctor instance. */
      DerivativeFunctor derivs;


      /* MEMBER FUNCTIONS */
      /** Default constructor. */
      FE() : derivs() { }

      /** Constructor allows DerivativeFunctor to be passed an argument into its
       * constructor. 
       * @param t
       *    An arbitrary argument for the DerivativeFunctor constructor.
       */
      template < typename T >
      FE( const T & t ) : derivs(t) { }

      /** Integrate using forward euler.
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
        Vector<double,ndim> D;
        // find D
        detail::Derivs<Optional>()( derivs, x, t, dt, D, opt );

        x += dt * D;
      }

      /** Integrate using forward euler.
       * This driver computes the integral x(t+dt) = x(t) + Int[f, t, t+dt].
       * @param x
       *      Dependent variables; input x(t), output x(t+dt).
       * @param t
       *      The starting time in the integral.
       * @param dt
       *      The integration length.
       */
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
