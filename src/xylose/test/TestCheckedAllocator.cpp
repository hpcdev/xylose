#include "CheckedAllocator.hpp"

// #defines of physical constants should be outlawed!
// seriously, Boost uses C as a template parameter, so we need to make
// sure it's not defined going in here.
#undef C

#define BOOST_TEST_MODULE CheckedAllocator
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( construct )
{
  CheckedAllocator< int > alloc;
  int* ptr = alloc.allocate( 1 );
  alloc.construct( ptr, 42 );
  BOOST_CHECK_EQUAL( *ptr, 42 );
  alloc.deallocate( ptr, 1 );
}

