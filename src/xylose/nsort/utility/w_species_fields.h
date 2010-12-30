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


#ifndef xylose_nsort_utility_w_species_fields_h
#define xylose_nsort_utility_w_species_fields_h


#include <xylose/IteratorRange.h>

#include <vector>

namespace xylose {
  namespace nsort {
    namespace utility {

      /** IteratorRanges set from the NSort class using the w_species wrapper map.
       * @tparam ParticleIterator
       *    The type of iterator to use in the IteratorRange class.
       *
       * @tparam _n_species
       *    Specify the number of compile time species to allow.  If this
       *    parameter is set to zero (0), the number of species will be
       *    configured at runtime instead of at compile time.
       */
      template< typename ParticleIterator, unsigned int _n_species = 0u >
      struct w_species_fields;


      /** Specialization of the w_species_fields class where the number of
       * species is set at compile time to >1. */
      template< typename ParticleIterator, unsigned int _n_species >
      struct w_species_fields {
        /* TYPEDEFS */
        typedef xylose::IteratorRange<ParticleIterator> Range;
        static const unsigned int n_species = _n_species;

        /* MEMBER STORAGE */
        Range species[n_species];

        /* MEMBER FUNCTIONS */
        /** NO-OP. */
        w_species_fields() {}

        template < typename NSort >
        w_species_fields( const ParticleIterator & begin,
                          const NSort & s,
                          const int & s_i ) {
          setSpeciesRanges( begin, s, s_i );
        }

        /** Sets each of the ranges for the species.  This function is only
         * called for Octree leaf nodes. */
        template < typename NSort >
        void setSpeciesRanges( const ParticleIterator & begin,
                               const NSort & s,
                               const int & s_i ) {
          for ( int i = 0; i < static_cast<int>(n_species); ++i ) {
            int s_ii = s_i + i;
            species[i] = Range( begin + s.begin(s_ii), begin + s.end(s_ii) ); 
          }
        }
      };


      /** Non-type sorting fields (only provides static const n_species (==1). */
      template< typename ParticleIterator >
      struct w_species_fields<ParticleIterator, 1u> {
        /* TYPEDEFS */
        static const unsigned int n_species = 1u;

        /** NO-OP. */
        w_species_fields() {}

        /** NO-OP. */
        template < typename NSort >
        w_species_fields( const ParticleIterator & begin,
                          const NSort & s,
                          const int & s_i ) {}
        /** NO-OP. */
        template < typename NSort >
        void setSpeciesRanges( const ParticleIterator & begin,
                               const NSort & s,
                               const int & s_i ) {}
      };


      /** Specialization of the w_species_fields class that allows the number of
       * species to be specified at runtime. */
      template< typename ParticleIterator >
      struct w_species_fields<ParticleIterator, 0u> {
        /* TYPEDEFS */
        typedef xylose::IteratorRange<ParticleIterator> Range;

        /* MEMBER STORAGE */
        std::vector< Range > species;

        /* MEMBER FUNCTIONS */
        /** NO-OP. */
        w_species_fields() {}

        template < typename NSort >
        w_species_fields( const ParticleIterator & begin,
                          const NSort & s,
                          const int & s_i,
                          const unsigned int & n_species ) {
          setSpeciesRanges( begin, s, s_i, n_species );
        }

        /** Sets each of the ranges for the species.  This function is only
         * called for Octree leaf nodes. */
        template < typename NSort >
        void setSpeciesRanges( const ParticleIterator & begin,
                               const NSort & s,
                               const int & s_i,
                               const unsigned int & n_species ) {
          species.clear();
          species.reserve( n_species );
          for ( int i = 0; i < static_cast<int>(n_species); ++i ) {
            int s_ii = s_i + i;
            species.push_back(
              Range( begin + s.begin(s_ii), begin + s.end(s_ii) )
            );
          }
        }
      };

    }/* namespace xylose::nsort::utility */
  }/* namespace xylose::nsort */
}/* namespace xylose */

#endif // xylose_nsort_utility_w_species_fields_h
