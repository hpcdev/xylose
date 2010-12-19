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
 * Generic discrete distribution inverter.
 *
 * This class is optimal for inverting distributions that have discrete data
 * and for which memory efficiency is a requirement.  It should be noted that
 * the generic continuous distribution inverter (the Inverter class in
 * Inverter.h) can handle discrete data sets as long as the sampling rate is
 * high enough--the definition of 'high enough' must be determined by testing.  
 */

/** \example invertdistro/discrete/vector.cpp
 * Example of using the distribution inverter for a one-dimensional
 * discrete distribution that is already loaded into an array. */

/** \example invertdistro/discrete/file.cpp
 * Example of using the distribution inverter for a one-dimensional
 * discrete distribution that is stored in an external ascii text file. */


#ifndef xylose_distribution_DiscreteInverter_h
#define xylose_distribution_DiscreteInverter_h

#include <xylose/random/Kiss.hpp>
#include <xylose/distribution/detail/pair.h>

#include <map>
#include <fstream>
#include <iterator>

namespace xylose {
  namespace distribution {

    /** A generic class to invert arbitrary discrete distributions.
     * The user provides a set of data either stored in memory or in file.  This
     * data need NOT be normalized.  
     *
     * @tparam RNG
     *    The random number generator class that will be used by this
     *    distribution. <br>
     *    [Default xylose::random::Kiss]
     *
     * @tparam operator_returns_discrete
     *    Whether operator() should return value that exactly matches one of the
     *    x-axis input values (i.e. calls discrete_value).  Otherwise, the
     *    values are linearly interpolated between neighboring data points.
     *    Note:  the false case has not been tested extensively nor has it been
     *    proven to be correct.
     *    [Default true]
     *
     * @see DiscreteInverter::DiscreteInverter constructor.
     * @see Inverter class for inverting continuous dataset/analytic function.
     * */
    template < typename RNG = xylose::random::Kiss,
               bool operator_returns_discrete = true >
    class DiscreteInverter {
      /* MEMBER STORAGE */
    private:
      /** \f$ F(v') \circeq \int_0^{v'} P(v) \; dv \rightarrow v' \f$. */
      std::map<double,double> prob_map;

      /** Reference to the random number generator that is used for this
       * distribution. */
      RNG & rng;


      /* STATIC STORAGE */
    private:
      /** The global generator to use for all distributions if a specific
       * reference is not passed in at constructor time. */
      static RNG global_RNG;


      /* MEMBER FUNCTIONS */
    public:

      /** Copy onstructor (copying from a specific probability map). */
      DiscreteInverter( const std::map<double,double> prob_map,
                        RNG & rng = DiscreteInverter::global_RNG )
        : prob_map( prob_map ), rng( rng ) { }

      /** DiscreteInverter constructor, taking data from memory.
       *
       * @param begin
       *    Beginning std::pair<double,double>-iterator of in-memory data.
       * @param end
       *    Ending std::pair<double,double>-iterator of in-memory data.
       * @param rng
       *    A reference to the random number generator that will be used by
       *    this distribution.
       */
      template < typename Iter >
      DiscreteInverter( const Iter & begin,
                        const Iter & end,
                        RNG & rng = DiscreteInverter::global_RNG ) : rng(rng) {
        resetProbabilityMap( begin, end );
      }

      /** DiscreteInverter constructor, taking data from an ascii text file.
       * The file representing the probability distribution P(v)
       * should have two columns, v, P(v).
       *
       * @param filename
       *    Name of file from which to read data.
       * @param rng
       *    A reference to the random number generator that will be used by
       *    this distribution.
       */
      DiscreteInverter( const std::string & filename,
                        RNG & rng = DiscreteInverter::global_RNG ) : rng(rng) {
        typedef std::istream_iterator< detail::pair<double,double> > IIter;
        std::ifstream in(filename.c_str());
        resetProbabilityMap( IIter(in), IIter() );
      }

      /** DiscreteInverter constructor, taking data from stream of ascii text.
       * The data format in the stream (representing probability distribution)
       * P(v) should have two columns, v, P(v).  The stream will be read until
       * end-of-stream.
       *
       * @param in
       *    Stream from which to read data.
       * @param rng
       *    A reference to the random number generator that will be used by
       *    this distribution.
       */
      DiscreteInverter( std::istream & in,
                        RNG & rng = DiscreteInverter::global_RNG ) : rng(rng) {
        typedef std::istream_iterator< detail::pair<double,double> > IIter;
        resetProbabilityMap( IIter(in), IIter() );
      }

      /** Reset the distribution function with a new set of data.
       * Each item in the data set must be a pair like std::make_pair(v, P(v)).
       */
      template < typename PairIter >
      void resetProbabilityMap( const PairIter & begin, const PairIter & end );

      /** Get a random number from this distribution.
       * This calls leverarm(double).  This is now essentially the same function
       * as lever().
       * @see lever().
       * @see lever(double).
       */
      double operator() (void) const {
        if (operator_returns_discrete)
          return discrete_value( rng.rand() );
        else
          return leverarm( rng.rand() );
      }

      /** Get a random number from this distribution.
       * Calls leverarm(double).
       */
      double lever() const {
        return leverarm( rng.rand() );
      }

      /** Get a random number from this distribution.
       * Calls discrete_value.
       */
      double discrete( ) const {
        return discrete_value( rng.rand() );
      }

      /** Sample the inverted distribution:  values returned will be exactly
       * those given in the original distribution (no interpolation done).
       */
      double discrete_value( const double & rf ) const {
        std::map<double,double>::const_iterator i = prob_map.lower_bound( rf );
        if ( i == prob_map.end() )
          return prob_map.rbegin()->second;
        else
          return i->second;
      }

      /** Sample the inverted distribution.
       * No bounds checking occurs here.  If you pass in rf>1 or rf<0, then you
       * will be reading from invalid/unallocated memory.  Therefore, be sure to
       * keep rf in range. 
       * @param rf
       *     A fraction in the range [0,1] (inclusive).
       */
      double leverarm(const double & rf) const {
        typedef std::map<double,double>::const_iterator Iter;

        Iter i = prob_map.upper_bound( rf );
        if ( i == prob_map.begin() )
          /* No extrapolation, just return first item. */
          return i->second;
        else if ( i == prob_map.end() )
          /* No extrapolation, just return the last item. */
          return prob_map.rbegin()->second;
        else {
          /* Interpolate between i and i-1. */
          Iter i0 = i;
          --i0;
          return ( i->second*(rf - i0->first) + i0->second*(i->first - rf) ) /
                              ( i->first - i0->first );
        }
      }

      /** Return a const reference to the probability map. 
       * This map represents:
       * \f[
         F(v') \circeq \int_0^{v'} P(v) \; dv \rightarrow v'
         \f]
       */
      const std::map<double,double> & invertedDistribution() const {
        return prob_map;
      }
    };

  }/* namespace xylose::distribution */
}/* namespace xylose */

#include <xylose/distribution/DiscreteInverter.cpp>

#endif // xylose_distribution_DiscreteInverter_h
