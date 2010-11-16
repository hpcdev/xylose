#ifndef _GENERICPOINT_H_
#define _GENERICPOINT_H_

#include <xylose/compat/math.hpp>

#include <algorithm>
#include <ostream>
#include <cassert>

#if defined(_MSC_VER)
  #undef min
  #undef max
#endif

namespace xylose {

/** This class is a 3D vector.  It can
 * be accessed by either .xyz or through
 * vector notation
 */
template< typename T >
class GenericPoint {
// FIXME: These should be private.
public:

  /// Storage locations of the data
  T x,y,z;

public:
  typedef T value_type;

  GenericPoint() :x(),y(),z(){}
  GenericPoint(const T t) :x(t),y(t),z(t){}
  //  GenericPoint(const T &t1,const T &t2,const T &t3) :x(t1),y(t2),z(t3){}
  GenericPoint(const T t1,const T t2,const T t3) :x(t1),y(t2),z(t3){}
  template <typename T2>
  GenericPoint(const GenericPoint<T2> &p):x(T(p.x)),y(T(p.y)),z(T(p.z)){} 
  


  /// Array access operator (non-const version)
  T& operator[]( size_t i ) {
    switch( i ) {
      case 0: return x;
      case 1: return y;
      case 2: return z;
    default: assert(false);
      return x;
    }
  }

  /// Array access operator (const version)
  const T& operator[]( size_t i ) const {
    switch( i ) {
      case 0: return x;
      case 1: return y;
      case 2: return z;
    default: assert(false);
      return x;
    }
  }

  /// Addition operators
  GenericPoint<T> operator + (const T &rhs) const{
    return GenericPoint<T> (x+rhs,y+rhs,z+rhs);
  }

  GenericPoint<T> operator + (const GenericPoint<T> &rhs) const{
    return GenericPoint<T> (x+rhs.x,y+rhs.y,z+rhs.z);
  }

  GenericPoint<T>& operator += (const GenericPoint<T> &rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }

  GenericPoint<T>& operator += (const T &rhs) {
    x += rhs;
    y += rhs;
    z += rhs;
    return *this;
  }

  /// Subtraction operators
  GenericPoint<T> operator - (const T &rhs) const{
    return GenericPoint<T> (x-rhs,y-rhs,z-rhs);
  }

  GenericPoint<T> operator - (const GenericPoint<T> &rhs) const{
    return GenericPoint<T> (x-rhs.x,y-rhs.y,z-rhs.z);
  }

  GenericPoint<T>& operator -= (const T &rhs) {
    x -= rhs;
    y -= rhs;
    z -= rhs;
    return *this;
  }

  GenericPoint<T>& operator -= (const GenericPoint<T> &rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }

  /// Multiplication operators
  GenericPoint<T> operator * (const T &rhs) const {
    return GenericPoint<T> (x*rhs,y*rhs,z*rhs);
  }

  GenericPoint<T> operator * (const GenericPoint<T> &rhs) const {
    return GenericPoint<T> (x*rhs.x,y*rhs.y,z*rhs.z);
  }
  
  GenericPoint<T>& operator *= (const T &rhs)  {
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
  }

  GenericPoint<T> operator *= (const GenericPoint<T> &rhs)  {
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
  }

  /// Division operators
  GenericPoint<T> operator / (const T &rhs) const{
    return GenericPoint<T> (x/rhs,y/rhs,z/rhs);
  }

  GenericPoint<T> operator / (const GenericPoint<T> &rhs) const{
    return GenericPoint<T> (x/rhs.x,y/rhs.y,z/rhs.z);
  }

  GenericPoint<T>& operator /= (const T &rhs) {
    x /= rhs;
    y /= rhs;
    z /= rhs;
    return *this;
  }

  GenericPoint<T>& operator /= (const GenericPoint<T> &rhs) {
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    return *this;
  }


  bool operator == (const GenericPoint<T> &rhs) const {
    return !memcmp(this,&rhs,sizeof(GenericPoint<T>));
  }

  bool operator != (const GenericPoint<T>& rhs) const {
    return !( GenericPoint<T>::operator==( rhs ) );
  }


  T length() const {
    return std::sqrt(x*x + y*y + z*z);
  }

  /// Returns a normalized copy of the vector
  GenericPoint<T> normalize()  const {
    T l = length();
    assert (l > 0.);
      
    return GenericPoint<T>(x / l, y / l, z / l);
  }

};

/// Performs a pointwise comparison of the two given points with the given
/// predicate. The comparison will return true iff the comparator is true for
/// each of the point components.
///
/// @param lhs Left side of comparison
/// @param rhs Right side of comparison
/// @param pred Binary predicate for comparison
template< typename T, typename PredT >
bool pointwiseCompare( 
  const GenericPoint< T >& lhs,
  const GenericPoint< T >& rhs,
  const PredT& pred )
{
  return pred(lhs.x, rhs.x) && pred(lhs.y, rhs.y) && pred(lhs.z,rhs.z);
}

/// Lexicographic comparison. Compares two points according to the specified
/// predicate as dictionary words are compared. 
///
/// @param lhs Left side of the comparison.
/// @param rhs Right side of the comparison.
/// @param pred A binary predicate comparing two of the point values.
template< typename T, typename PredT >
bool lexicographicCompare( 
  const GenericPoint<T>& lhs, 
  const GenericPoint<T>& rhs,
  const PredT& pred )
{
  if ( pred( lhs.z, rhs.z ) ) return true;
  if ( pred( lhs.y, rhs.y ) ) return ( lhs.z == rhs.z );
  return ( lhs.y == rhs.y ) && ( pred( lhs.x, rhs.x ) );
}

/* ******************************************************* */
/* ***** BEGIN AGREGATE MIN/MAX FUNCTIONS FOR POINTS ***** */
/* ******************************************************* */

/// Aggregate minimum of two points.
template <class T>
inline
GenericPoint<T> AggregateMin(const GenericPoint<T> &lhs,
                             const GenericPoint<T> &rhs) {
    return GenericPoint<T>(std::min(lhs.x, rhs.x),
                           std::min(lhs.y, rhs.y),
                           std::min(lhs.z, rhs.z));
}

/// Aggregate maximum of two points.
template <class T>
inline
GenericPoint<T> AggregateMax(const GenericPoint<T> &lhs,
                             const GenericPoint<T> &rhs) {
    return GenericPoint<T>(std::max(lhs.x, rhs.x),
                           std::max(lhs.y, rhs.y),
                           std::max(lhs.z, rhs.z));
}

/// Aggregate minimum of a point compared to a single value.
template <class T>
inline
GenericPoint<T> AggregateMin(const GenericPoint<T> &lhs, const T & rhs) {
    return AggregateMin(lhs,GenericPoint<T>(rhs));
}

/// Aggregate minimum of a point compared to a single value.
template <class T>
inline
GenericPoint<T> AggregateMin(const T & rhs, const GenericPoint<T> &lhs) {
    return AggregateMin(GenericPoint<T>(lhs),rhs);
}

/// Aggregate maximum of a point compared to a single value.
template <class T>
inline
GenericPoint<T> AggregateMax(const GenericPoint<T> &lhs, const T & rhs) {
    return AggregateMax(lhs,GenericPoint<T>(rhs));
}

/// Aggregate maximum of a point compared to a single value.
template <class T>
inline
GenericPoint<T> AggregateMax(const T & rhs, const GenericPoint<T> &lhs) {
    return AggregateMax(GenericPoint<T>(lhs),rhs);
}

/* ******************************************************* */
/* ***** END AGREGATE MIN/MAX FUNCTIONS FOR POINTS ***** */
/* ******************************************************* */
template <typename T>
T dot(const GenericPoint<T> &lhs, const GenericPoint<T> &rhs) {
  return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
}

template <typename T>
GenericPoint<T> cross(const GenericPoint<T> &lhs, const GenericPoint<T> &rhs) {
    GenericPoint<T> c;
    c.x = lhs.y*rhs.z - lhs.z*rhs.y;
    c.y = lhs.z*rhs.x - lhs.x*rhs.z;
    c.z = lhs.x*rhs.y - lhs.y*rhs.x;
    return c;
}

/// Project v onto n
template <typename T>
GenericPoint<T> project(const GenericPoint<T> &v, const GenericPoint<T> &n) {
  return n * (dot(v,n)/n.length());
}



/*! Print a point to an ostream */
template< typename T >
std::ostream& operator<<( std::ostream& ostr, const GenericPoint<T>& point ) {
  ostr << "(" << point.x << ", " << point.y << ", " << point.z << ")";
  return ostr;
}


}


#endif

