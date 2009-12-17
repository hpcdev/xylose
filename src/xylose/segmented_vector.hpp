#ifndef xylose_segmented_vector_hpp
#define xylose_segmented_vector_hpp

#include <xylose/detail/Iterator.hpp>

#include <xylose/Index.hpp>
#include <xylose/Stack.hpp>

#include <algorithm>
#include <functional>
#include <limits>
#include <vector>

#include <cassert>
#include <cmath>

namespace xylose {

  /** A segmented_vector is a dynamically sized list broken into statically sized
   * chunks (segments).  As more elements are added to the structure, the
   * list adds new segments.  This is useful for two reasons:
   * 1. Contiguous blocks are allocated one at a time, no reallocation
   *    needed 
   * 2. For certain types, specializations may be defined for efficient
   *    sorting 
   */
  template< typename T, 
            unsigned int kSegmentSize, 
            typename Alloc = std::allocator< int > >
  class segmented_vector {
    /* TYPEDEFS */
  public:
    //-- typedefs to make this container standard compliant --//
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    
    typedef xylose::Index< kSegmentSize, true  > Index;
    typedef xylose::Index< kSegmentSize, false > ReverseIndex;

    typedef detail::Iterator< segmented_vector, true, false  > iterator;
    typedef detail::Iterator< segmented_vector, true, true   > const_iterator;
    typedef detail::Iterator< segmented_vector, false, false > reverse_iterator;
    typedef detail::Iterator< segmented_vector, false, true  > const_reverse_iterator;
    
    static const size_type segment_size = kSegmentSize;

  private:
    typedef typename Alloc::template rebind< T  >::other        Allocator;
    typedef typename Alloc::template rebind< T* >::other PointerAllocator;
    typedef Stack< T*, PointerAllocator > DataType;


    /* MEMBER STORAGE */
  private:
    DataType mData;
    size_type mNSegments;
    size_type mFirstFreeSegment;
    std::vector< size_type > mFirstFreeSeat;
    Allocator mAlloc;


    /* MEMBER FUNCTIONS */
  public:

    /// Default Constructor
    segmented_vector();

    /// Copy constructor
    segmented_vector( const segmented_vector& other );

    /// Default Destructor
    ~segmented_vector();

    /// Assigment operator
    segmented_vector & operator= ( const segmented_vector& other );

    /// Clear all memory allocated by the segmented_vector
    void clear();

    /// Return the length of the segmented_vector
    size_type size() const;

    /// Return the capacity of the segmented_vector
    size_type capacity() const;

    /// Get a const iterator pointing to the beginning of the container
    const_iterator begin() const;
    /// Get an iterator pointing to the beginning of the container
    iterator begin();
    /// Get a const iterator pointing to the end of the container
    const_iterator end() const;
    /// Get an iterator pointing to the end of the container
    iterator end();

    /// Get a const reverse iterator pointing to the end of the container
    const_reverse_iterator rbegin() const;
    /// Get a reverse iterator pointing to the end of the container
    reverse_iterator rbegin();
    /// Get a const reverse iterator pointing to the beginning of the container
    const_reverse_iterator rend() const;
    /// Get a reverse iterator pointing to the beginning of the container
    reverse_iterator rend();

    /// Index the const traincar using an integer
    const_reference operator[]( size_type i ) const;
    /// Index the traincar using an integer
    reference operator[]( size_type i );

    /// Index the const traincar using an integer
    const_reference operator[]( const Index &i ) const;
    /// Index the traincar using an integer
    reference operator[]( const Index &i );

    /// Get a const reference to the value at the given index.
    template< typename IndexT >
    const_reference get( const IndexT& index ) const;
    /// Get the value at the given index within the segmented_vector
    template< typename IndexT >
    reference get( const IndexT& index );


    /// Get a const reference to the value at the given index.
    const_reference back(  ) const;
    /// Get the value at the given index within the train car
    reference back(  );

    /// Get a const pointer to the location at the given index
    template< typename IndexT >
    const_pointer getPointer( const IndexT& index ) const;
    /// Get a pointer to the location at the given index
    template< typename IndexT >
    pointer getPointer( const IndexT& index );


    /// Get a const pointer to the location at the given index
    template< typename IndexT >
    const_iterator getIterator( const IndexT& index ) const;
    /// Get a pointer to the location at the given index
    template< typename IndexT >
    iterator getIterator( const IndexT& index );

    /// Determine if the given index is valid;
    template< typename IndexT >
    bool isValid( const IndexT& index ) const;

    /// Retrieve the index corresponding to the given object pointer
    template< typename IndexT >
    IndexT index( const pointer ptr ) const;

    /// Return an invalid index
    Index invalidIndex() const {
      static Index invld;
      return invld.invalidIndex();
    }

    /// Returns an constant invalid iterator
    const_iterator invalidIterator() const {
      return getIterator(invalidIndex());
    }

    /// Returns an invalid iterator
    iterator invalidIterator()  {
      return getIterator(invalidIndex());
    }

    /// Retrieve the const reverse iterator corresponding to the given object pointer
    const_reverse_iterator getRIterator( const pointer ptr ) const {
      return const_reverse_iterator( *this, index< ReverseIndex >( ptr ) );
    }
    
    /// Retrieve the reverse iterator corresponding to the given object pointer
    reverse_iterator getRIterator( const pointer ptr )  { 
      return (reverse_iterator(*this,index<ReverseIndex >(ptr))); }

    /// Retrieve the const iterator corresponding to the given object pointer
    const_iterator getIterator( const pointer ptr ) const {
      if ( ptr == NULL)
        return invalidIterator();
      return const_iterator( *this, index< Index >( ptr ) ); 
    }

    /// Retrieve the iterator corresponding to the given object pointer
    iterator getIterator( const pointer ptr ) {
      if ( ptr == NULL)
        return invalidIterator();
      return iterator( *this, index< Index >( ptr ) ); 
    }

    /// Append a new item to the list
    void push_back( const_reference value );

    /// Allocate space for a new element on the list and return a pointer
    /// to the storage
    pointer allocate();

    /// Swap two points based on an index
    void swap(const Index& lhs, const Index& rhs) {
      T tmp = get(rhs);
      get(rhs) = get(lhs);
      get(lhs) = tmp;
    }

    void swap(const int lhs, const int rhs) {
      T tmp = (*this)[rhs];
      (*this)[rhs] = (*this)[lhs];
      (*this)[lhs] = tmp;
    }

    /// Swap the guts of this segmented_vector with another
    void swap( segmented_vector & other ) {
      using std::swap;
      //using std::__alloc_swap;
      this->mData.swap( other.mData );
      swap( this->mNSegments, other.mNSegments );
      swap( this->mFirstFreeSegment, other.mFirstFreeSegment);
      this->mFirstFreeSeat.swap( other.mFirstFreeSeat );
      /* This seems to be what the GNU vector::swap does. */
      //__alloc_swap<Allocator>::_S_do_it(this->mAlloc, other.mAlloc);
    }

    /// Reserve enough space for n elements
    void reserve( size_type n );

    /// Remove an element from the list possibly using a custom deleter
    template< typename DestroyFunctionT >
    void erase( const ReverseIndex& index, const DestroyFunctionT& destroy );
    void erase( const ReverseIndex& index );

    /// Remove the last element from the list
    void pop_back(  ) {erase(rend());}


    /// Erase all elements that match the given predicate
    template< typename PredT, typename DestroyFunctionT > 
    void eraseIf( const PredT& predicate, const DestroyFunctionT& destroy );
    template< typename PredT > void eraseIf( const PredT& predicate );

    /// Compact the list
    void compact();

    /// Apply a function to each segmented_vector
    template< typename SegmentFnT > 
    int iterateSegments( const SegmentFnT& func );

  private:
    
    // append a new segment to the list
    void appendSegment();
  };

  template< unsigned int kSegmentSize >
  struct make_stl_container {
    template< typename T, 
              typename Alloc = std::allocator< int > >
    struct type : segmented_vector<T, kSegmentSize, Alloc> {
      typedef segmented_vector<T, kSegmentSize, Alloc> super;

      /// Default constructor
      type( ) : super() {}

      /// Copy constructor
      type( const type& other ) : super(other) {}

      /// Assigment operator
      type & operator= ( const type& other ) {
        super::operator=(other);
        return *this;
      }
    };
  };


  //------------------------------------------------------------------------------
  namespace detail {
    
    // null destruction operation
    template< typename T >
    struct NullDestructor {
      void operator()( T& ) const {
        /* no-op */
      }
    };

  } // namespace detail

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  segmented_vector< T, kSegmentSize, Alloc >::segmented_vector() :
    mData(),
    mNSegments( 0 ),
    mFirstFreeSegment( 0 ),
    mFirstFreeSeat(),
    mAlloc()
  {
    mFirstFreeSeat.push_back( 0 );
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  segmented_vector< T, kSegmentSize, Alloc >::segmented_vector(
    const segmented_vector< T, kSegmentSize, Alloc >& other ) :
    mData(),
    mNSegments( other.mNSegments ),
    mFirstFreeSegment( other.mFirstFreeSegment ),
    mFirstFreeSeat( other.mFirstFreeSeat ),
    mAlloc( other.mAlloc )

  {
    for ( size_type i = 0; i < mNSegments; ++i ) {
      pointer newSegment = mAlloc.allocate( segment_size );
      mData.push_back( newSegment );
      std::uninitialized_copy( 
        other.mData[i], 
        other.mData[i] + other.mFirstFreeSeat[i],
        mData[i] );
    }
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  segmented_vector< T, kSegmentSize, Alloc > & segmented_vector< T, kSegmentSize, Alloc >::operator=(
    const segmented_vector< T, kSegmentSize, Alloc >& other ) {
    /* start with clearing everything out of this container. */
    clear();

    mNSegments = other.mNSegments;
    mFirstFreeSegment = other.mFirstFreeSegment;
    mFirstFreeSeat = other.mFirstFreeSeat;
    mAlloc = other.mAlloc;

    for ( size_type i = 0; i < mNSegments; ++i ) {
      pointer newSegment = mAlloc.allocate( segment_size );
      mData.push_back( newSegment );
      std::uninitialized_copy( 
        other.mData[i], 
        other.mData[i] + other.mFirstFreeSeat[i],
        mData[i] );
    }

    return *this;
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  segmented_vector< T, kSegmentSize, Alloc >::~segmented_vector()
  {
    clear();
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  void segmented_vector< T, kSegmentSize, Alloc >::clear()
  { 
    // for each segment in the Train
    for ( size_type segment = 0; segment != mNSegments; ++segment ) {
      pointer p = mData[ segment ];
      // for each filled seat on the segment
      for ( size_type i = 0; i < mFirstFreeSeat[ segment ]; ++i ) {
        // invoke the destructor for the object at the current seat
        mAlloc.destroy( p + i );
      }
      mAlloc.deallocate( p, segment_size );
    }

    mData.clear();
    mFirstFreeSeat.clear();
    mNSegments = 0;
    mFirstFreeSegment = 0;
    mFirstFreeSeat.push_back( 0 );
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  typename segmented_vector< T, kSegmentSize, Alloc >::size_type 
  segmented_vector< T, kSegmentSize, Alloc >::size() const
  {
    if ( mNSegments == 0 ) {
      return 0;
    }
    return ( mFirstFreeSegment ) * kSegmentSize + mFirstFreeSeat[mFirstFreeSegment];
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  typename segmented_vector< T, kSegmentSize, Alloc >::size_type 
  segmented_vector< T, kSegmentSize, Alloc >::capacity() const
  {
    if ( mNSegments == 0 ) {
      return 0;
    }
    return ( mNSegments ) * kSegmentSize;
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  typename segmented_vector< T, kSegmentSize, Alloc >::const_iterator
  segmented_vector< T, kSegmentSize, Alloc >::begin() const
  {
    return const_iterator( *this, Index( 0, 0 ) );
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  typename segmented_vector< T, kSegmentSize, Alloc >::iterator
  segmented_vector< T, kSegmentSize, Alloc >::begin() 
  {
    return iterator( *this, Index( 0, 0 ) );
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  typename segmented_vector< T, kSegmentSize, Alloc >::const_iterator
  segmented_vector< T, kSegmentSize, Alloc >::end() const
  {
    return const_iterator( *this, Index( mFirstFreeSegment, mFirstFreeSeat[ mFirstFreeSegment ] ) );
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  typename segmented_vector< T, kSegmentSize, Alloc >::iterator
  segmented_vector< T, kSegmentSize, Alloc >::end()  
  {
    return iterator( *this, Index( mFirstFreeSegment, mFirstFreeSeat[ mFirstFreeSegment ] ) );
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  typename segmented_vector< T, kSegmentSize, Alloc >::const_reverse_iterator
  segmented_vector< T, kSegmentSize, Alloc >::rbegin() const
  {
    if ( mFirstFreeSeat[ mFirstFreeSegment ] == 0 ) {
      return const_reverse_iterator( *this, ReverseIndex( mFirstFreeSegment - 1, segment_size - 1 ) );
    } else {
      return const_reverse_iterator( *this, 
        ReverseIndex( mFirstFreeSegment, mFirstFreeSeat[ mFirstFreeSegment ] - 1 ) );
    }
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  typename segmented_vector< T, kSegmentSize, Alloc >::reverse_iterator
  segmented_vector< T, kSegmentSize, Alloc >::rbegin() 
  {
    if ( mFirstFreeSeat[ mFirstFreeSegment ] == 0 ) {
      return reverse_iterator( *this, ReverseIndex( mFirstFreeSegment - 1, segment_size - 1 ) );
    } else {
      return reverse_iterator( *this, 
        ReverseIndex( mFirstFreeSegment, mFirstFreeSeat[ mFirstFreeSegment ] - 1 ) );
    }
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  typename segmented_vector< T, kSegmentSize, Alloc >::const_reverse_iterator
  segmented_vector< T, kSegmentSize, Alloc >::rend() const
  {
    return const_reverse_iterator( *this, ReverseIndex( 
      std::numeric_limits< size_type >::max(), segment_size - 1 ) );
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  typename segmented_vector< T, kSegmentSize, Alloc >::reverse_iterator
  segmented_vector< T, kSegmentSize, Alloc >::rend() 
  {
    return reverse_iterator( *this, ReverseIndex( 
      std::numeric_limits< size_type >::max(), segment_size - 1 ) );
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  typename segmented_vector< T, kSegmentSize, Alloc >::const_reference
  segmented_vector< T, kSegmentSize, Alloc >::operator[]( size_type i ) const
  {
    assert( isValid( Index( i / segment_size, i % segment_size ) ) );
    return ( mData[i/segment_size][i%segment_size] );
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  typename segmented_vector< T, kSegmentSize, Alloc >::reference
  segmented_vector< T, kSegmentSize, Alloc >::operator[]( size_type i )
  {
    assert( isValid( Index( i / segment_size, i % segment_size ) ) );
    return ( mData[i/segment_size][i%segment_size] );
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  typename segmented_vector< T, kSegmentSize, Alloc >::reference
  segmented_vector< T, kSegmentSize, Alloc >::operator[]( const Index &i )
  {
    assert( isValid( i ) );
    return ( mData[i.mSegment][i.mPosition] );
  }

  template< typename T, unsigned int kSegmentSize, typename Alloc >
  typename segmented_vector< T, kSegmentSize, Alloc >::const_reference
  segmented_vector< T, kSegmentSize, Alloc >::operator[]( const Index &i ) const
  {
    assert( isValid( i ) );
    return ( mData[i.mSegment][i.mPosition] );
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  template< typename IndexT >
  inline typename segmented_vector< T, kSegmentSize, Alloc >::const_reference
  segmented_vector< T, kSegmentSize, Alloc >::get( const IndexT& index ) const
  {
    assert( isValid( index ) );
    return mData[index.mSegment][index.mPosition];
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  template< typename IndexT >
  inline typename segmented_vector< T, kSegmentSize, Alloc >::reference
  segmented_vector< T, kSegmentSize, Alloc >::get( const IndexT& index )
  {
    assert( isValid( index ) );
    return mData[index.mSegment][index.mPosition];
  }
  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  inline typename segmented_vector< T, kSegmentSize, Alloc >::const_reference
  segmented_vector< T, kSegmentSize, Alloc >::back(  ) const {
    assert( isValid( rend() ) );
    return *rend();
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  inline typename segmented_vector< T, kSegmentSize, Alloc >::reference
  segmented_vector< T, kSegmentSize, Alloc >::back(  ) {
    assert( isValid( rend() ) );
    return *rend();
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  template< typename IndexT >
  inline typename segmented_vector< T, kSegmentSize, Alloc >::const_pointer
  segmented_vector< T, kSegmentSize, Alloc >::getPointer( const IndexT& index ) const {
    //  assert( isValid(index)  );
    //  return mData[index.mSegment] + index.mPosition;
    if ( isValid(index) )
      return mData[index.mSegment] + index.mPosition;
    else 
      return NULL;
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  template< typename IndexT >
  inline typename segmented_vector< T, kSegmentSize, Alloc >::pointer
  segmented_vector< T, kSegmentSize, Alloc >::getPointer( const IndexT& index ) {
    if ( isValid(index) )
      return mData[index.mSegment] + index.mPosition;
    else 
      return NULL;
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  template< typename IndexT >
  inline typename segmented_vector< T, kSegmentSize, Alloc >::const_iterator
  segmented_vector< T, kSegmentSize, Alloc >::getIterator( const IndexT& index ) const {
    assert( isValid(index) || index == invalidIndex() );
    return const_iterator(*this,index);
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  template< typename IndexT >
  inline typename segmented_vector< T, kSegmentSize, Alloc >::iterator
  segmented_vector< T, kSegmentSize, Alloc >::getIterator( const IndexT& index ) {
    assert( isValid(index) || index == invalidIndex() );
    return iterator(*this,index);
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  template< typename IndexT >
  bool segmented_vector< T, kSegmentSize, Alloc >::isValid( const IndexT& index ) const {
    return ( index.mSegment < mNSegments ) && 
           ( index.mPosition < mFirstFreeSeat[index.mSegment] );
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  template< typename IndexT >
  IndexT segmented_vector< T, kSegmentSize, Alloc >::index( const pointer ptr ) const
  {
    IndexT index;
    for ( size_type i = 0; i < mNSegments; ++i ) {
      // we know that the array of pointers to traincars is contiguous in memory
      // and that the traincars are contiguous.
      T * segment = mData[i];
      if ( (ptr >= segment) && (ptr < &segment[mFirstFreeSeat[i]]) ) {
        index.mSegment = i;
        index.mPosition = ptr - segment;
        return index;
      }
    }

    assert( false );
    return index;
  }


  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  void segmented_vector< T, kSegmentSize, Alloc >::push_back( const_reference value )
  {
    pointer newItem = allocate();
    mAlloc.construct( newItem, value );
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  typename segmented_vector< T, kSegmentSize, Alloc >::pointer 
  segmented_vector< T, kSegmentSize, Alloc >::allocate()
  {
    pointer result;

    // do we need a new segment?
    if ( mFirstFreeSegment == mNSegments ) {
      appendSegment();
    }

    // the pointer is at the first free location of the first free segment
    result = mData[mFirstFreeSegment] + mFirstFreeSeat[ mFirstFreeSegment ]++;

    // if the segment is full, then we move the first free pointer to the next free
    while ( ( mFirstFreeSegment < mNSegments ) && 
      ( mFirstFreeSeat[mFirstFreeSegment] == segment_size ) ) {
      mFirstFreeSegment++;
    }
    return result;
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  template< typename DestroyFunctionT > 
  void segmented_vector< T, kSegmentSize, Alloc >::erase( 
    const ReverseIndex& index,
    const DestroyFunctionT& destroy )
  {
    // delete an element at a location index and move the last element
    // to fill the hole
    size_type fromSegment = mFirstFreeSegment;

    assert( isValid( index ) );

    while ( fromSegment > 0 && mFirstFreeSeat[fromSegment] == 0 ) {
      fromSegment--;
    }

    destroy( mData[index.mSegment][index.mPosition] );
    mAlloc.destroy(&mData[index.mSegment][index.mPosition] );

    assert( fromSegment >= 0 );

    mFirstFreeSeat[fromSegment]--;
    if ( (index.mSegment != fromSegment) || (index.mPosition != mFirstFreeSeat[fromSegment]) ) {
      mData[index.mSegment][index.mPosition] = mData[fromSegment][mFirstFreeSeat[fromSegment]];
    }
    mFirstFreeSegment = std::min( fromSegment, mFirstFreeSegment );
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  void segmented_vector< T, kSegmentSize, Alloc >::erase( const ReverseIndex& index )
  {
    erase( index, detail::NullDestructor< T >() );
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  template< typename PredT, typename DestroyFunctionT >
  void segmented_vector< T, kSegmentSize, Alloc >::eraseIf( const PredT& pred, 
    const DestroyFunctionT& destroy )
  {
    ReverseIndex index;
    for ( size_type i = 0; i < mNSegments; i++ ) {
      for ( size_type j = 0; i < mNSegments && j < mFirstFreeSeat[i]; j++ ) {
        if ( pred( mData[i][j] ) ) {
          index.mSegment = i;
          index.mPosition = j;
          erase( index, destroy );
          j--; // we erased current element and there is a new one here, retest
        }
      }
    }
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  template< typename PredT >
  void segmented_vector< T, kSegmentSize, Alloc >::eraseIf( const PredT& pred )
  {
    eraseIf( pred, detail::NullDestructor< T >() );
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  void segmented_vector< T, kSegmentSize, Alloc >::compact()
  {
    if ( (mNSegments == 0) || (mFirstFreeSeat[mNSegments - 1] != 0 ) ) {
      return;
    }

    while ( (mNSegments > 0) && (mFirstFreeSeat[mNSegments-1] == 0 ) ) {
      mAlloc.deallocate( mData.back(), segment_size );
      mData.pop_back();
      mFirstFreeSeat.pop_back();
      mNSegments--;
    }
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  template< typename SegmentFnT >
  int segmented_vector< T, kSegmentSize, Alloc >::iterateSegments( const SegmentFnT& func )
  {
    int result = 0;
    T b = begin();
    if ( size() > 0 ) 
      result = func(b,size());
    return result;
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  void segmented_vector< T, kSegmentSize, Alloc >::appendSegment()
  {
    mNSegments++;
    mData.push_back( mAlloc.allocate( segment_size ) );
    mFirstFreeSeat.push_back( 0 );
  }

  //------------------------------------------------------------------------------
  template< typename T, unsigned int kSegmentSize, typename Alloc >
  void segmented_vector< T, kSegmentSize, Alloc >::reserve( size_type n )
  {
    size_type segments_needed =
      static_cast<size_type>(std::ceil(n / static_cast<double>(segment_size)));

    for ( size_type i = mNSegments; i < segments_needed; ++i )
      appendSegment();
  }

} // namespace xylose

#endif // xylose_segmented_vector_hpp

