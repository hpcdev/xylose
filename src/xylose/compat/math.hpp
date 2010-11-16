/*==============================================================================
 * Public Domain Contributions 2010 United States Government                   *
 * as represented by the U.S. Air Force Research Laboratory.                   *
 * Portions copyright Copyright (C) 2010 Stellar Science                       *
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
 * Windows compatibility layer for <math.h> ( expm1, lgamma, cbrt).
 */

#ifndef xylose_compat_math_hpp
#define xylose_compat_math_hpp

#ifndef WIN32

  #ifndef __cplusplus
    #include <math.h>
  #else
    #include <cmath>
  #endif

#else

  #ifndef _USE_MATH_DEFINES
    #define _USE_MATH_DEFINES
  #endif

  #ifndef __cplusplus
    #include <math.h>
    double expm1(double);
    double lgamma(double);
    double cbrt(double);
  #else



//--------- C++ -----------
#include <cmath>
namespace xylose {
  namespace compat {

    inline double expm1( const double & d ) {
      const double sxth = 1./6;
      const double tw4th = 1./24;
      if (std::abs(d) < 1e-4 )
        return (d + 0.5*d*d + sxth*d*d*d + tw4th*d*d*d*d);
      else
        return std::exp(d) - 1.0;
    }

    inline double lgamma( const double & x ) {
      double cof[] = {
        76.18009172947146,
        -86.50532032941677,
        24.01409824083091,
        -1.231739572450155,
        0.1208650973866179e-2,
        -0.5395239384953e-5
      };

      double y = x;
      double tmp = x + 5.5;
      tmp -= ((x + 0.5) * std::log(tmp));
      double ser = 1.000000000190015;

      for ( int j = 0; j < 6; ++j ) {
        y += 1;
        ser += cof[j] / y;
      }
      return -tmp + std::log(2.5066282746310005*ser / x);
    }

    inline double cbrt(const double & x) {
      return std::pow(x,1./3.);
    }

  }/* namespace xylose::compat */
}/* namespace xylose::compat */


/* import xylose::compat into :: */
using namespace xylose::compat;

#endif // C++

#endif // WIN32

#endif // xylose_compat_math_hpp
