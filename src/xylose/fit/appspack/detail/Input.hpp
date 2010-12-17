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

#ifndef xylose_fit_appspack_detail_Input_hpp
#define xylose_fit_appspack_detail_Input_hpp

#include <xylose/xml/vector_parse.h>

#include <appspack/APPSPACK_Float.hpp> /* we get dne() from here */

#include <map>
#include <string>
#include <iostream>

namespace xylose {
  namespace fit {
    namespace appspack {
      namespace detail {

        using APPSPACK::dne;

        namespace parameter {

          struct Details {
            double lower;
            double initial;
            double upper;
            double scaling;
            Details( const double & l = 0.0,
                     const double & i = 0.0,
                     const double & u = 0.0,
                     const double & s = 0.0 )
              : lower(l), initial(i), upper(u), scaling(s) { }
          };

          struct Parameter : std::pair< std::string, Details > { };
          struct Map : std::map< std::string, Details > { };

          inline void parse_item( Parameter & p, const xml::Context & x ) {
            p.first = x.query< std::string >("@name");
            p.second = Details(
                         x.query< double >("Lower", dne()),
                         x.query< double >("Initial"),
                         x.query< double >("Upper", dne()),
                         x.query< double >("Scaling", 1.0)
                       );
          }

          inline void parse_item( Map & m, const xml::Context & x ) {
            xml::Context::list xl = x.eval("Param");
            for ( xml::Context::list::iterator i = xl.begin(),
                                             end = xl.end();
                                              i != end; ++i ) {
              std::pair< Map::iterator, bool > ret
                = m.insert( i->parse< Parameter >() );
              if ( !ret.second )
                std::cerr << "WARNING:  duplicate parameter "
                             "'" << ret.first->first << "' specified"
                          << std::endl;
            }
          }

        } /* namespace parameter */


        namespace constraint {

          struct FactorMap : std::map< std::string, double > { };
          struct Factor : std::pair< std::string, double > { };

          inline void parse_item( Factor & f, const xml::Context & x ) {
            f.first  = x.query< std::string >("@param");
            f.second = x.parse< double >();
          }


          struct Equation {
            double lower;
            FactorMap factors;
            double upper;
          };

          struct Vector : std::vector< Equation > { };

          inline void parse_item( Equation & eq, const xml::Context & x ) {
            eq.lower = x.query< double >("Lower", dne());
            eq.upper = x.query< double >("Upper", dne());

            xml::Context::list xl = x.eval("Multiply");
            for ( xml::Context::list::iterator i = xl.begin(),
                                             end = xl.end();
                                              i != end; ++i ) {
              std::pair< FactorMap::iterator, bool > ret
                = eq.factors.insert( i->parse< Factor >() );
              if ( !ret.second )
                std::cerr << "WARNING:  duplicate constraint factor for "
                             "'" << ret.first->first << "' specified"
                          << std::endl;
            }
          }

          inline void parse_item( Vector & v, const xml::Context & x ) {
            xml::Context::list xl = x.eval("Eq");
            for ( xml::Context::list::iterator i = xl.begin(),
                                             end = xl.end();
                                              i != end; ++i ) {
              v.push_back( i->parse< Equation >() );
            }
          }

          inline std::vector<double> makeRow( const parameter::Map & p,
                                              const FactorMap & f ) {
            std::vector<double> retval;
            for (parameter::Map::const_iterator i = p.begin(); i != p.end(); ++i){
              FactorMap::const_iterator fi = f.find( i->first );
              if ( fi == f.end() )
                retval.push_back(0.0);
              else
                retval.push_back( fi->second );
            }

            return retval;
          }

        } /* namespace constraint */

      } /* namespace xylose::appspack::detail */
    } /* namespace xylose::appspack */
  } /* namespace xylose::appspack::fit */
} /* namespace xylose */

#endif // xylose_fit_appspack_detail_Input_hpp
