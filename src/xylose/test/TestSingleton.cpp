#include <xylose/Singleton.hpp>

#define BOOST_TEST_MODULE Singleton
#include <boost/test/unit_test.hpp>

// run this test through valgrind to ensure we are not leaking the instance!

namespace {

class Test : public xylose::Singleton< Test > {
public:
  int x;
private:
  Test() : x( 42 ) {}
  ~Test() {}
  
  friend class xylose::Singleton< Test >;
};

BOOST_AUTO_TEST_CASE( constructor ) {
  BOOST_CHECK_EQUAL( Test::instance()->x, 42 );
}

BOOST_AUTO_TEST_CASE( instance ) {
  BOOST_CHECK_EQUAL( Test::instance(), Test::instance() );
}

} // namespace anon

