/** \file
 * C++ abstract class Runge-Kutta.
 */

#ifndef xylose_integrate_rk_h
#define xylose_integrate_rk_h

#include <xylose/integrate/Integrator.h>
#include <xylose/integrate/detail/NullRKTweak.h>

#include <xylose/Vector.h>

namespace xylose {
  namespace integrate {

    /** Runge-Kutta implementations for various orders. */
    template < typename DerivativeFunctor,
               unsigned int order = 4u,
               typename RKTweak = detail::NullRKTweak >
    struct RK : Integrator {
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

      /** Integrate using Runge-Kutta which yields <code>order</code>th order
       * accuracy.
       * This driver computes the integral x(ti+dt) = x(ti) + Int[f, ti, ti+dt].
       * @param x
       *      Dependent variables; input x(ti), output x(ti+dt).
       * @param ti
       *      The starting time in the integral.
       * @param dt
       *      The integration length.
       * @param dt_step
       *      The sub-integration length to try for the adaptive methods.
       */
      template < unsigned int ndim,
                 typename Optional >
      inline void operator() (       Vector<double,ndim> & x,
                               const double & t,
                               const double & dt,
                                     Optional & opt ) {
        typedef DerivativeFunctor D;
        return D::NO_RUNGE_KUTTA_IMPLEMENTATION
                ::WITH_OPTIONAL
                ::AVAILABLE_FOR_THIS_ORDER;
      }


      template < unsigned int ndim >
      inline void operator() (       Vector<double,ndim> & x,
                               const double   & t,
                               const double   & dt ) {
        typedef DerivativeFunctor D;
        return D::NO_RUNGE_KUTTA_IMPLEMENTATION
                ::WITHOUT_OPTIONAL
                ::AVAILABLE_FOR_THIS_ORDER;
      }
    };

  }/* namespace xylose::integrate */
}/* namespace xylose */

#if !defined( xylose_integrate_rk2_h) && \
    !defined( xylose_integrate_rk4_h) && \
    !defined( xylose_integrate_rk5_h)
  /* We'll only  include these if one wasn't already specifically included. */
#  include <xylose/integrate/rk2.h>
#  include <xylose/integrate/rk4.h>
#  include <xylose/integrate/rk5.h>
#endif

#endif // xylose_integrate_rk_h
