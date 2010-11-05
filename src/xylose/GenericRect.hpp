/*  This software is property of the United States Air Force.
 *  Use of this software is contingent upon agreement to terms in the
 *  file ICEPIC/TERMS.  A signed hard copy of this agreement should
 *  be on-file with the distribution authority before it is used.
 */
 
/* Points and rectangles
 * Bradley Smith, 4 Dec 94
 */


#ifndef __GENERICRECT_HPP_
#define __GENERICRECT_HPP_

#include <xylose/GenericPoint.hpp>

#include <algorithm>
#include <limits>

namespace xylose {

/** This function is a generic rectangle.
    It is defined by the lower and upper bounds
    rmin, and rmax.  It has constructors based on points.
   
 */
template< typename T >
class GenericRect {
public:
  /// Lower bound of the rectangle
  GenericPoint <T> rmin;
  /// Upper bound of the rectangle
  GenericPoint <T> rmax;

  /// Standard constructors
  GenericRect() :rmin(), rmax() {} 
  GenericRect(const GenericPoint<T> &min, const GenericPoint<T> &max) :
    rmin(min), rmax(max) {} 
  GenericRect(const T min, const T max) :
    rmin(GenericPoint<T>(min)), rmax(GenericPoint<T>(max)) {} 
  GenericRect(const T min1,const T min2,const T min3,
	      const T max1,const T max2,const T max3) :
    rmin(GenericPoint<T>(min1,min2,min3)), rmax(GenericPoint<T>(max1,max2,max3)) {} 

  /// Standard copy constructor
  GenericRect(const GenericRect<T> &r) : rmin(r.rmin),rmax(r.rmax){}

  /// Modification constructors
  template <typename T2>
  GenericRect(const GenericRect<T2> &r) :rmin(T(r.rmin)),rmax(T(r.rmax)) {} 

  /// Set to a null rect
  void Nullify() { rmin = rmax = GenericPoint<T>(0);};

  /// Union operator, will return the union of two rects
  GenericRect<T> Union(const GenericRect<T> &r) const {
    return GenericRect<T>(std::min(rmin.x, r.rmin.x),
			  std::min(rmin.y, r.rmin.y),
			  std::min(rmin.z, r.rmin.z),
			  std::max(rmax.x, r.rmax.x),
			  std::max(rmax.y, r.rmax.y),
			  std::max(rmax.z, r.rmax.z));
  }



  /// Expands the rect to include the input coordinate.  If the rect is
  /// uninitialized, its min and max are set to the input coordinate.
  void expand( const GenericPoint< T >& p ) {
    if ( p.x < rmin.x ) rmin.x = p.x;
    if ( p.x > rmax.x ) rmax.x = p.x;
    
    if ( p.y < rmin.y ) rmin.y = p.y;
    if ( p.y > rmax.y ) rmax.y = p.y;
    
    if ( p.z < rmin.z ) rmin.z = p.z;
    if ( p.z > rmax.z ) rmax.z = p.z;
  }

  /// Check to see if the bounds on the min are higher than the max
  bool IsValid() const { 
    return ( !(rmax.x < rmin.x ||rmax.y < rmin.y ||rmax.z < rmin.z ));
  }

  /// See if the rect is empty 
  bool IsNullRect() const {
    return (rmin.x == 0 && rmax.x == 0 &&
	    rmin.y == 0 && rmax.y == 0 &&
	    rmin.z == 0 && rmax.z == 0);
  }

  T height() const {
    return this->rmax.y - this->rmin.y;
  }

  T width() const {
    return this->rmax.x - this->rmin.x;
  }

  T depth() const {
    return this->rmax.z - this->rmin.z;
  }

  /// Get the volume of the bounding box
  T volume() const {
    float h = rmax.y - rmin.y;
    float w = rmax.x - rmin.x;
    float d = rmax.z - rmin.z;
    return h*w*d;
  }

  /// Expand operators
  GenericRect<T> operator +( const T &r) {
    return GenericRect<T> (rmin-r,rmax+r);
  }

  GenericRect<T> operator +( const GenericPoint<T> &r) {
    return GenericRect<T> (rmin-r,rmax+r);
  }

  /// Contraction operators
  GenericRect<T> operator - ( const T &r) {
    return GenericRect<T> (rmin+r,rmax-r);
  }

  GenericRect<T> operator - ( const GenericPoint<T> &r) {
    return GenericRect<T> (rmin+r,rmax-r);
  }

  bool PointInRect(const GenericPoint<T> &p) const { 
    return ( p.x >= rmin.x && p.y >= rmin.y && p.z >= rmin.z &&
	     p.x <= rmax.x && p.y <= rmax.y && p.z <= rmax.z);
  }

  GenericRect<T> Intersect(const GenericRect<T> &r) const {
    GenericRect<T> intersect;
    /* Compute intersection */
    intersect.rmin = AggregateMax(rmin,r.rmin);
    intersect.rmax = AggregateMin(rmax,r.rmax);
    if (!intersect.IsValid() )
      intersect.Nullify();

    return intersect;
  }

  GenericRect<T> Union(const GenericPoint<T> &p) const {
    GenericRect<T> intersect;
    /* Compute intersection */
    intersect.rmin = AggregateMin(rmin,p);
    intersect.rmax = AggregateMax(rmax,p);
    return intersect;
  }

};


template < typename T >
GenericRect<T> Intersection( const GenericRect<T>& lhs, const GenericRect<T>& rhs )
{
  return lhs.Intersect( rhs );
}

/*! Equality comparison for two rectangles */
template< typename T >
inline bool operator==( 
  const GenericRect<T>& lhs, 
  const GenericRect<T>& rhs ) {
  return (lhs.rmin == rhs.rmin) && (lhs.rmax == rhs.rmax);
}

/*! Determine if two rectangles are not equal */
template< typename T >
inline bool operator!=(
  const GenericRect<T>& lhs,
  const GenericRect<T>& rhs ) {
  return (lhs.rmin != rhs.rmin) || (lhs.rmax != rhs.rmax);
}



}

#endif /* __GENERICRECT_HPP_ */
