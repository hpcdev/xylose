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

/*! 
  \file PExecFunc.hpp

  \brief Execute a function file and obtain the result via PIPE communication.  

  \sa \ref pageCustomize_executor 
*/

#ifndef xylose_fit_PExecFunc_hpp
#define xylose_fit_PExecFunc_hpp

#include <xylose/strutil.h>

#include <redi/pstream.h>

#include <iterator>
#include <limits>
#include <sstream>

namespace xylose {
  namespace fit {

    struct PExecFunc {
      /* MEMBER STORAGE */
      const std::string program_name;
      const std::string extra_args;

      /* MEMBER FUNCTIONS */
      PExecFunc( const std::string & program_name,
                 const std::string & extra_args = "" )
        : program_name( program_name ),
          extra_args( extra_args ) { }

      template < typename V0, typename V1 >
      std::string operator() ( const V0 & x,
                                     V1 & f,
                               const int & tag = 0,
                               const std::string & extra_args2 = "" ) const {
        redi::pstream prog( program_name
                              + ' ' + to_string(tag)
                              + ' ' + extra_args
                              + ' ' + extra_args2,
                            redi::pstreams::pstdin |
                            redi::pstreams::pstdout );

        /* send program the inputs values. */
        prog << x.size() << std::endl;
        copy( x.begin(), x.end(), std::ostream_iterator<double>(prog, "\n") );
        prog << std::flush;

        std::string msg;
        { /* read output */
          std::string line;
          std::getline( prog, line );
          std::istringstream istr( line );
          /* reap the child program and check its status. */
          prog.close();
          int status = prog.rdbuf()->status() >> 8;

          double val = std::numeric_limits<double>::infinity();
          istr >> val;

          if ( istr.fail() ) {
            f.resize(0);
            msg = line;
          } else if (status != 0) {
            msg = "unknown evaluation failure";
          } else {
            f.resize(1);
            f[0] = val;
            std::getline( istr, msg );
          }
        }

        return msg;
      }/* operator() */
    };

  }/* namespace xylose::fit */
}/* namespace xylose */

#endif // xylose_fit_PExecFunc_hpp
