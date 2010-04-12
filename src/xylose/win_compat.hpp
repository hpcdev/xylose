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


#ifndef _WINCOMPAT_HPP_
#define _WINCOMPAT_HPP_
#include <cmath>
#include <cassert>

#ifdef WIN32

inline int strncasecmp(const char *c1, const char *c2, size_t len) {
	return _strnicmp(c1,c2,len);
}
inline int strcasecmp(const char *c1, const char *c2){
	return _stricmp(c1,c2);
}

inline double expm1( const double d) {
  const double sxth = 1./6;
  const double tw4th = 1./24;
	if (fabs(d) < 1e-4 )
		return (d + 0.5*d*d + sxth*d*d*d + tw4th*d*d*d*d);
	else
		return (exp(d)-1);
}


inline double lgamma(double x) {
  double cof[] =   {
  76.18009172947146, -86.50532032941677,
  24.01409824083091,-1.231739572450155,
  0.1208650973866179e-2,-0.5395239384953e-5};
  double y = x;
  double tmp = x + 5.5;
  tmp -= ((x + 0.5) * log(tmp));
  double ser = 1.000000000190015;
  int j = 0;
  while(j < 6) {
    y += 1;
    ser += (cof[j]/y);
    j +=1;
  }
  return (-tmp + log(2.5066282746310005*ser / x));
}

inline double cbrt(double x) {
	return std::pow(x,1./3.);
}


inline void sleep(int t) {
	_sleep(t*1000);
}


inline FILE *popen(const char *cmd, const char *flags) {
	system(cmd);
	return (FILE*)NULL;
}
inline void pclose(FILE* stream) {}

inline void gethostname(char*name, const int len) {
	strncat(name,"WindozeBox",std::min(10,len-1));
}
inline int getpid() {return -1;}
#endif

#endif
