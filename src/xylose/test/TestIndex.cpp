#include <xylose/Index.hpp>

#define BOOST_TEST_MODULE Index 
#include <boost/test/unit_test.hpp>

namespace {

BOOST_AUTO_TEST_CASE( increment )
{
  xylose::Index< 2, true > test(0);
  BOOST_CHECK_EQUAL( test.mSegment, 0u );
  BOOST_CHECK_EQUAL( test.mPosition, 0u );

  ++test;
  BOOST_CHECK_EQUAL( test.mSegment, 0u );
  BOOST_CHECK_EQUAL( test.mPosition, 1u );

  ++test;
  BOOST_CHECK_EQUAL( test.mSegment, 1u );
  BOOST_CHECK_EQUAL( test.mPosition, 0u );
}

} // namespace anon

