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

#include <xylose/compat/math.hpp>

#ifdef WIN32

double expm1(double d) {
  const double sxth = 1./6;
  const double tw4th = 1./24;
  if (fabs(d) < 1e-4 )
    return (d + 0.5*d*d + sxth*d*d*d + tw4th*d*d*d*d);
  else
    return exp(d) - 1.0;
}

double lgamma(double x) {
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
  double ser = 1.000000000190015;
  int j = 0;
  tmp -= ((x + 0.5) * log(tmp));

  for ( j = 0; j < 6; ++j ) {
    y += 1;
    ser += cof[j] / y;
  }
  return -tmp + log(2.5066282746310005*ser / x);
}

double cbrt(double x) {
  return pow(x,1./3.);
}

#else
/* shut the compiler up and define something... */
double xylose_dummy_function() {
  return 1.0;
}
#endif // WIN32
