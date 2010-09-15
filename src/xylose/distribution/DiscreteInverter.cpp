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

#include <xylose/distribution/detail/MultiplyFirst.h>

#include <vector>
#include <utility>
#include <algorithm>

namespace xylose {
  namespace distribution {

    template < typename RNG, bool B >
    RNG DiscreteInverter<RNG,B>::global_RNG;


    template < typename RNG, bool B >
    template < typename PairIter >
    void DiscreteInverter<RNG,B>::resetProbabilityMap( const PairIter & begin,
                                                     const PairIter & end ) {
      /** Integral of P(v):  Int[P(v), {v, 0, inf}]. */
      double IntP_total = 0.0;
      /** The integral of P(v):  { Int[P(v),{v,0,v'}], v' }. */
      std::vector< std::pair<double,double> > IntP;

      for ( PairIter i = begin; i != end; ++i ) {
        if ( i->second <= 0.0 )
          /* we skip anthing that has zero probabilty. */
          continue;

        IntP_total += i->second;
        IntP.push_back( std::make_pair( IntP_total, i->first ) );
      }

      /* normalize the integral and store the probability map. */
      using detail::MultiplyFirst;
      std::for_each(IntP.begin(), IntP.end(), MultiplyFirst(1./IntP_total));
      prob_map = std::map<double,double>(IntP.begin(), IntP.end());
    }


  }/* namespace xylose::distribution */
}/* namespace xylose */

