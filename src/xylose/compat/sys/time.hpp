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

#include <windows.h>
#include <ctime>

namespace xylose {
  namespace compat {

    struct GetTimeOfDay {
      /* STORAGE MEMBERS */
      LARGE_INTEGER offset;
      double frequencyToMicroseconds;
      bool usePerformanceCounter;


      /* MEMBER FUNCTIONS */
      GetTimeOfDay();

      int operator() (timeval *tv, void *tz) {
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
        tv->tv_sec    = t.QuadPart / 1000000;
        tv->tv_usec   = t.QuadPart % 1000000;
        return 0;
      }
    };


    /** The replacement function on windows. */
    inline int gettimeofday( timeval * tv, void * tz ) {
      static xylose::detail::GetTimeOfDay gtd = xylose::detail::GetTimeOfDay();
      return gtd(tv,NULL);
    }

  }/* namespace xylose::compat */
}/* namespace xylose */

/* import xylose::compat into :: namespace. */
using namespace xylose::compat;

#endif // WIN32

#endif // xylose_detail_compat_sys_time_hpp
