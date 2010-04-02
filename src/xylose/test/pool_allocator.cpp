#define BOOST_TEST_MODULE pool_allocator

#include <xylose/pool_allocator.hpp>
#include <xylose/logger.h>
#include <xylose/segmented_vector.hpp>
#include <xylose/random/Kiss.hpp>

#include <iostream>
#include <map>

#include <boost/test/unit_test.hpp>

namespace {

static int initLogger() {
  xylose::logger::setLogProgramName("pool_allocator test");
  return 1;
}

static const int init_log = initLogger();

struct parent {};

template < typename T, typename _Alloc >
struct A : public T {
  typedef typename _Alloc::template rebind<A>::other Alloc;

  Alloc allocator;
};

BOOST_AUTO_TEST_CASE( log_initialization ) {
  /* just a dummy test case to make sure that the logger gets initialized. */
  BOOST_CHECK_EQUAL( init_log, 1 );
}

BOOST_AUTO_TEST_CASE( rebind ) {
  typedef A< parent, xylose::pool_allocator<int> > AA;
  AA a;
}

BOOST_AUTO_TEST_CASE( direct_allocation ) {
  xylose::pool_allocator<int, xylose::make_stl_container<1000000u>::type > alloc;
  xylose::pool_allocator<int> alloc2;

  int * a0 = alloc.allocate(1);
  int * a1 = alloc.allocate(1);
  alloc.deallocate(a0);

        a0 = alloc.allocate(1);

  alloc.deallocate(a0);
  alloc.deallocate(a1);


  /* and now for a much larger set of random allocations and deallocations. */
  std::vector<int*> pointers;
  xylose::random::Kiss r;
  for ( unsigned int i = 0; i < 100000; ++i ) {
    if ( r.rand() < 0.85 ) /* bias towards allocation */
      pointers.push_back( alloc.allocate(1) );
    else {
      int idx = static_cast<int>(r.randExc() * pointers.size());
      if ( pointers.size() && pointers[idx] ) {
        alloc.deallocate( pointers[idx] );
        pointers[idx] = NULL;
      }
    }
  }

  /* now dealloc them. */
  while ( pointers.size() ) {
    if ( pointers.back() )
      alloc.deallocate( pointers.back() );
    pointers.pop_back();
  }

  /* This call essentially serves to test whether all items have been
   * deallocated. */
  alloc.reset();
}

BOOST_AUTO_TEST_CASE( std_map_test ) {
  std::map<std::string, int, std::less<std::string>, xylose::pool_allocator<std::string> > foo;
  //std::map<std::string, int, std::less<std::string>, std::allocator<std::string> > foo;
  std::string a="asdf";
  foo[a] = 0;
  ++foo[a];

  BOOST_CHECK_EQUAL( foo[a], 1 );
}

}

