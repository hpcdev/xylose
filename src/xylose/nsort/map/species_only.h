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




#ifndef xylose_nsort_map_species_h
#define xylose_nsort_map_species_h

namespace xylose {
  namespace nsort {
    namespace map {
      namespace tag {
        struct species_only {};
      }

      /** maps particle to particle.species.
       * This sort-map does <b>not</b> dictate that new children are to be created.
       *
       * The species information will be attempted to be retrieved via a
       * required Argument-Dependent Lookup ADL function called species that
       * takes a single argument of the const Particle &. 
       * */
      struct species_only {
        typedef tag::species_only tag;
        typedef void super;

        template<class Particle>
        inline int operator()(const Particle & p) const {
          return species(p);
        }
   
      };/*struct species_only */
    }/*namespace map */
  }/*namespace nsort */
}/*namespace xylose */


#endif // xylose_nsort_map_species_h

