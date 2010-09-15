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
 * Windows compatibility layer for <sys/times.h>.
 */

#ifndef xylose_compat_sys_times_hpp
#define xylose_compat_sys_times_hpp

#ifndef WIN32
#  include <sys/times.h>
#else

#include <windows.h>
#include <ctime>

namespace xylose {
  namespace compat {

    struct tms {
      clock_t tms_utime; /* user time */
      clock_t tms_stime; /* system time */
      clock_t tms_cutime; /* user time of children */
      clock_t tms_cstime; /* system time of children */
    };

    /** Return the CPU time used by this process.  In this semi-fake
     * implementation for windows, only the user time > 0.  All other times are
     * set to zero. */
    inline clock_t times( struct tms * t ) {
      clock_t result = std::clock();
      if (t) {
        t->tms_stime = t->tms_cutime = t->tms_cstime = 0;
        t->tms_utime = result;
      }

      return result;
    }

  }/* namespace xylose::compat */
}/* namespace xylose */

/* import xylose::compat into :: namespace. */
using namespace xylose::compat;

#endif // WIN32

#endif // xylose_detail_compat_sys_times_hpp
