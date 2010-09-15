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

#ifdef WIN32

#include <xylose/detail/windblows.h>


namespace {

  /* a replacement for gettimeofday on windows. */
  LARGE_INTEGER getFILETIMEoffset() {
    SYSTEMTIME s;
    FILETIME f;
    LARGE_INTEGER t;

    s.wYear = 1970;
    s.wMonth = 1;
    s.wDay = 1;
    s.wHour = 0;
    s.wMinute = 0;
    s.wSecond = 0;
    s.wMilliseconds = 0;
    SystemTimeToFileTime(&s, &f);
    t.QuadPart = f.dwHighDateTime;
    t.QuadPart <<= 32;
    t.QuadPart |= f.dwLowDateTime;
    return t;
  }

}

namespace xylose {
  namespace detail {


      GetTimeOfDay::GetTimeOfDay() {
        LARGE_INTEGER performanceFrequency;
        usePerformanceCounter
          = static_cast<bool>(QueryPerformanceFrequency(&performanceFrequency));

        if (usePerformanceCounter) {
          QueryPerformanceCounter(&offset);
          frequencyToMicroseconds
            = static_cast<double>(performanceFrequency.QuadPart / 1000000.);
        } else {
          offset = getFILETIMEoffset();
          frequencyToMicroseconds = 10.;
        }
      }


  }
}

#endif // WIN32
