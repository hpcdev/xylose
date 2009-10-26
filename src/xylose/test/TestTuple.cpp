#include "Tuple.hpp"

#define BOOST_TEST_MODULE Tuple

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( valuePair )
{
  typedef Tuple< int, double > ValPair;

  int intVal = 42;
  double doubleVal = 3.14;

  ValPair test( intVal, doubleVal );
  BOOST_CHECK_EQUAL( test.head(), intVal );
  BOOST_CHECK_EQUAL( test.tail(), doubleVal );

  test.head() = 84;
  test.tail() = 6.28;
  BOOST_CHECK_EQUAL( test.head(), 84 );
  BOOST_CHECK_EQUAL( test.tail(), 6.28 );

  // make sure the initial values did not get twiddled by the above code
  BOOST_CHECK_EQUAL( intVal, 42 );
  BOOST_CHECK_EQUAL( doubleVal, 3.14 );
}

BOOST_AUTO_TEST_CASE( referencePair )
{
  typedef Tuple< int&, double& > RefPair;

  int intVal = 42;
  double doubleVal = 3.14;

  RefPair test( intVal, doubleVal );
  BOOST_CHECK_EQUAL( &test.head(), &intVal );
  BOOST_CHECK_EQUAL( &test.tail(), &doubleVal );

  test.head() = 84;
  test.tail() = 6.28;
  BOOST_CHECK_EQUAL( test.head(), 84 );
  BOOST_CHECK_EQUAL( test.tail(), 6.28 );

  // make sure the initial values did get twiddled by the above code
  BOOST_CHECK_EQUAL( intVal, 84 );
  BOOST_CHECK_EQUAL( doubleVal, 6.28 );
}

BOOST_AUTO_TEST_CASE( swapPair )
{
  typedef Tuple< int&, double& > RefPair;
  int a = 0;
  double b = 10.0;
  int c = 1;
  double d = 11.0;

  RefPair p1( a, b );
  RefPair p2( c, d );

  swap( p1, p2 );

  BOOST_CHECK_EQUAL( a, 1 );
  BOOST_CHECK_EQUAL( b, 11.0 );
  BOOST_CHECK_EQUAL( c, 0 );
  BOOST_CHECK_EQUAL( d, 10.0 );
}

BOOST_AUTO_TEST_CASE( poorMansSwap )
{
  // the purpose of this test is to check whether or not the swap
  // implementation used by GCC's STL implementation of iter_swap
  // works with Tuples.
  typedef Tuple< int, double > value_type;
  typedef Tuple< int&, double& > reference;

  int a = 0;
  double b = 10.0;
  int c = 1;
  double d = 11.0;

  reference r1( a, b );
  reference r2( c, d );

  value_type tmp = r1;
  r1 = r2;
  r2 = tmp;

  BOOST_CHECK_EQUAL( a, 1 );
  BOOST_CHECK_EQUAL( b, 11.0 );
  BOOST_CHECK_EQUAL( c, 0 );
  BOOST_CHECK_EQUAL( d, 10.0 );
}

