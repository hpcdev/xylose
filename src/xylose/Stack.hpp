#ifndef xylose_Stack_hpp
#define xylose_Stack_hpp

#include <memory>

#include <cassert>

namespace xylose {

  template< typename T, typename AllocT = std::allocator< T > >
  class Stack {
  public:

    //-- Types to provide std::vector semantics --//
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    static const size_type kInitialSize = 128;

    /// construct a stack with 0 elements and an initial capacity of 128
    Stack() :
      mData( NULL ),
      mSize( 0 ),
      mCapacity( kInitialSize ),
      mAlloc() {
      mData = mAlloc.allocate( kInitialSize );
    }

    /// copy constructor creates a copy of the right hand side's data
    Stack( const Stack& other ) :
      mData( NULL ),
      mSize( other.mSize ),
      mCapacity( other.mCapacity ),
      mAlloc( other.mAlloc ) {
      mData = mAlloc.allocate( other.mCapacity );
      std::uninitialized_copy( other.mData, other.mData + other.mSize, mData );
    }

    /// Destructor calls element destructors and deallocates memory
    ~Stack() {
      clear();
      mAlloc.deallocate( mData, mCapacity );
    }

    /// Determine if the container is empty
    bool empty() const {
      return mSize == 0;
    }

    /// Clear all elements from the stack; does not deallocate storage
    void clear() {
      for ( pointer p = mData; p != mData + mSize; ++p ) {
        mAlloc.destroy( p );
      }
      mSize = 0;
    }

    /// Get the container's size
    size_type size() const {
      return mSize;
    }

    /// Get the current capacity for the container
    size_type capacity() const {
      return mCapacity;
    }

    //-- Back Insertion Sequence Semantics --//
    /// Return a reference to the back of the stack
    reference back() {
      assert( !empty() );
      return mData[ mSize - 1 ];
    }

    /// Return a const reference to the back of the stack
    const_reference back() const {
      assert( !empty() );
      return mData[ mSize - 1 ];
    }

    /// Push an element onto the back of the stack
    void push_back( const_reference t ) {
      if ( mSize == mCapacity ) {
        reserve( mCapacity * 2 );
      }
      mAlloc.construct( mData + mSize++, t );
    }

    /// Pop an element from the back of the stack
    void pop_back() {
      assert( !empty() );
      mAlloc.destroy( mData + mSize - 1 );
      mSize--;
    }

    /// Get a reference to the nth element in the stack
    reference operator[]( size_type n ) {
      assert( n < mSize );
      return mData[ n ];
    }

    /// Get a const reference to the nth element in the stack
    const_reference operator[]( size_type n ) const {
      assert( n < mSize );
      return mData[ n ];
    }

    /// Reserve enough space for n elements
    void reserve( size_type n ) {
      if ( mCapacity < n ) {
        size_type oldCapacity = mCapacity;
        pointer oldData = mData;
        mCapacity = n;
        mData = mAlloc.allocate( mCapacity );
        std::uninitialized_copy( oldData, oldData + mSize, mData );
        mAlloc.deallocate( oldData, oldCapacity );
      }
    }

  private:

    pointer mData;
    size_type mSize;
    size_type mCapacity;
    AllocT mAlloc;

  };

}/* namespace xylose */

#endif // xylose_Stack_hpp
