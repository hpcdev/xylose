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
 * Collection of routines to modify and access the individual bits in a vector
 * of bytes (can be vector of any type that can be used with the bitwise
 * operations |, &).
 */

#ifndef xylose_bits_hpp
#define xylose_bits_hpp

#include <boost/cstdint.hpp>

#include <vector>
#include <ostream>
#include <istream>
#include <cassert>

namespace xylose {

  /** Routines to modify, access, read-in, print-out, the individual bits in a
   * vector of bytes. */
  namespace bits {

    using  boost::int8_t;
    using boost::uint8_t;
    using  boost::int16_t;
    using boost::uint16_t;
    using  boost::int32_t;
    using boost::uint32_t;

    /* {***** BEGIN OPERATIONS ON NATIVE TYPES (char,int,long,...) ***** */

    /** Reverse the bits so that the highest bit is the lowest and the lowest is
     * the highest.  
     * This operation is used for creating intial seeds of random number
     * generators.
     */
    template < typename T >
    inline T reverse( T v ) {
      T r = T(0);
      size_t s = 8u * sizeof(T);
      for ( ; v; v>>=1u, --s ) {
        r <<= 1u;
        r |= v & T(0x1);
      }
      r <<= s; // shift when v's highest bits are zero
      return r;
    }

#if !defined( __PGIC__ )
    /* For some reason, the other compilers thing int8_t and char are different
     * types. */
    /** Must specialize for signed types because (-1)>>1u == -1 */
    template<>
    inline char reverse( char v ) {
      return static_cast<char>( reverse( static_cast<uint8_t>(v) ) );
    }
#endif

    /** Must specialize for signed types because (-1)>>1u == -1 */
    template<>
    inline int8_t reverse( int8_t v ) {
      return static_cast<int8_t>( reverse( static_cast<uint8_t>(v) ) );
    }

    /** Must specialize for signed types because (-1)>>1u == -1 */
    template<>
    inline int16_t reverse( int16_t v ) {
      return static_cast<int16_t>( reverse( static_cast<uint16_t>(v) ) );
    }

    /** Must specialize for signed types because (-1)>>1u == -1 */
    template<>
    inline int32_t reverse( int32_t v ) {
      return static_cast<int32_t>( reverse( static_cast<uint32_t>(v) ) );
    }

    /* }***** END OPERATIONS ON NATIVE TYPES (char,int,long,...) ***** */







    /* {***** BEGIN OPERATIONS ON VECTORS OF BYTES AGGREGATED AS BITS ***** */

    /** Calculates the number of bits contained within the entire vector. */
    template < typename T >
    inline size_t size( const std::vector<T> & bytes ) {
      static const size_t Tsz = 8u * sizeof(T);
      return bytes.size() * Tsz;
    }

    /** Number of type T that contain at least nbits bits. */
    template < typename T >
    inline size_t numT( const size_t & nbits ) {
      static const size_t Tsz = 8u * sizeof(T);
      return (nbits + Tsz - 1u) / Tsz;
    }

    /** Resizes to contain at least nbits bits. */
    template < typename T >
    inline void resize( std::vector<T> & bytes,
                        const size_t & nbits,
                        const T & newvalue = T() ) {
      bytes.resize( numT<T>(nbits), newvalue );
    }

    /** Reserves enough room for at least nbits bits. */
    template < typename T >
    inline void reserve( std::vector<T> & bytes, const size_t & nbits ) {
      bytes.reserve( numT<T>(nbits) );
    }

    /** Unsets all bits. */
    template < typename T >
    inline void unsetAll( std::vector<T> & bytes ) {
      std::fill( bytes.begin(), bytes.end(), T(0) );
    }

    /** Sets all bits to true. */
    template < typename T >
    inline void setAll( std::vector<T> & bytes ) {
      std::fill( bytes.begin(), bytes.end(), ~T(0) );
    }

    /** Sets the nth bit to true. */
    template < typename T >
    inline void set( std::vector<T> & bytes, const size_t & n ) {
      static const size_t Tsz = 8u * sizeof(T);
      assert( (n/Tsz) < bytes.size() );
      bytes[n/Tsz] |= T(1) << T(n%Tsz);
    }

    /** Unsets the nth bit (sets to false). */
    template < typename T >
    inline void unset( std::vector<T> & bytes, const size_t & n ) {
      static const size_t Tsz = 8u * sizeof(T);
      assert( (n/Tsz) < bytes.size() );
      bytes[n/Tsz] &= (T(1) << T(n%Tsz)) ^ (~T(0));
    }

    /** Test whether the nth bit is set. */
    template < typename T >
    inline bool test( const std::vector<T> & bytes, const size_t & n ) {
      static const size_t Tsz = 8u * sizeof(T);
      assert( (n/Tsz) < bytes.size() );
      return bytes[n/Tsz] & ( T(1) << T(n%Tsz) );
    }

    /** Creates a new set of bits by reversing the order of the given bits. */
    template < typename T >
    inline const std::vector<T> reverse( const std::vector<T> & bytes ) {
      std::vector<T> retval;
      retval.reserve( bytes.size() );
      /* copy the input in reverse order. */
      for ( typename std::vector<T>::const_reverse_iterator i  = bytes.rbegin(),
                                                           end = bytes.rend();
                                                            i != end;
                                                          ++i ) {
        /* reverse each byte. */
        retval.push_back( bits::reverse( *i ) );
      }

      return retval;
    }

    /** Writes sequential bits of vector of bytes to stream as 0's and 1's. */
    template < typename T >
    inline std::ostream & operator<< ( std::ostream & out,
                                       const std::vector<T> & bytes ) {
      const size_t sz = bits::size(bytes);
      for ( size_t i = 0; i < sz; ++i ) {
        out << bits::test(bytes, i);
      }
      return out;
    }

    /** Reads stream of 0's and 1's into sequential bits of vector of bytes. */
    template < typename T >
    inline std::istream & operator>> ( std::istream & in,
                                       std::vector<T> & bytes ) {
      std::vector<T> tmp;

      size_t current_bit = 0;
      char c = in.peek();
      while ( c == '0' || c == '1' ) {

        /* expand the bytes if necessary. */
        if ( current_bit >= bits::size(tmp) )
          tmp.push_back(T(0));

        /* now, set it if necessary */
        if ( c == '1' )
          bits::set( tmp, current_bit );

        ++current_bit;
        (void)in.get();// move past this char
        c = in.peek();
      }

      /* swap the tmp storage out to the return value. */
      bytes.swap(tmp);
      return in;
    }

    /* }***** END OPERATIONS ON VECTORS OF BYTES AGGREGATED AS BITS ***** */

  }/* namespace xylose::bits */
}/* namespace xylose */

#endif // xylose_bits_hpp
