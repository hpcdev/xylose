#define BOOST_TEST_MODULE TypedFactory
#include <boost/test/unit_test.hpp>

#include <xylose/Factory.hpp>
#include <xylose/TypedFactory.hpp>

#include <memory>

namespace {

class Base {
public:
  Base() {}
  virtual ~Base() {}
  virtual int value() { return 0; }
};

typedef xylose::Factory< Base, int > BaseFactory;

class Derived : public Base {
public:
  Derived( const int& ) {}
  virtual ~Derived() {}
  virtual int value() { return 1; }
};

typedef xylose::TypedFactory< Base, Derived, int > DerivedFactory;

BOOST_AUTO_TEST_CASE( create ) {
  std::auto_ptr< BaseFactory > factory( new DerivedFactory );
  std::auto_ptr< Base > object( factory->create( 0 ) );
  BOOST_CHECK_EQUAL( 1, object->value() );
}

} // namespace

