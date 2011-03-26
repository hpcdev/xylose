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

/** \file appsPack.cpp
 * Main code for threaded APPSPack executor binary.  
 */

#include <xylose/fit/appspack/Input.hpp>
#include <xylose/fit/appspack/ThreadedExecutor.hpp> ///< Provides user-defined custom executor
#include <xylose/fit/ExecFunc.hpp>
#ifdef HAVE_PSTREAMS
  #include <xylose/fit/PExecFunc.hpp>
#endif
#include <xylose/XSTR.h>
#include <xylose/PThreadCache.h>

#include <appspack/APPSPACK_Solver.hpp>             ///< Provides APPSPACK::Solver
#include <appspack/APPSPACK_Vector.hpp>             ///< Provides APPSPACK::Vector 
#include <appspack/APPSPACK_Constraints_Linear.hpp> ///< Provides APPSPACK::Constraints::Linear
#include <appspack/APPSPACK_Float.hpp>              ///< Provides APPSPACK::dne()

#include <boost/program_options.hpp>

#include <string>

namespace {
  namespace po = boost::program_options;
  typedef xylose::fit::ExecFunc MinFunc;
#ifdef HAVE_PSTREAMS
  typedef xylose::fit::PExecFunc PMinFunc;
#endif
}

struct options {
  /* MEMBER STORAGE */
  po::options_description descriptions;
  po::variables_map  map;

  options() : descriptions("Allowed Options") {
    descriptions.add_options()
      ("help,h",                                  "Produce help message.")
      ("input-file,i",  po::value<std::string>(), "File to open as input")
    #ifdef HAVE_PSTREAMS
      ("pipe-comm,p", "Communicate to merit function on stdout/stderr instead "
                      "of using input/output files.  ")
    #endif
      ("num-threads,n", po::value<unsigned  int>()->default_value(2u),
                        "Use up to <value> threads.  This can also be "
                        "specified by the environment variable\n"
                        "NUM_PTHREADS")
      ;
  }

  std::ostream & printUsage(std::ostream & out) {
    out <<
      "  Threaded AppsPack Executor Help\n"
      "  ===============================\n"
      "  xylose version : " XSTR(XYLOSE_VERSION) "\n"
      "\n"
      "\n"
      "  usage:  appsPack [options]...\n\n"
        << descriptions;
    return out;
  }

  int parse( const int & argc, char * argv[] ) {
    try {
      po::positional_options_description p;
      po::store(
        po::command_line_parser(argc, argv)
           .options( descriptions )
           .positional( p ) // we don't support positional parameters for now
           .run(),
        map );
      po::notify( map );

      if ( map.count("help") ) {
        printUsage( std::cout ) << std::endl;
        return EXIT_SUCCESS;
      } else
        return -1;
    } catch ( const std::exception & e ) {
      std::cerr << "command line parsing error: " << e.what() << std::endl;
      printUsage( std::cerr ) << std::endl;
      return EXIT_FAILURE;
    } catch(...) {
      std::cerr << "command line parsing error:  Unknown error!" << std::endl;
      printUsage( std::cerr ) << std::endl;
      return EXIT_FAILURE;
    }
  }
};

int main(int argc, char* argv[]) {
  options opts;
  { int ret = opts.parse( argc, argv );
    if ( ret >= 0 )
      return ret;
  }

  if ( opts.map.count("input-file") == 0 ) {
    opts.printUsage(std::cerr << "error:  missing input file\n\n") << std::endl;
    return EXIT_FAILURE;
  }

  xylose::pthreadCache.set_max_threads(
    opts.map["num-threads"].as<unsigned int>()
  );

  const std::string input_file = opts.map["input-file"].as<std::string>();
  xylose::fit::appspack::Input P( input_file );

  /* get the parameter information */
  xylose::fit::appspack::PackedParameters pp = P.packParameters();

  /* get the coupled parameter constraints */
  xylose::fit::appspack::PackedConstraints pc = P.packConstraints();

  // *** Create and fill the parameter list ***
  APPSPACK::Parameter::List params;
  params.sublist("Linear").setParameter("Lower", pp.lower);
  params.sublist("Solver").setParameter("Initial X", pp.initial);
  params.sublist("Linear").setParameter("Upper", pp.upper);
  params.sublist("Linear").setParameter("Scaling", pp.scaling);
  params.sublist("Linear").setParameter("Inequality Lower", pc.lower);
  params.sublist("Linear").setParameter("Inequality Matrix", pc.m);
  params.sublist("Linear").setParameter("Inequality Upper", pc.upper);
  //params.sublist("Solver").setParameter("Initial F", initialf);

  // Be sure to do casts on any ambiguous constants, such as in this case
  if ( P.getDebug() >= 0 )
    params.sublist("Solver").setParameter("Debug", P.getDebug());
  if ( P.getCacheOutput().size() )
    params.sublist("Solver").setParameter("Cache Output File", P.getCacheOutput() );
  if ( P.getCacheInput().size() )
    params.sublist("Solver").setParameter("Cache Input File", P.getCacheInput() );
  if ( P.getInitialStep() > 0.0 )
    params.sublist("Solver").setParameter("Initial Step", P.getInitialStep() );
  if ( P.getStepTolerance() > 0.0 )
    params.sublist("Solver").setParameter("Step Tolerance", P.getStepTolerance() );
  if ( P.getCacheComparisonTolerance() > 0.0 )
    params.sublist("Solver").setParameter("Cache Comparison Tolerance", P.getCacheComparisonTolerance() );
  if ( P.getBoundsTolerance() > 0.0 )
    params.sublist("Solver").setParameter("Bounds Tolerance", P.getBoundsTolerance() );


  // *** Instantiate the bounds ***
  APPSPACK::Constraints::Linear linear(params.sublist("Linear"));

  typedef APPSPACK::Solver<> Solver;
  Solver::State state;

#ifdef HAVE_PSTREAMS
  if ( opts.map.count("pipe-comm") ) {
    // *** Create the MinFunc
    PMinFunc minFunc( P.getProbFile(), input_file );

    // *** Instantiate the custom executor ***
    xylose::fit::appspack::ThreadedExecutor<PMinFunc> executor(minFunc);
    
    // *** Create and run the solver ***
    state = Solver(params.sublist("Solver"), executor, linear).solve();
  } else
#endif
  {
    // *** Create the MinFunc
    MinFunc minFunc( P.getProbFile(), input_file );

    // *** Instantiate the custom executor ***
    xylose::fit::appspack::ThreadedExecutor<MinFunc> executor(minFunc);
    
    // *** Create and run the solver ***
    state = Solver(params.sublist("Solver"), executor, linear).solve();
  }
  
  switch( state ) {
    case Solver::Continue:
      std::cerr << "Finished APPSPack with a continue state?" << std::endl;
      break;
    case Solver::StepConverged:
      std::cerr << "Finished APPSPack with step length converged"
                << std::endl;
      break;
    case Solver::FunctionConverged:
      std::cerr << "Finished APPSPack with function tolerance converged"
                << std::endl;
      break;
    case Solver::EvaluationsExhausted:
      std::cerr << "Finished APPSPack with number of function "
                   "evaluations exhausted" << std::endl;
      break;
    default:
      std::cerr << "Unknown finish APPSPack finish state" << std::endl;
  }
  
  return EXIT_SUCCESS;
}

