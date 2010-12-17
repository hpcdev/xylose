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

#ifndef xylose_fit_appspack_Input_hpp
#define xylose_fit_appspack_Input_hpp

#include <xylose/fit/appspack/detail/Input.hpp>

#include <xylose/xml/Doc.h>

#include <appspack/APPSPACK_Vector.hpp>
#include <appspack/APPSPACK_Matrix.hpp>

#include <map>
#include <string>
#include <iostream>
#include <cassert>

namespace xylose {
  namespace fit {
    namespace appspack {

      struct PackedParameters {
        APPSPACK::Vector lower;
        APPSPACK::Vector initial;
        APPSPACK::Vector upper;
        APPSPACK::Vector scaling;
      };

      struct PackedConstraints {
        APPSPACK::Vector lower;
        APPSPACK::Matrix m;
        APPSPACK::Vector upper;
      };

      class Input {
        /* MEMBER STORAGE */
        std::string probFile;
        detail::parameter::Map parameters;
        detail::constraint::Vector constraints;
        double initialStep;
        double stepTolerance;
        double cacheComparisonTolerance;
        double boundsTolerance;
        int debug;
        std::string cache_output;
        std::string cache_input;


        /* MEMBER FUNCTIONS */
      public:
        Input( const std::string & filename ) {
          xml::Doc db( filename );
          probFile = db.query< std::string >("/APPSPack/Minimize");
          parameters = db.query<detail::parameter::Map>("/APPSPack/Parameters");
          constraints
            = db.query<detail::constraint::Vector>("/APPSPack/Constraints");

          initialStep = db.query<double>("/APPSPack/Solver/InitialStep", 0.0);
          stepTolerance = db.query<double>("/APPSPack/Solver/StepTolerance", 0.0);
          cacheComparisonTolerance =
            db.query<double>("/APPSPack/Solver/CacheComparisonTolerance", 0.0);
          boundsTolerance = db.query<double>("/APPSPack/Solver/BoundsTolerance", 0.0);
          debug = db.query<int>("/APPSPack/Solver/Debug", 3);
          cache_output = db.query<std::string>("/APPSPack/Solver/CacheOutput","");
          cache_input = db.query<std::string>("/APPSPack/Solver/CacheInput","");
        }

        const int & getDebug() const { return debug; }
        const std::string & getCacheOutput() const { return cache_output; }
        const std::string & getCacheInput() const { return cache_input; }
        const std::string & getProbFile() const { return probFile; }
        const double & getInitialStep() const { return initialStep; }
        const double & getStepTolerance() const { return stepTolerance; }
        const double & getCacheComparisonTolerance() const { return cacheComparisonTolerance; }
        const double & getBoundsTolerance() const { return boundsTolerance; }

        PackedParameters packParameters() const {
          PackedParameters retval;
          for ( detail::parameter::Map::const_iterator i = parameters.begin(),
                                                     end = parameters.end();
                                                      i != end; ++i ) {
            retval.lower.push_back( i->second.lower );
            retval.initial.push_back( i->second.initial );
            retval.upper.push_back( i->second.upper );
            retval.scaling.push_back( i->second.scaling );
          }
          assert( retval.lower.size() == retval.upper.size()   &&
                  retval.lower.size() == retval.initial.size() &&
                  retval.lower.size() == retval.scaling.size() );
          return retval;
        }

        PackedConstraints packConstraints() const {
          PackedConstraints retval;
          typedef detail::constraint::Vector::const_iterator CIter;
          for ( CIter i = constraints.begin(); i != constraints.end(); ++i ) {
            retval.lower.push_back( i->lower );
            retval.upper.push_back( i->upper );

            retval.m.addRow( makeRow( parameters, i->factors ) );
          }
          assert( retval.lower.size() == retval.upper.size() &&
                  static_cast<int>(retval.lower.size()) == retval.m.getNrows()&&
                  static_cast<int>(parameters.size())   == retval.m.getNcols());
          return retval;
        }
      };

    } /* namespace xylose::appspack */
  } /* namespace xylose::fit */
} /* namespace xylose */

#endif // xylose_fit_appspack_Input_hpp
