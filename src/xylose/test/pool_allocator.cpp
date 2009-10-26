#define BOOST_TEST_MODULE pool_allocator

#include <xylose/pool_allocator.hpp>

#include <xylose/segmented_vector.hpp>

#include <boost/test/unit_test.hpp>
#include <iostream>

namespace {
  struct parent {};

  template < typename T, typename _Alloc >
  struct A : public T {
    typedef typename _Alloc::template rebind<A>::other Alloc;

    Alloc allocator;
  };
}


BOOST_AUTO_TEST_CASE( rebind ) {
  typedef A< parent, xylose::pool_allocator<int> > AA;
  AA a;

  /** FIXME:  need to have some real tests here. */
  BOOST_CHECK_EQUAL( 0, 0 );
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

  /** FIXME:  need to have some real tests here. */
  BOOST_CHECK_EQUAL( 0, 0 );
}

