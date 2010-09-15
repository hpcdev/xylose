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

#ifndef xylose_detail_windblows_h
#define xylose_detail_windblows_h

#ifdef WIN32

#include <windows.h>
#include <ctime>

namespace xylose {
  namespace detail {

    struct timezone {
      int  tz_minuteswest; /* minutes W of Greenwich */
      int  tz_dsttime;     /* type of dst correction */
    };


    struct GetTimeOfDay {
      /* STORAGE MEMBERS */
      LARGE_INTEGER offset;
      double frequencyToMicroseconds;
      bool usePerformanceCounter;


      /* MEMBER FUNCTIONS */
      GetTimeOfDay();

      int operator() (struct timeval *tv, struct timezone *tz) {
        LARGE_INTEGER t;
        if (usePerformanceCounter)
          QueryPerformanceCounter(&t);
        else {
          FILETIME f;
          GetSystemTimeAsFileTime(&f);
          t.QuadPart = f.dwHighDateTime;
          t.QuadPart <<= 32;
          t.QuadPart |= f.dwLowDateTime;
        }

        t.QuadPart   -= offset.QuadPart;
        double microseconds = static_cast<double>(t.QuadPart)
                            / frequencyToMicroseconds;
        t.QuadPart    = microseconds;
        tv->tv_sec    = t.QuadPart / 1000000;
        tv->tv_usec   = t.QuadPart % 1000000;
        return 0;
      }
    };

  }/* namespace xylose::detail */
}/* namespace xylose */


/* import this into :: namespace. */
using xylose::detail::timezone;

/** The replacement function on windows. */
inline gettimeofday( struct timeval * tv, struct timezone * tz ) {
  static GetTimeOfDay gtd;
  return gtd(tv,tz);
}


inline times( struct tms * t ) {
  t->tms_utime = std::clock() * CLOCKS_PER_SEC / seconds_per_clock_tick;
}

#endif // WIN32

#endif // xylose_detail_windblows_h
