#include <xylose/Index.hpp>

#define BOOST_TEST_MODULE Index 
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( Index );

BOOST_AUTO_TEST_CASE( increment )
{
  xylose::Index< 2, true > test(0);
  BOOST_CHECK_EQUAL( test.mSegment, 0 );
  BOOST_CHECK_EQUAL( test.mPosition, 0 );

  ++test;
  BOOST_CHECK_EQUAL( test.mSegment, 0 );
  BOOST_CHECK_EQUAL( test.mPosition, 1 );

  ++test;
  BOOST_CHECK_EQUAL( test.mSegment, 1 );
  BOOST_CHECK_EQUAL( test.mPosition, 0 );
}

BOOST_AUTO_TEST_SUITE_END();

