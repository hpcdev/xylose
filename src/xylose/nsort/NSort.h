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


#ifndef xylose_nsort_NSort_h
#define xylose_nsort_NSort_h

#include <xylose/nsort/map/direct.h>
#include <xylose/nsort/tweak/Null.h>
#include <xylose/ref_of.h>

#include <ostream>
#include <algorithm>

namespace xylose {

  /** An \f$ O(N) \f$ sorting utility and a variety of flexible extensions. */
  namespace nsort {

    /** An \f$ O(N) \f$ sort using a predefined number of sorting buckets.
     * This is not necessarily a stable sort.
     * @tparam val_map
     *    Map a reference of the sorted items to an integer bucket index.  Note
     *    that this class does NOT check for overruns in the mapped value (to
     *    see if 0 <= val_map::operator()(item&) < n_values). <br>
     *    [Default nsort::map::direct]
     *
     * @tparam NSortTweaker
     *    Optional class to allow the user code to tweak the map according to
     *    the preliminary counting statistics. <br>
     *    [Default nsort::tweak::Null]
     */
    template < typename val_map = map::direct,
               typename NSortTweaker = tweak::Null >
    class NSort {
    private:
      int n_values;
      int * bin;

    public:
      /** Constructor allocates the specified number of buckets. */
      NSort(const int & n_values) : n_values (n_values) {
        bin = new int[n_values];
      }

      /** Destructor frees the buckets. */
      ~NSort() {
        delete[] bin;
      }

      /** Get the number of bins/values used in this sort. */
      inline const int & size() const { return n_values; }

      /** Obtain the index of the end() element of the ith value.
       * Note that i should conform to 0 <= i < n_values; there is no bound
       * checking on the input i.
       *
       * Only valid after sort(...) has been called. */
      inline const int & end(const int & i) const { return bin[i]; }

      /** Obtain the index of the begin() element of the ith value.
       * Note that i should conform to 0 <= i < n_values; there is no bound
       * checking on the input i.
       *
       * Only valid after sort(...) has been called. */
      inline int begin(const int & i) const {
        if (i == 0) return 0;
        else return bin[i-1];
      }

      /** Get the number of items for a particular value index.
       * This function just computes <code>(end(i) - begin(i))</code>.
       *
       * Only valid after sort(...) has been called. */
      inline int size(const int & i) const { return end(i) - begin(i); }

      /** Overload of sort for using default constructed value map and tweaker.
       * This function subsequently calls the other overload of sort().
       */
      template <class Iter>
      void sort(const Iter & Ai, const Iter & Af,
                const val_map & map = val_map(),
                const NSortTweaker & nsortTweaker = NSortTweaker()) {
        val_map mapcopy = map;
        NSortTweaker tweakcopy = nsortTweaker;
        sort(Ai,Af,mapcopy,tweakcopy);
      }

      /** Sort the items within the range [Ai,Af) using the specified value map
       * and NSort tweaker. */
      template <class Iter>
      void sort(const Iter & Ai, const Iter & Af,
                val_map & map, NSortTweaker & nsortTweaker ) {
        int * ptr = new int[n_values];

        using std::fill;
        fill(bin, bin + n_values, 0);
        fill(ptr, ptr + n_values, 0);

        /* first count the number of occurrences for each value. */
        for (Iter i = Ai; i < Af; ++i)
          ++bin[map(ref_of(*i))];

        /* Allow user code to tweak the map according to the preliminary
         * counting statistics. */
        nsortTweaker.tweakNSort(map, bin, static_cast<const int&>(n_values));

        /* now change this array of occurrences to an array of start
         * positions. */
        for (int i = 0, cur_ptr = 0; i < n_values; ++i) {
          ptr[i]   = cur_ptr;
          cur_ptr += bin[i];
          bin[i]   = cur_ptr;
        }

        for (int i = 0; i < n_values; ++i) {
          const int & end_pos = end(i);
          int & pos = ptr[i];

          while (pos < end_pos) {
            int & pos2 = ptr[map(ref_of(*(Ai + pos)))];

            if (pos == pos2) {
              /* don't need to swap current position, move to next... */
              pos++;
              continue;
            }
            std::iter_swap(Ai+pos, Ai + pos2++);
          }/*while*/
        }/*for*/

        delete[] ptr;
      }/*sort()*/
    };/* NSort class */

  }/* namespace nsort */
}/* namespace xylose */

#endif // xylose_nsort_NSort_h
