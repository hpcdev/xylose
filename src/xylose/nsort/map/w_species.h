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




#ifndef xylose_nsort_map_w_species_h
#define xylose_nsort_map_w_species_h


namespace xylose {
  namespace nsort {
    namespace map {
      namespace tag {
        struct w_species {};
      }

      /** Wraps a map (may be an aggregate) inside a species map such that the
       * species map creates the slowest moving index.  
       *
       * The species information will be attempted to be retrieved via a
       * required Argument-Dependent Lookup ADL function called species that
       * takes a single argument of the const Particle &. 
       * */
      template <class T>
      struct w_species : T {
        typedef map::tag::w_species tag;
        typedef T super;
        const unsigned int n_species;

        w_species(const unsigned int & n_species) : n_species(n_species) {}
        template <class TT> w_species(const TT & tt) : T(tt) {}

        inline int getNumberValues() const {
          return T::getNumberValues() * n_species;
        }

        /** Map operation used when performing sorting. */
        template<class Particle>
        inline int operator()(const Particle & p) const {
          return n_species * T::operator()(p) + species(p);
        }

      };/*struct w_species */
    }/*namespace map */
  }/*namespace nsort */
}/*namespace xylose */


#endif // xylose_nsort_map_w_species_h

