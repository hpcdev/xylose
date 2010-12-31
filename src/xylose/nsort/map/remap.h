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



#ifndef xylose_nsort_map_remap_h
#define xylose_nsort_map_remap_h

#include <set>

namespace xylose {
  namespace nsort {
    namespace map {
      namespace tag {
        template < typename T >
        struct remap {};
      }

      /** Re-maps an underlying map function onto a different set of mapped
       * values.
       * An example use of this class is to remap a portion of the domain of the
       * underlying map function onto another portion of the map function's
       * domain.
       * */
      template < typename T,
                 unsigned int _nval = T::number_values >
      struct remap : T {
        typedef map::tag::remap<T> tag;
        typedef T super;

        /** Maximum number of possible values. */
        static const unsigned int number_values = _nval;

        /* STORAGE MEMBER(S) */

        /** The remap map. */
        int m_remap[number_values];
      

        /* FUNCTION MEMBERS */
        /** Default constructor calls 'reset'.
         * @see reset. */
        remap() : super() {
          reset();
        }

        /** Constructor passes to super as required by wrapping components.
         * Default constructor calls 'reset'.
         * @see reset.
         */
        template <class TT>
        remap(const TT & tt) : super(tt) {
          reset();
        }

        /** Resets the m_remap[] array to 1..nval. */
        void reset() {
          for ( unsigned int i = 0; i < number_values; ++i )
            m_remap[i] = i;
        }

        /** Returns the number of unique values.  This function cannot be
         * optimized away during compilation because it relies on runtime
         * calculations.
         * */
        inline int getNumberValues() const {
          /** \todo Is there a way to make this not rely on heap allocation? */
          std::set<int> s( m_remap, m_remap + number_values );
          return s.size();
        }

        /** Actual remap operation used when performing sorting. */
        template < typename Particle >
        inline int operator()(const Particle & p) const {
          return m_remap[super::operator()(p)];
        }
      };

    }
  }
}

#endif // xylose_nsort_map_remap_h

