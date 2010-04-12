/*==============================================================================
 * Public Domain Contributions 2010 United States Government                   *
 * as represented by the U.S. Air Force Research Laboratory.                   *
 * Portions copyright Copyright (C) 2010 Stellar Science                       *
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


#ifndef xylose_detail_Iterator_h
#define xylose_detail_Iterator_h

#include <xylose/detail/Iterator.hpp>

#include <xylose/Index.hpp>

#include <algorithm>

#include <cassert>

namespace xylose {
  namespace detail {

    /// Utility class to define types for the Iterator class.  This is the general
    /// template that provides non-const access to the pointed to object.
    /// @param SegVectorT segmented_vector type.
    /// @param kIsConst Boolean that determines the const-ness of the objects.
    template< typename SegVectorT, bool kIsConst >
    struct IteratorTypes {
      typedef typename SegVectorT::reference Reference;
      typedef typename SegVectorT::pointer Pointer;
      typedef SegVectorT& SegVectorReference;
      typedef SegVectorT* SegVectorPointer;
    };

    /// Template specialization of IteratorTypes for const only access.
    template< typename SegVectorT >
    struct IteratorTypes< SegVectorT, true > {
      typedef typename SegVectorT::const_reference Reference;
      typedef typename SegVectorT::const_pointer Pointer;
      typedef const SegVectorT& SegVectorReference;
      typedef const SegVectorT* SegVectorPointer;
    };

    /// Iterator is a generalization of a pointer. 
    ///
    /// This class implements a random access iterator to iterate through the 
    /// contents of a segmented_vector data structure. This single template class
    /// parametrizes the iteration on three things:
    ///   - The type of segmented_vector iterated over
    ///   - The direction of iteration
    ///   - The const-ness of the data structure iterated over
    ///
    /// @param SegVectorT The type of segmented_vector to iterate over.
    /// @param kIsForward Boolean that determines the direction of iteration.
    /// @param kIsConst Boolean that determines the const-ness of the data structure
    template< typename SegVectorT, bool kIsForward, bool kIsConst >
    class Iterator : public xylose::Index< SegVectorT::segment_size, kIsForward > {

      // ease the definition of input and return values.
      typedef IteratorTypes< SegVectorT, kIsConst > ItType;
      typedef typename ItType::Reference ReferenceReturn;
      typedef typename ItType::Pointer PointerReturn;
      typedef typename ItType::SegVectorReference SegVectorInput;
      typedef typename ItType::SegVectorPointer SegVectorStorage;

    public:
      typedef typename SegVectorT::size_type size_type;
      typedef ptrdiff_t difference_type;
      typedef typename SegVectorT::value_type value_type;
      typedef typename SegVectorT::reference reference;
      typedef typename SegVectorT::const_reference const_reference;
      typedef typename SegVectorT::pointer pointer;

      // required to work with std::sort
      typedef std::random_access_iterator_tag iterator_category;

      // convenience typedefs to easily access the utility templates
      typedef xylose::Index< SegVectorT::segment_size, kIsForward > Index;
      
      // These depend upon a template parameter, we need to explicitly tell
      // the compiler that they do so that name lookup will succeed.
      using Index::mSegment;
      using Index::mPosition;

      Iterator() :
        Index(),
        mData( NULL )
      {}

      Iterator( SegVectorInput train, const Index& index ) :
        Index( index ),
        mData( &train )
      {}

      Iterator( const Iterator& rhs ) :
        Index( rhs ),
        mData( rhs.mData )
      {}


      /// return a pointer 
      PointerReturn getPointer() const { return &(mData->get(*this));}

      /// Return an index 
      Index getIndex() const { return Index(*this);}

      /// Returns is a iterator is valid
      bool isValid() const {
        if ( mData )
          return mData->isValid(getIndex());
        else
          return false;
      }

      /// Assignment operator
      Iterator& operator=( const Iterator& rhs ) {
        mSegment = rhs.mSegment;
        mPosition = rhs.mPosition;
        mData = rhs.mData;
        return *this;
      }
      
      /// Equality comparison
      bool operator==( const Iterator& rhs ) const {
        return (
          ( mData == rhs.mData ) &&
          ( mPosition == rhs.mPosition ) && 
          ( mSegment == rhs.mSegment ) );
      }

      /// Inequality comparison
      bool operator!=( const Iterator& rhs ) const {
        return ( 
          ( mData != rhs.mData ) ||
          ( mPosition != rhs.mPosition ) || 
          ( mSegment != rhs.mSegment ) );
      }

      /// Less than comparator
      bool operator<( const Iterator& rhs ) const {
        if ( mSegment < rhs.mSegment  )
          return true;
        return ( mSegment == rhs.mSegment  ) && ( mPosition < rhs.mPosition );
      }

      /// Less than comparator
      bool operator<=( const Iterator& rhs ) const {
        if ( mSegment < rhs.mSegment  )
          return true;
        return ( mSegment == rhs.mSegment  ) && ( mPosition <= rhs.mPosition );
      }

      /// Dereference operator
      ReferenceReturn operator*() const {
        return mData->get( *this );
      }

      /// Member access operator
      PointerReturn operator->() const {
        return mData->getPointer( *this );
      }

      /// Pre-increment operator
      Iterator& operator++() {
        Index::operator++();
        return *this;
      }

      /// Post-increment operator
      Iterator operator++( int ) {
        Iterator old( *this );
        Index::operator++();
        return old;
      }

      /// Pre-decrement operator
      Iterator& operator--() {
        Index::operator--();
        return *this;
      }

      /// Post-decrement operator
      Iterator operator--( int ) {
        Iterator old( *this );
        Index::operator--();
        return old;
      }

      /// Addition of integers moves n spaces in the train
      Iterator& operator+=( long int n ) {
        Index::operator+=( n );
        return *this;
      }

      /// add n spaces to the left hand operand
      Iterator operator+( long int n ) {
        Index index = Index::operator+( n );
        Iterator result;
        result.mSegment = index.mSegment;
        result.mPosition = index.mPosition;
        result.mData = mData;
        return result;
      }

      /// Move n spaces backward in the train
      Iterator& operator-=( long int n ) {
        Index::operator-=( n );
        return *this;
      }

      /// subtract n spaces from the left hand operand
      Iterator operator-( long int n ) {
        Index index = Index::operator-( n );
        Iterator result;
        result.mSegment = index.mSegment;
        result.mPosition = index.mPosition;
        result.mData = mData;
        return result;
      }

      difference_type operator-( const Iterator& rhs ) {
        return Index::operator-( rhs );
      }

      /// Retrieve the element n spaces away
      ReferenceReturn operator[]( size_type n ) {
        return *( *this + n );
      }

    private:
      // Pointer to the segmented_vector
      SegVectorStorage mData;
    };

  } // namespace detail
} // namespace xylose

#endif // xylose_util_train_detail_Iterator_h

