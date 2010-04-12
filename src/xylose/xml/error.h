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



#ifndef xylose_xml_error_h
#define xylose_xml_error_h

#include <string>
#include <stdexcept>

namespace xylose {
  namespace xml {

    /** The error exception class for this little XML library wrapper. */
    struct error : std::runtime_error {
      typedef std::runtime_error super;
      error(const std::string & s) : super(s) {}
    };

    struct nonsingle_result_error : error {
      nonsingle_result_error(const std::string & s) : error(s) {}
    };

    struct no_results : nonsingle_result_error {
      no_results(const std::string & s) : nonsingle_result_error(s) {}
    };

    struct too_many_results : nonsingle_result_error {
      too_many_results(const std::string & s) : nonsingle_result_error(s) {}
    };

  }/* namespace xylose::xml */
}/* namespace xylose */

#endif // xylose_xml_error_h
