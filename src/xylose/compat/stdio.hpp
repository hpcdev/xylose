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
 * Windows compatibility layer for <stdio.h> ( popen, pclose ).
 * Note that this only really provides some functions with similar names, but
 * not necessarily functionality. 
 */

#ifndef xylose_compat_stdio_hpp
#define xylose_compat_stdio_hpp

#ifndef WIN32
#  include <stdio.h>
#else


#include <windows.h>

namespace xylose {
  namespace compat {

    inline FILE *popen(const char *cmd, const char *flags) {
    	system(cmd);
    	return (FILE*)NULL;
    }

    inline void pclose(FILE* stream) {}

  }/* namespace xylose::compat */
}/* namespace xylose::compat */


/* import xylose::compat into :: */
using namespace xylose::compat;

#endif // WIN32

#endif // xylose_compat_stdio_hpp
