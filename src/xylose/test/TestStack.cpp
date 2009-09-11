#include <xylose/Stack.hpp>

#define BOOST_TEST_MODULE( Stack )
#include <boost/test/unit_test.hpp>

namespace {
  typedef xylose::Stack< int > IntStack;
} // namespace anon

BOOST_AUTO_TEST_SUITE( StackSuite );

BOOST_AUTO_TEST_CASE( construct )
{
  IntStack stack;

  BOOST_CHECK_EQUAL( stack.size(), 0 );
  BOOST_CHECK_EQUAL( stack.capacity(), 128 );
}

BOOST_AUTO_TEST_CASE( back )
{
  IntStack stack;
  for ( int i = 0; i < 5; ++i ) {
    stack.push_back( i );
    BOOST_CHECK_EQUAL( stack.back(), i );
  }
}

BOOST_AUTO_TEST_CASE( push_back )
{
  IntStack stack;

  int i;

  for ( i = 0; i < 128; ++i ) {
    stack.push_back( i );
  }

  BOOST_CHECK_EQUAL( stack.size(), 128 );
  BOOST_CHECK_EQUAL( stack.capacity(), 128 );
  
  stack.push_back( i++ );
  BOOST_CHECK_EQUAL( stack.size(), 129 );
  BOOST_CHECK_EQUAL( stack.capacity(), 256 );

  for ( i = 0; i < 129; ++i ) {
    BOOST_CHECK_EQUAL( stack[i], i );
  }
}

BOOST_AUTO_TEST_CASE( reserve )
{
  IntStack stack;

  BOOST_CHECK_EQUAL( stack.capacity(), 128 );

  stack.reserve( 64 );
  BOOST_CHECK_EQUAL( stack.capacity(), 128 );

  stack.reserve( 200 );
  BOOST_CHECK_EQUAL( stack.capacity(), 200 );
  stack.reserve( 128 );
  BOOST_CHECK_EQUAL( stack.capacity(), 200 );
}

BOOST_AUTO_TEST_SUITE_END();

