// -*- c++ -*-
// $Id$
/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2004-2008 Spencer Olson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *  
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *                                                                                 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.                                                                           .
 * 
 * Questions? Contact Spencer Olson (olsonse@umich.edu) 
 */

/*
 * $Log: Vector.h,v $
 * Revision 1.1.1.1  2005/01/08 04:57:25  olsonse
 * Initial CVS import (finally).
 *
 */

/** \file
 * Generic SquareMatrix implementation that works nicely with the Vector class.
 * I used to say:<br>
 * BEGIN OLD COMMENTS<br>
 * I would hardly recommend using this stuff for time-sensitive operations
 * because of the inefficiency of using temporaries all over the place.  For
 * time-insensitive operations, these classes are really helpful as the code
 * is easier to write, looks cleaner, and much easier to read.
 * 
 * One <em>could</em> use parts of these classes in time-sensitive operations
 * if one is careful.  I don't want to go into all the caveats for
 * time-sensitive operations, but be assured they can be worked around.
 *   END OLD COMMENTS<br>
 *
 * But, I've noticed that compilers are pretty smart at optimizing temporaries
 * away and unrolling the loops for sizes ~3 (which are mostly what I use this
 * for).  Thus, in most cases, these classes should perform just fine.  
 *
 * Copyright 2004-2008 Spencer Olson
 */


#ifndef xylose_SquareMatrix_h
#define xylose_SquareMatrix_h

#include <xylose/Vector.h>
#include <xylose/power.h>


#include <limits>
#include <sstream>
#include <ostream>

#include <cmath>
#include <cstring>
#include <cstdarg>
#include <stdint.h>


namespace xylose {

  /** Square matrix class.  The idea here is to provide a clean interface to
   * matrix calculations that are not too slow.  The size is compile time and
   * the compiler may opt to unroll loops and perform other optimizations.  
   */
  template <class T, unsigned int L>
  class SquareMatrix {
    /* TYPEDEFS */
  public:
    /** The length of the val array. */
    static const unsigned int length = L;



    /* MEMBER STORAGE */
  public:
    /** The internal storage array of the matrix elements.
     * Note:  Array index is row selection, vector index is column selection. */
    Vector<T,L> val[L];


    /* MEMBER FUNCTIONS */
  public:
    /** Default constructor does nothing. */
    SquareMatrix() {}

    /** SquareMatrix copy constructor--from another SquareMatrix. */
    template < typename T2 >
    SquareMatrix(const SquareMatrix<T2,L> & that) { *this = that; }

    /** SquareMatrix copy constructor--from a one dimensional C-array of
     * Vectors. */
    template < typename T2 >
    SquareMatrix(const Vector<T2,L> that[L]) { *this = that; }

    /** SquareMatrix copy constructor--from a two dimensional C-array. */
    SquareMatrix(const T that[L][L]) { *this = that; }

    /** SquareMatrix assignment constructor.
     * Assigns all elements of the matrix to the given value that.
     */
    SquareMatrix(const T that) { *this = that; }

    /** Set all matrix elements to zero. */
    void zero () {
      using std::fill;
      fill( val, val+L, 0 );
    }

    /** Index operator--non-const version. */
    T & operator() (const int & i, const int & j) { return val[i][j]; }

    /** Index operator--const version. */
    const T & operator() (const int & i, const int & j) const { return val[i][j]; }

    /** Row extractor--non-const version. */
    Vector<T,L> & row(const int & i) { return val[i]; }

    /** Row extractor--const version. */
    const Vector<T,L> & row(const int & i) const { return val[i]; }

    /** Column extractor--const version (no non-const version exists). */
    Vector<T,L> col(const int & j) const {
      Vector<T,L> v;
      for (int i = 0; i < L; ++i) v[i] = val[i][j];
      return v;
    }

    /** Assignment operator--from SquareMatrix. */
    template < typename T2 >
    const SquareMatrix & operator= (const SquareMatrix<T2,L> & that) {
      using std::copy;
      copy( that.val, that.val+L, val );
      return *this;
    }

    /** Assignment operator--from two dimensional C-array. */
    template < typename T2 >
    const SquareMatrix & operator= (const Vector<T2,L> that[L]) {
      using std::copy;
      copy( that, that+L, val );
      return *this;
    }

    /** Assignment operator--from two dimensional C-array. */
    const SquareMatrix & operator= (const T that[L][L]) {
      using std::copy;
      copy( that, that+L, val );
      return *this;
    }

    /** Assignment operator--from scalar. */
    const SquareMatrix & operator= (const T & that) {
      using std::fill;
      fill( val, val+L, that );
      return *this;
    }

    /** Matrix X Scalar immediate multiplication. */
    SquareMatrix & operator*= (const T & that) {
      for (unsigned int i = 0; i < L; ++i )
        this->val[i] *= that;

      return *this;
    }

    /** Matrix - Scalar immediate subtraction. */
    SquareMatrix & operator-= (const T & that) {
      for (unsigned int i = 0; i < L; ++i )
        this->val[i] -= that;

      return *this;
    }

    /** Matrix - Scalar immediate addition. */
    SquareMatrix & operator+= (const T & that) {
      for (unsigned int i = 0; i < L; ++i )
        this->val[i] += that;

      return *this;
    }

    /** Matrix / Scalar immediate division. */
    SquareMatrix & operator/= (const T & that) {
      for (unsigned int i = 0; i < L; ++i ) {
        this->val[i] /= that;
      }

      return *this;
    }

    /** Transpose oeprator. */
    SquareMatrix & transpose() {
      for (unsigned int i = 0; i < L; ++i) {
        for (unsigned int j = 0; j < L; ++j) {
          T tmp = val[i][j];
          val[i][j] = val[j][i];
          val[j][i] = tmp;
        }
      }
      return *this;
    }

    /** component by component multiplication after type of that is converted
     * type of this.
     * @return reference to this (type SquareMatrix<T,L>).
     */
    template <class T2>
    const SquareMatrix & compMult(const SquareMatrix<T2,L>& that) {
      for (unsigned int i = 0; i < L; ++i)
        this->val[i].compMult( that.val[i] );
      return *this;
    }

    /** component by component division after type of that is converted
     * type of this.
     * @return reference to this (type SquareMatrix<T,L>).
     */
    template <class T2>
    const SquareMatrix & compDiv(const SquareMatrix<T2,L>& that) {
      for (unsigned int i = 0; i < L; ++i)
        this->val[i].compMult( that.val[i] );
      return *this;
    }

    /** Create (in temporary storage) an identity matrix equal in size to the
     * current SquareMatrix. */
    static SquareMatrix identity() {
      using std::fill;
      SquareMatrix retval;
      fill(retval.val, retval.val+L, 0);

      for (unsigned int i = 0; i < L; ++i ) {
        retval.val[i][i] = 1;
      }
      return retval;
    }


    /** Convert the SquareMatrix to a string with an optional delimiter between
     * columns (default: [tab]), and an optional delimiter between rows
     * (default: [newline]).
     *
     * @param col_delim
     *  Delimiter between adjacent columns [Default '\t'].
     *
     * @param row_delim
     *  Delimiter between adjacent rows [Default '\n'].
     * */
    std::string to_string( const char & col_delim = '\t',
                           const char & row_delim = '\n' ) const {
      std::stringstream output;
      for (unsigned int i = 0; i < L; ++i)
        output << val[i].to_string( col_delim ) << row_delim;
      return output.str( );
    }
  };






  /* ****** BEGIN MATH OPERATIONS ******* */
  /** Matrix inner product. */
  template < typename TL, typename TR, unsigned int L >
  SquareMatrix<TL,L> operator* ( const SquareMatrix<TL,L> & lhs,
                                 const SquareMatrix<TR,L> & rhs ) {
    SquareMatrix<TL,L> result;

    for (unsigned int i = 0; i < L; ++i ) {
      for (unsigned int j = 0; j < L; ++j) {
        TL & val_ij = result.val[i][j];
        val_ij = 0;
        for (unsigned int k = 0; k < L; k++ ) {
          val_ij +=  lhs.val[i][k] * rhs.val[k][j];
        }
      }
    }

    return result;
  }

  /** Matrix subtraction. */
  template < typename TL, typename TR, unsigned int L >
  SquareMatrix<TL,L> operator- ( const SquareMatrix<TL,L> & lhs,
                                 const SquareMatrix<TR,L> & rhs ) {
    SquareMatrix<TL,L> result;

    for (unsigned int i = 0; i < L; ++i )
      result.val[i] = lhs.val[i] - rhs.val[i];

    return result;
  }

  /** Matrix addition. */
  template < typename TL, typename TR, unsigned int L >
  SquareMatrix<TL,L> operator+ ( const SquareMatrix<TL,L> & lhs,
                                 const SquareMatrix<TR,L> & rhs ) {
    SquareMatrix<TL,L> result;

    for (unsigned int i = 0; i < L; ++i )
      result.val[i] = lhs.val[i] + rhs.val[i];

    return result;
  }

  /** Matrix X Vector multiplication. */
  template < typename TL, typename TR, unsigned int L >
  Vector<TL,L> operator* ( const SquareMatrix<TL,L> & lhs,
                           const Vector<TR,L> & rhs ) {
    Vector<TL,L> result;

    for (unsigned int i = 0; i < L; ++i )
      result.val[i] = lhs.val[i] * rhs;

    return result;
  }

  /** Matrix * Scalar. */
  template < typename TL, typename TR, unsigned int L >
  SquareMatrix<TL,L> operator* ( const SquareMatrix<TL,L> & lhs,
                                 const TR & rhs ) {
    SquareMatrix<TL,L> result;

    for (unsigned int i = 0; i < L; ++i )
      result.val[i] = lhs.val[i] * rhs;

    return result;
  }

  /** Matrix / Scalar. */
  template < typename TL, typename TR, unsigned int L >
  SquareMatrix<TL,L> operator/ ( const SquareMatrix<TL,L> & lhs,
                                 const TR & rhs ) {
    SquareMatrix<TL,L> result;

    for (unsigned int i = 0; i < L; ++i )
      result.val[i] = lhs.val[i] / rhs;

    return result;
  }

  /** Matrix + Scalar. */
  template < typename TL, typename TR, unsigned int L >
  SquareMatrix<TL,L> operator+ ( const SquareMatrix<TL,L> & lhs,
                                 const TR & rhs ) {
    SquareMatrix<TL,L> result;

    for (unsigned int i = 0; i < L; ++i )
      result.val[i] = lhs.val[i] + rhs;

    return result;
  }

  /** Matrix - Scalar. */
  template < typename TL, typename TR, unsigned int L >
  SquareMatrix<TL,L> operator- ( const SquareMatrix<TL,L> & lhs,
                                 const TR & rhs ) {
    SquareMatrix<TL,L> result;

    for (unsigned int i = 0; i < L; ++i )
      result.val[i] = lhs.val[i] - rhs;

    return result;
  }



  template <class T, unsigned int L>
  inline SquareMatrix<T,L> transpose(const SquareMatrix<T,L> & m) {
    SquareMatrix<T,L> retval;
    for (unsigned int i = 0; i < L; ++i) {
      for (unsigned int j = 0; j < L; ++j) {
        retval(i,j) = m(j,i);
      }
    }
    return retval;
  }

  /** Component by component multiplication after type of that is converted
   * type of this.
   * @return reference to temporary (type SquareMatrix<T,L>).
   *
   * @see Vector::compDiv.
   */
  template <class T1, class T2, unsigned int L>
  inline const SquareMatrix<T1,L> compMult(const SquareMatrix<T1,L> & m1,
                                           const SquareMatrix<T2,L> & m2) {
    SquareMatrix<T1,L> retval;
    for (unsigned int i = 0; i < L; ++i)
      for (unsigned int j = 0; j < L; ++j)
        retval.val[i][j] = m1.val[i][j] * m2.val[i][j];
    return retval;
  }

  /** Component by component division after type of that is converted
   * type of this.
   * @return reference to temporary (type SquareMatrix<T,L>).
   *
   * @see Vector::compMult.
   */
  template <class T1, class T2, unsigned int L>
  inline const SquareMatrix<T1,L> compDiv(const SquareMatrix<T1,L> & m1,
                                          const SquareMatrix<T2,L> & m2) {
    SquareMatrix<T1,L> retval;
    for (unsigned int i = 0; i < L; ++i)
      for (unsigned int j = 0; j < L; ++j)
        retval.val[i][j] = m1.val[i][j] / m2.val[i][j];
    return retval;
  }

  /** Component by component power after type of that is converted
   * type of this.
   * @return reference to temporary (type SquareMatrix<T,L>).
   *
   * @see Vector::compMult.
   */
  template <class T1, class T2, unsigned int L>
  inline const SquareMatrix<T1,L> compPow(const SquareMatrix<T1,L> & m1,
                                          const SquareMatrix<T2,L> & m2) {
    SquareMatrix<T1,L> retval;
    for (unsigned int i = 0; i < L; ++i)
      for (unsigned int j = 0; j < L; ++j)
        retval.val[i][j] = fast_pow(m1.val[i][j], m2.val[i][j]);
    return retval;
  }

  /** Component by component power after type of that is converted
   * type of this.
   * @return reference to temporary (type SquareMatrix<T,L>).
   *
   * @see Vector::compMult.
   */
  template <class T, unsigned int L>
  inline const SquareMatrix<T,L> compPow(const SquareMatrix<T,L> & m1,
                                         const double & e) {
    SquareMatrix<T,L> retval;
    for (unsigned int i = 0; i < L; ++i)
      for (unsigned int j = 0; j < L; ++j)
        retval.val[i][j] = fast_pow(m1.val[i][j], e);
    return retval;
  }

  /** Define SQR explicitly to be in the inner product of a SquareMatrix with its
   * self.  We define this specialization because the return value is not the
   * same as the arguments. */
  template <class T, unsigned int L>
  inline SquareMatrix<T,L> SQR(const SquareMatrix<T,L> & m) {
    return m*m;
  }




  /** Stream output operator. */
  template <class T, unsigned int L>
  inline std::ostream & operator<< (std::ostream & output, const SquareMatrix<T,L> & m) {
    for (unsigned int i = 0; i < L; ++i) {
      for (unsigned int j = 0; j < L; ++j) {
        output << m.val[i][j] << '\t';
      }
      output << '\n';
      //output << ";\t";
    }
    return output;
  }

  /** Stream input operator. */
  template <class T, unsigned int L>
  inline std::istream & operator>> (std::istream & input, SquareMatrix<T,L> & m) {
    for (unsigned int i = 0; i < L; ++i) {
      for (unsigned int j = 0; j < L; ++j) {
        input >> m.val[i][j];
      }
    }
    return input;
  }




  /** Generic implementation of make_matrix class.  Note that this uses va_args
   * and as such is not bug friendly.  Common used lengths will be implemented
   * seperately with absolute arguments.
   */
  template < typename T, unsigned int L >
  struct make_matrix {
    static const int
      not_implemented = T::MATRIX_SIZE_NOT_YET_SUPPORTED;
  };

  /** Specific implementation of make_vector class for length=3.  Providing this
   * will help avoid introducing bugs which va_arg stuff is prone to. 
   */
  template < typename T >
  struct make_matrix<T,3u> {
    SquareMatrix<T,3u> operator() ( const Vector<T,3u> & t0,
                                    const Vector<T,3u> & t1,
                                    const Vector<T,3u> & t2 ) {
      SquareMatrix<T,3u> retval;
      retval.val[0] = t0;
      retval.val[1] = t1;
      retval.val[2] = t2;
      return retval;
    }
  };

  /** Specific implementation of make_vector class for length=6.  Providing this
   * will help avoid introducing bugs which va_arg stuff is prone to. 
   */
  template < typename T >
  struct make_matrix<T,6u> {
    SquareMatrix<T,6u> operator() ( const Vector<T,6u> & t0,
                                    const Vector<T,6u> & t1,
                                    const Vector<T,6u> & t2,
                                    const Vector<T,6u> & t3,
                                    const Vector<T,6u> & t4,
                                    const Vector<T,6u> & t5 ) {
      SquareMatrix<T,6u> retval;
      retval.val[0] = t0;
      retval.val[1] = t1;
      retval.val[2] = t2;
      retval.val[3] = t3;
      retval.val[4] = t4;
      retval.val[5] = t5;
      return retval;
    }
  };


  /** Easier form of make_vector for 3-vector of arbitrary type. */
  template < typename T >
  inline SquareMatrix<T,3u> M3( const Vector<T,3u> & a,
                                const Vector<T,3u> & b,
                                const Vector<T,3u> & c ) {
    return make_matrix<T,3u>()(a,b,c);
  }

}/* namespace xylose */

#endif // xylose_SquareMatrix_h
