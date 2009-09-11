#ifndef xylose_Index_hpp
#define xylose_Index_hpp

#include <limits>
#include <ostream>
#include <cassert>

namespace xylose {

  namespace detail {

    // general template to adjust an index forward by one space
    template< typename IndexT, bool >
    struct IndexAdjuster {
      static void adjust( IndexT& index ) {
        if ( ++index.mPosition >= IndexT::segment_size ) {
          index.mSegment++;
          index.mPosition = 0;
        }
      }
    };

    // specialization to adjust an index backward by one space
    template< typename IndexT >
    struct IndexAdjuster< IndexT, false > {
      static void adjust( IndexT& index ) {
        if ( index.mPosition == 0 ) {
          if ( index.mSegment != 0 ) {
            index.mSegment--;
          } else {
            index.mSegment = std::numeric_limits< typename IndexT::size_type >::max();
          }
          index.mPosition = IndexT::segment_size - 1;
        } else {
          index.mPosition--;
        }
      }
    };

  } // namespace detail

  /** Index type indexes a segment, position pair within a 2D type.  The class
   * is templated on the direction of iteration so that this can represent a
   * forward or reverse moving index.  
   */
  template< unsigned int kSegmentSize, bool kIsForward >
  class Index {
    /* TYPEDEFS */
    typedef Index< kSegmentSize, kIsForward > Self;
  public:
      
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    static const size_type segment_size = kSegmentSize;


    /* MEMBER STORAGE */

    size_type mSegment;
    size_type mPosition;



    /* MEMBER FUNCTIONS */
    Index() : mSegment(std::numeric_limits< size_type >::max() ), 
              mPosition(segment_size - 1) {}
    Index( size_type segment, size_type position ) :
      mSegment( segment ), mPosition( position )
    {}

    Index( const Index& rhs ) : mSegment(rhs.mSegment), mPosition(rhs.mPosition) {}

    Index(const size_type i) { fromInteger(i); }

    template <unsigned int kSegmentSize2, bool kIsForward2>
    Index(const Index<kSegmentSize2,kIsForward2> &rhs) :
      mSegment( rhs.mSegment ), mPosition( rhs.mPosition )
    {
      assert (kSegmentSize2 ==segment_size);
    }

    Index invalidIndex() const {
      return Index(std::numeric_limits< size_type >::max(), segment_size - 1 );
    }

    bool isValid() const {
      return (*this != invalidIndex());
    }  
    /// Determine if two indices are equal
    bool operator==( const Index& rhs ) const {
      return ( mSegment == rhs.mSegment ) && ( mPosition == rhs.mPosition );
    }

    /// Determine if two indices are not equal
    bool operator!=( const Index& rhs ) const {
      return ( mSegment != rhs.mSegment ) || ( mPosition != rhs.mPosition );
    }

    /// Determine if lhs indexes a prior position in the train than rhs
    bool operator<( const Index& rhs ) const {
      if ( mSegment < rhs.mSegment  )
        return true;
      return ( mSegment == rhs.mSegment  ) && ( mPosition < rhs.mPosition );
    }


    /// Increment an index linearly through the train
    Index& operator++() {
      detail::IndexAdjuster< Self, kIsForward >::adjust( *this );
      return *this;
    }

    /// Decrement an index linearly through the train
    Index& operator--() {
      detail::IndexAdjuster< Self, !kIsForward >::adjust( *this );
      return *this;
    }

    /// Move n spaces forward in the train
    Index& operator+=( long int n ) {
      if ( n < 0 )
        return operator-=(-n);
      if ( ( mPosition += n ) >= segment_size ) {
        mSegment += ( mPosition / segment_size );
        mPosition %= segment_size;
      }
      return *this;
    }

    /// Add n places to the right hand operand
    Index operator+( long int n ) {
      if ( n < 0 )
        return operator-(-n);
      Index result;
      result.mSegment = mSegment;
      if ( ( result.mPosition = mPosition + n ) >= segment_size ) {
        result.mSegment += ( result.mPosition / segment_size );
        result.mPosition %= segment_size;
      }
      return result;
    }

    /// Move n spaces backward in the train
    Index& operator-=( long int n ) {
      if ( n < 0 )
        return operator+=(-n);
      if ( (size_type)n <= mPosition ) {
        mPosition -= n;
      } else {
        size_type mNewPosition = (segment_size*mSegment+mPosition-n) % segment_size;
        mSegment = (segment_size*mSegment+mPosition-n) / segment_size;
        mPosition = mNewPosition;
      }
      return *this;
    }

    /// Subtract n places from the left hand operand
    Index operator-( long int n ) {
      if ( n < 0 )
        return operator+(-n);
      Index result;
      result.mSegment = mSegment;
      if ( (size_type)n <= mPosition ) {
        result.mPosition = mPosition - n;
      } else {
        result.mPosition = (segment_size*mSegment+mPosition-n) % segment_size;
        result.mSegment = (segment_size*mSegment+mPosition-n) / segment_size;
      }
      return result;
    }

    /// Determine the distance between two indices
    difference_type operator-( const Index& rhs ) {
      return ( mSegment - rhs.mSegment ) * segment_size + mPosition - rhs.mPosition;
    }
      
    /// Set this index from an integer representing the linear location in
    /// the list
    void fromInteger( size_type i ) {
      mSegment = i / segment_size;
      mPosition = i % segment_size;
    }
    
    size_type Integer() {
      return mSegment*segment_size+mPosition;
    }
  };

  /// Print an index to a stream
  template< unsigned int kSize, bool kIsForward >
  std::ostream& operator<<( std::ostream& ostr, const Index< kSize, kIsForward >& rhs ) {
    return ( ostr << "(" << rhs.mSegment << ", " << rhs.mPosition << ")" );
  }

}/* namespace xylose */

#endif // xylose_Index_hpp
