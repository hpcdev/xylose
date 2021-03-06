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


#include <xylose/segmented_vector.hpp>
#include <xylose/Swap.hpp>

#define BOOST_TEST_MODULE segmented_vector

#include <boost/test/unit_test.hpp>

#include <cstdlib>

namespace {
  
typedef xylose::segmented_vector< int, 2 > Test_segmented_vector;

struct predicate {
  bool operator()( int value ) const {
    return ( value == 1 || value == 2 ) ? true : false;
  }
};

BOOST_AUTO_TEST_CASE( size )
{
  Test_segmented_vector test;

  for ( int i = 0; i < 10; i++ ) {
    test.push_back( i );
  }

  BOOST_CHECK_EQUAL( test.size(), 10u );
}

BOOST_AUTO_TEST_CASE( begin )
{
  Test_segmented_vector test;
  
  for ( int i = 0; i < 10; i++ ) {
    test.push_back( i );
  }

  Test_segmented_vector::iterator it = test.begin();
  BOOST_CHECK_EQUAL( *it, 0 );
}

BOOST_AUTO_TEST_CASE( constBegin )
{
  Test_segmented_vector test;
  
  for ( int i = 0; i < 10; i++ ) {
    test.push_back( i );
  }

  const Test_segmented_vector& constTest = test;

  Test_segmented_vector::const_iterator it = constTest.begin();
  BOOST_CHECK_EQUAL( *it, 0 );
}

BOOST_AUTO_TEST_CASE( end )
{
  Test_segmented_vector test;
  
  test.push_back( 1 );
  Test_segmented_vector::iterator result1 = test.end();
  BOOST_CHECK_EQUAL( result1.mSegment, 0u );
  BOOST_CHECK_EQUAL( result1.mPosition, 1u );
  
  test.push_back( 1 );
  Test_segmented_vector::iterator result2 = test.end();
  BOOST_CHECK_EQUAL( result2.mSegment, 1u );
  BOOST_CHECK_EQUAL( result2.mPosition, 0u );
}

BOOST_AUTO_TEST_CASE( constEnd )
{
  Test_segmented_vector test;
  const Test_segmented_vector& constTest = test;
  
  test.push_back( 1 );
  Test_segmented_vector::const_iterator result1 = constTest.end();
  BOOST_CHECK_EQUAL( result1.mSegment, 0u );
  BOOST_CHECK_EQUAL( result1.mPosition, 1u );
  
  test.push_back( 1 );
  Test_segmented_vector::const_iterator result2 = constTest.end();
  BOOST_CHECK_EQUAL( result2.mSegment, 1u );
  BOOST_CHECK_EQUAL( result2.mPosition, 0u );
}

BOOST_AUTO_TEST_CASE( rbegin )
{
  Test_segmented_vector test;
  for ( int i = 0; i < 10; ++i ) {
    test.push_back( i );
  }

  Test_segmented_vector::reverse_iterator rit = test.rbegin();
  BOOST_CHECK_EQUAL( *rit, 9 );
}

BOOST_AUTO_TEST_CASE( rend )
{
  Test_segmented_vector test;
  test.push_back( 1 );
  
  Test_segmented_vector::reverse_iterator rit = test.rend();
  BOOST_CHECK_EQUAL( rit.mSegment, std::numeric_limits< Test_segmented_vector::size_type >::max() );
  BOOST_CHECK_EQUAL( rit.mPosition, 1u );
}

BOOST_AUTO_TEST_CASE( clear )
{
  Test_segmented_vector test;
  for ( int i = 0; i < 100; ++i ) {
    test.push_back( rand() );
  }

  Test_segmented_vector::iterator end = test.end();
  BOOST_REQUIRE_EQUAL( end.mSegment, 50u );
  BOOST_REQUIRE_EQUAL( end.mPosition, 0u );

  test.clear();
  end = test.end();
  BOOST_CHECK_EQUAL( end.mSegment, 0u );
  BOOST_CHECK_EQUAL( end.mPosition, 0u );
}

BOOST_AUTO_TEST_CASE( sort )
{
  Test_segmented_vector test;
  test.push_back( 2 );
  test.push_back( 3 );
  test.push_back( 1 );
  test.push_back( 4 );
 
  BOOST_REQUIRE_EQUAL( test.begin(), Test_segmented_vector::iterator( test, Test_segmented_vector::Index( 0, 0 ) ) );
  BOOST_REQUIRE_EQUAL( test.end(), Test_segmented_vector::iterator( test, Test_segmented_vector::Index( 2, 0 ) ) );
  BOOST_REQUIRE_EQUAL( test.end() - test.begin(), 4 );
  std::sort( test.begin(), test.end() );

  BOOST_CHECK_EQUAL( test[0], 1 );
  BOOST_CHECK_EQUAL( test[1], 2 );
  BOOST_CHECK_EQUAL( test[2], 3 );
  BOOST_CHECK_EQUAL( test[3], 4 );
}

BOOST_AUTO_TEST_CASE( largeSort )
{
  typedef xylose::segmented_vector< int, 1000 > LargeContainer;
  LargeContainer test;
  for ( int i = 0; i < 100000; ++i ) {
    test.push_back( rand() );
  }

  std::sort( test.begin(), test.end() );

  LargeContainer::iterator it, end = test.end() - 1;
  BOOST_TEST_MESSAGE( "BEGIN Failure Only Logging" );
  for ( it = test.begin(); it != end; ++it ) {
    if ( ! ( *it <= *( it + 1 ) ) ) {
      BOOST_CHECK( *it <= *( it + 1 ) );
    }
  }
  BOOST_TEST_MESSAGE( "END Failure Only Logging" );
}

BOOST_AUTO_TEST_CASE( get )
{
  Test_segmented_vector test;

  test.push_back( 0 );
  test.push_back( 1 );
  test.push_back( 2 );
  test.push_back( 3 );

  BOOST_CHECK_EQUAL( test.get( Test_segmented_vector::Index( 0, 0 ) ), 0 );
  BOOST_CHECK_EQUAL( test.get( Test_segmented_vector::Index( 0, 1 ) ), 1 );
  BOOST_CHECK_EQUAL( test.get( Test_segmented_vector::Index( 1, 0 ) ), 2 );
  BOOST_CHECK_EQUAL( test.get( Test_segmented_vector::Index( 1, 1 ) ), 3 );
}

BOOST_AUTO_TEST_CASE( getPointer )
{
  Test_segmented_vector test;

  test.push_back( 1 );
  test.push_back( 2 );

  BOOST_CHECK_EQUAL( *test.getPointer( Test_segmented_vector::Index( 0, 0 ) ), 1 );
  BOOST_CHECK_EQUAL( *test.getPointer( Test_segmented_vector::Index( 0, 1 ) ), 2 );
}

BOOST_AUTO_TEST_CASE( indexPtr )
{
  Test_segmented_vector test;

  test.push_back( 1 );
  test.push_back( 2 );

  int* ptr = test.getPointer( Test_segmented_vector::Index( 0, 1 ) );
  Test_segmented_vector::Index result = test.index< Test_segmented_vector::Index >( ptr );

  BOOST_CHECK_EQUAL( result.mSegment, 0u );
  BOOST_CHECK_EQUAL( result.mPosition, 1u );
}

BOOST_AUTO_TEST_CASE( erase )
{
  Test_segmented_vector test;
  test.push_back( 0 );
  test.push_back( 1 );
  test.push_back( 2 );
  test.push_back( 3 );

  Test_segmented_vector::ReverseIndex index( 0, 1 );
  test.erase( index );
  // final element should be moved to erased element's place
  BOOST_CHECK_EQUAL( test.get( index ), 3 );
}

BOOST_AUTO_TEST_CASE( eraseIf )
{
  Test_segmented_vector test;
  test.push_back( 0 );
  test.push_back( 1 );
  test.push_back( 2 );
  test.push_back( 3 );

  test.eraseIf( predicate() );

  BOOST_CHECK_EQUAL( test.get( Test_segmented_vector::Index( 0, 0 ) ), 0 );
  BOOST_CHECK_EQUAL( test.get( Test_segmented_vector::Index( 0, 1 ) ), 3 );
}

BOOST_AUTO_TEST_CASE( vectorOfTrainsBug )
{
  typedef xylose::segmented_vector< int, 913 > Train;
  std::vector< Train > v;
  v.resize( 3 );
  v[0].push_back( 42 );

  BOOST_CHECK_EQUAL( v[0][0], 42 );
}

BOOST_AUTO_TEST_CASE( swapTest )
{
  Test_segmented_vector src;
  Test_segmented_vector dst;
  
  for ( int i = 0; i < 10; i++ ) {
    src.push_back( i );
  }

  ::xylose::swap( src, dst );
  BOOST_CHECK_EQUAL( src.size(),    0u );
  BOOST_CHECK_EQUAL( dst.size(),   10u );
  BOOST_CHECK_EQUAL( *dst.begin(),   0 );
  BOOST_CHECK_EQUAL( *dst.rbegin(),  9 );
}

BOOST_AUTO_TEST_CASE( reserve )
{
  Test_segmented_vector test;
  
  BOOST_CHECK_EQUAL( test.capacity(), 0u );
  test.reserve( 41 );
  unsigned int cap = static_cast<unsigned int>(
    std::ceil(41. / test.segment_size) * test.segment_size
  );
  BOOST_CHECK_EQUAL( test.size(), 0u );
  BOOST_CHECK_EQUAL( test.capacity(), cap );
}

BOOST_AUTO_TEST_CASE( resize )
{
  Test_segmented_vector test;
  
  BOOST_CHECK_EQUAL( test.capacity(), 0u );
  test.resize( 41 );
  unsigned int cap = static_cast<unsigned int>(
    std::ceil(41. / test.segment_size) * test.segment_size
  );
  BOOST_CHECK_EQUAL( test.size(), 41u );
  BOOST_CHECK_EQUAL( test.capacity(), cap );
}


BOOST_AUTO_TEST_SUITE( Iterator );

BOOST_AUTO_TEST_CASE( equal )
{
  Test_segmented_vector test;
  Test_segmented_vector::iterator a( test, Test_segmented_vector::Index( 0, 0 ) );
  Test_segmented_vector::iterator b( test, Test_segmented_vector::Index( 0, 0 ) );
  Test_segmented_vector::iterator c( test, Test_segmented_vector::Index( 0, 1 ) );
  
  BOOST_CHECK( a == b );
  BOOST_CHECK( !( a == c ) );
  BOOST_CHECK( a != c );
}

BOOST_AUTO_TEST_CASE( lessThan )
{
  Test_segmented_vector test;
  for ( int i = 0; i < 10; ++i ) {
    test.push_back( i );
  }

  Test_segmented_vector::iterator i, j;
  BOOST_TEST_MESSAGE( "BEGIN Failure Only Logging" );
  for ( i = test.begin(); i != test.end(); ++i ) {
    for ( j = i + 1; j != test.end(); ++j ) {
      if ( ! ( i < j ) ) {
        BOOST_CHECK( i < j );
      }
    }
    if ( i != test.begin() ) {
      for ( j = i - 1; j != test.begin(); --j ) {
        if ( ! ( ! ( i < j ) ) ) {
          BOOST_CHECK( !( i < j ) );
        }
      }
      if ( ! ( ! ( i < test.begin() ) ) ) {
        BOOST_CHECK( !( i < test.begin() ) );
      }
    }
  }
  BOOST_TEST_MESSAGE( "END Failure Only Logging" );
}

BOOST_AUTO_TEST_CASE( dereference )
{
  Test_segmented_vector test;

  test.push_back( 42 );
  Test_segmented_vector::iterator it = test.begin();

  BOOST_CHECK_EQUAL( *it, 42 );

  *it = 84;
  BOOST_CHECK_EQUAL( test[0], 84 );
}

BOOST_AUTO_TEST_CASE( increment )
{
  Test_segmented_vector test;

  for ( int i = 0; i < 10; ++i ) {
    test.push_back( i );
  }

  Test_segmented_vector::iterator it;
  int i = 0;
  for ( it = test.begin(); it != test.end(); ++it, i++ ) {
    BOOST_CHECK_EQUAL( *it, i );
  }
}

BOOST_AUTO_TEST_CASE( decrement )
{
  Test_segmented_vector test;
  for ( int i = 0; i < 10; ++i ) {
    test.push_back( i );
  }

  Test_segmented_vector::iterator it = test.end();
  BOOST_REQUIRE_EQUAL( it.mSegment, 5u );
  BOOST_REQUIRE_EQUAL( it.mPosition, 0u );
  --it;
  BOOST_REQUIRE_EQUAL( *it, 9 );

  for ( int i = 9; i > 0; --i ) {
    --it;
  }
  BOOST_REQUIRE_EQUAL( *it, 0 );

  --it;
  --it;
  ++it;
  ++it;
  BOOST_CHECK_EQUAL( *it, 0 );
}

BOOST_AUTO_TEST_CASE( reverseIncrement )
{
  Test_segmented_vector test;
  for ( int i = 0; i < 10; ++i ) {
    test.push_back( i );
  }
  
  Test_segmented_vector::reverse_iterator rit;
  int value = 9;
  for ( rit = test.rbegin(); rit != test.rend(); ++rit ) {
    BOOST_CHECK_EQUAL( *rit, value-- );
  }
}

BOOST_AUTO_TEST_CASE( reverseDecrement )
{
  Test_segmented_vector test;
  for ( int i = 0; i < 10; ++i ) {
    test.push_back( i );
  }

  Test_segmented_vector::reverse_iterator rit = test.rend();
  rit--;
  int value = 0;
  BOOST_CHECK_EQUAL( *rit, value );
  for ( ; rit != test.rbegin(); --rit ) {
    BOOST_CHECK_EQUAL( *rit, value++ );
  }
  BOOST_CHECK_EQUAL( *rit, 9 );
}

BOOST_AUTO_TEST_CASE( subtract )
{
  Test_segmented_vector::Index lhs( 4, 1 );

  BOOST_CHECK_EQUAL( lhs - 0, Test_segmented_vector::Index( 4, 1 ) );
  BOOST_CHECK_EQUAL( lhs - 1, Test_segmented_vector::Index( 4, 0 ) );
  BOOST_CHECK_EQUAL( lhs - 2, Test_segmented_vector::Index( 3, 1 ) );
  BOOST_CHECK_EQUAL( lhs - 3, Test_segmented_vector::Index( 3, 0 ) );
  BOOST_CHECK_EQUAL( lhs - 4, Test_segmented_vector::Index( 2, 1 ) );
  BOOST_CHECK_EQUAL( lhs - 5, Test_segmented_vector::Index( 2, 0 ) );
  BOOST_CHECK_EQUAL( lhs - 6, Test_segmented_vector::Index( 1, 1 ) );
  BOOST_CHECK_EQUAL( lhs - 7, Test_segmented_vector::Index( 1, 0 ) );
  BOOST_CHECK_EQUAL( lhs - 8, Test_segmented_vector::Index( 0, 1 ) );
  BOOST_CHECK_EQUAL( lhs - 9, Test_segmented_vector::Index( 0, 0 ) );
}

BOOST_AUTO_TEST_CASE( distance )
{
  Test_segmented_vector::Index lhs( 1, 1 );
  
  BOOST_CHECK_EQUAL( lhs - Test_segmented_vector::Index( 1, 1 ), 0 );
  BOOST_CHECK_EQUAL( lhs - Test_segmented_vector::Index( 1, 0 ), 1 );
  BOOST_CHECK_EQUAL( lhs - Test_segmented_vector::Index( 0, 1 ), 2 );
  BOOST_CHECK_EQUAL( lhs - Test_segmented_vector::Index( 0, 0 ), 3 );
}

BOOST_AUTO_TEST_CASE( backwards )
{
  Test_segmented_vector test;
  for ( int i = 0; i < 10; ++i ) {
    test.push_back( i );
  }

  Test_segmented_vector::iterator it = test.end();
  --it;
  int value = 9;
  for ( ; it != test.begin(); --it ) {
    BOOST_CHECK_EQUAL( *it, value );
    --value;
  }
}

BOOST_AUTO_TEST_CASE( subtractInteger )
{
  typedef xylose::segmented_vector< int, 20 > Test;
  Test test;

  for ( int i = 0; i < 90; ++i ) {
    test.push_back( i );
  }

  Test::iterator start = test.begin() + 50;
  BOOST_CHECK_EQUAL( start.mSegment, 2u );
  BOOST_CHECK_EQUAL( start.mPosition, 10u );
  BOOST_CHECK_EQUAL( *start, 50 );

  Test::iterator it = start - 20;
  BOOST_CHECK_EQUAL( it.mSegment, 1u );
  BOOST_CHECK_EQUAL( it.mPosition, 10u );
  BOOST_CHECK_EQUAL( *it, 30 );

  it = start - 50;
  BOOST_CHECK_EQUAL( it.mSegment, 0u );
  BOOST_CHECK_EQUAL( it.mPosition, 0u );
  BOOST_CHECK_EQUAL( *it, 0 );

  // FIXME these don't pass at the moment
  //it = start - 55;
  //it = start + 55;
  //BOOST_CHECK_EQUAL( it.mSegment, 2u );
  //BOOST_CHECK_EQUAL( it.mPosition, 10u );
  //BOOST_CHECK_EQUAL( *it, 50 );
}

BOOST_AUTO_TEST_SUITE_END(); //Iterator

} // namespace anon

