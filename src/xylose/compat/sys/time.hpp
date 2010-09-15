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
 * Windows compatibility layer for <sys/time.h>.
 */

#ifndef xylose_compat_sys_time_hpp
#define xylose_compat_sys_time_hpp

#ifndef WIN32
#  include <sys/time.h>
#else

#include <xylose/compat/config.hpp>

#if defined(USE_WINSOCK)
// you will also have to link to ws2_32.lib
#  include <winsock2.h> // for gethostname
#endif

#include <windows.h>


#include <ctime>

namespace xylose {
  namespace compat {

    #ifndef USE_WINSOCK
    /** timeval defined when POSIX not available. */
    struct timeval {
      /** seconds */
      time_t tv_sec;
      /** microseconds.  Type is supposed to support [-1, 1000000] */
      int tv_usec;
    };
    #endif

    /** timezone defined when POSIX not available.  */
    struct timezone {
      int tz_minuteswest;     /* minutes west of Greenwich */
      int tz_dsttime;         /* type of DST correction */
    };


    struct GetTimeOfDay {
      /* STORAGE MEMBERS */
      LARGE_INTEGER offset;
      double frequencyToMicroseconds;
      bool usePerformanceCounter;


      /* MEMBER FUNCTIONS */
      GetTimeOfDay();

      /** gettimeofday implementation on Windows.  Note that the timezone
       * structure is ignored currently. */
      int operator() (timeval *tv, timezone *tz) {
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
        t.QuadPart    = static_cast<LONGLONG>(microseconds);
        tv->tv_sec    = static_cast<long>(t.QuadPart / 1000000);
        tv->tv_usec   = static_cast<long>(t.QuadPart % 1000000);
        return 0;
      }
    };


    /** The replacement function on windows. */
    inline int gettimeofday( timeval * tv, timezone * tz ) {
      static xylose::compat::GetTimeOfDay gtd = xylose::compat::GetTimeOfDay();
      return gtd(tv,tz);
    }

  }/* namespace xylose::compat */
}/* namespace xylose */

/* import xylose::compat into :: namespace. */
using namespace xylose::compat;

#endif // WIN32

#endif // xylose_compat_sys_time_hpp
