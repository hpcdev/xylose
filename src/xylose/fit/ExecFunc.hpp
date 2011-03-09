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
  \file ExecFunc.hpp

  \brief Execute a function file and obtain the result via FILE communication.  

  \sa \ref pageCustomize_executor 
*/

#ifndef xylose_fit_ExecFunc_hpp
#define xylose_fit_ExecFunc_hpp

#include <xylose/strutil.h>

#include <fstream>
#include <iterator>
#include <limits>
#include <cstdlib>
#include <sstream>

namespace xylose {
  namespace fit {

    struct ExecFunc {
      /* MEMBER STORAGE */
      const std::string program_name;
      const std::string extra_args;

      /* MEMBER FUNCTIONS */
      ExecFunc( const std::string & program_name,
                const std::string & extra_args = "" )
        : program_name( program_name ),
          extra_args( extra_args ) { }

      template < typename V0, typename V1 >
      std::string operator() ( const V0 & x,
                                     V1 & f,
                               const int & tag = 0 ) const {
        /* create temporary input and output filenames. */
        char infile[100]  = "infile.XXXXXX";
        char outfile[100] = "outfile.XXXXXX";
        close(::mkstemp(infile));
        close(::mkstemp(outfile));

        std::string cmd = program_name
                        + ' '
                        + std::string(infile)
                        + ' '
                        + std::string(outfile)
                        + ' '
                        + to_string(tag)
                        + ' '
                        + extra_args;

        { /* write program the inputs values. */
          std::ofstream ifile(infile);
          ifile << x.size() << '\n';
          copy( x.begin(), x.end(), std::ostream_iterator<double>(ifile, "\n") );
          ifile.close();
        }
        
        /* run program */
        int status = std::system(cmd.c_str()) >> 8;

        std::string msg;
        { /* read output. */
          std::ifstream ofile(outfile);
          std::string line;
          std::getline( ofile, line );
          std::istringstream istr( line );
          ofile.close();

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

        /* remove temporary files. */
        unlink(infile);
        unlink(outfile);

        return msg;
      }/* operator() */
    };

  }/* namespace xylose::fit */
}/* namespace xylose */

#endif // xylose_fit_ExecFunc_hpp
