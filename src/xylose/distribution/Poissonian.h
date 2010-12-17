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
 * Poissonian distribtuion functor implementation.
 */

#ifndef xylose_distribution_Poissonian_h
#define xylose_distribution_Poissonian_h

#include <xylose/compat/math.hpp>

namespace xylose {
  namespace distribution {

    /** Poissonian distribution for use.  */
    struct Poissonian {
      double lambda;    /**< Center/mean of distribution. */
      double ln_lambda; /**< ln(lambda). */

      /** Contructor for gaussian distribution.
       * @param b
       *    Set \f$ \beta \f$ for the distribution defined by 
       *    \f$ {\rm e}^{-\beta v^{2}} \f$.
       * @param offs
       *    the center of the Poissonian distribution.
       */
      Poissonian( const double & lambda )
        : lambda( lambda ), ln_lambda( std::log(lambda) ) { }

      /** Evaluate
       \f$
        f(k; \lambda) = \frac{ e^{-\lambda} \lambda^{k} } { \Gamma(k+1) }
       \f$.
       * @param k
       *     Independent variable.
       */
      double operator() ( const double & k ) const {
        using std::exp; using ::lgamma;
        if      ( lambda == 0.0 )
          return k != 0.0 ? 0.0 : 1.0;
        else if ( k < 0.0 )
          return 0.0;
        else
          return exp( k * ln_lambda - lambda - lgamma( k + 1 ) );
      }
    };

  }/* namespace xylose::distribution */
}/* namespace xylose */

#endif // xylose_distribution_Poissonian_h
