// Copyright (C) 2007, 2008 Steven Watanabe, Joseph Gauterin, Niels Dekker
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// For more information, see http://www.boost.org

//-----------------------------------------------------------------------------
// This file was copied from <boost/utility/swap.hpp>. The #include
// guard and boost names were changed to 'xylose'. If it should become
// acceptable for Xylose utilities to use Boost, this file should be removed.
//-----------------------------------------------------------------------------

#ifndef xylose_Swap_hpp
#define xylose_Swap_hpp

/** \file
 * This file was copied from <boost/utility/swap.hpp>. The #include
 * guard and boost names were changed to 'xylose'. If it should become
 * acceptable for Xylose utilities to use Boost, this file should be removed.
 *
 * Note: the implementation of this utility contains various workarounds:
 * - swap_impl is put outside the xylose namespace, to avoid infinite
 * recursion (causing stack overflow) when swapping objects of a primitive
 * type.
 * - swap_impl has a using-directive, rather than a using-declaration,
 * because some compilers (including MSVC 7.1, Borland 5.9.3, and
 * Intel 8.1) don't do argument-dependent lookup when it has a
 * using-declaration instead.
 * - xylose::swap has two template arguments, instead of one, to
 * avoid ambiguity when swapping objects of a Boost type that does
 * not have its own xylose::swap overload.
 */

#include <algorithm> //for std::swap
#include <cstddef> //for std::size_t

/** Implementation namespace for xylose::swap.  
 * swap_impl is put outside the xylose namespace, to avoid infinite
 * recursion (causing stack overflow) when swapping objects of a primitive
 * type.
 */
namespace xylose_swap_impl {

  /** Implementation uses correct version of swap based on argument dependent
   * lookup (ADL). */
  template<class T>
  void swap_impl(T& left, T& right) {
    using namespace std;//use std::swap if argument dependent lookup fails
    swap(left,right);
  }

  /** Implementation for swapping a primitive array of types. */
  template<class T, std::size_t N>
  void swap_impl(T (& left)[N], T (& right)[N]) {
    for ( std::size_t i = 0; i < N; ++i ) {
      ::xylose_swap_impl::swap_impl(left[i], right[i]);
    }
  }

}/* namespace xylose_swap_impl */

namespace xylose {

  template<class T1, class T2>
  void swap(T1& left, T2& right) {
    ::xylose_swap_impl::swap_impl(left, right);
  }

}

#endif // xylose_Swap_hpp
