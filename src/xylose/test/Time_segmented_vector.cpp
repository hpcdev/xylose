#define BOOST_TEST_MODULE segmented_vector_Timing

#include <xylose/segmented_vector.hpp>

#include <xylose/Timer.h>
#include <xylose/Vector.h>

#include <boost/test/unit_test.hpp>

#include <vector>
#include <cmath>


namespace {

const unsigned int len = 10000000u;
const unsigned int car_size = 10000u;

template < typename Vector >
struct SomeOp {
  Vector sum;
  SomeOp() : sum(0.0) {}
  void operator()( const Vector & v ) {
    sum += v;
  }
};

BOOST_AUTO_TEST_CASE( insertion ) {
  typedef xylose::Vector<double,3> Vector;
  using xylose::V3;

  xylose::Timer timer_car,
              timer_vec,
              timer_array;

  timer_car.wall_time_label =
  timer_vec.wall_time_label =
  timer_array.wall_time_label = "s;  ";

  timer_car.cpu_time_label =
  timer_vec.cpu_time_label =
  timer_array.cpu_time_label = "s (cpu)";

  /* lets first time assignment. */
  {
    xylose::segmented_vector< Vector, car_size > vec;
    timer_car.start();
    for ( unsigned int i = 0; i < len ; ++i ) {
      vec.push_back(V3(0.,1.,(double)i));
    }
    timer_car.stop();
  }

  {
    std::vector< Vector > vec;
    timer_vec.start();
    for ( unsigned int i = 0; i < len ; ++i ) {
      vec.push_back(V3(0.,1.,(double)i));
    }
    timer_vec.stop();
  }

  {
    Vector * array = NULL;
    timer_array.start();
    unsigned int current_size = 0; 
    for ( unsigned int i = 0; i < len ; ++i ) {
      // This was originally a realloc for every iteration, but
      // on griffin with mpich-gm, realloc became *terribly slow*
      // when approaching 'len' and caused this test to foul up
      // the unit testing system. Instead of using that simple 
      // approach, the following implementation doubles the 
      // allocation size every time a realloc is required. 
      // This is generally considered the best approach, 
      // but requires quite a bit more code that is highly
      // error-prone. -- K. R. Walker on 2009-12-04
      const std::size_t required_size = sizeof( Vector ) * ( i + 1 ); 
      if ( current_size < required_size ) {
        if ( !current_size ) {
          current_size = required_size;
        } else {
          current_size *= 2;
        }
        // Note that realloc() invokes undefined behavior when
        // Vector is a non-POD type (it currently is non-POD). 
        array = (Vector*)realloc( array, current_size );
      }    
      array[i] = V3(0.,1.,(double)i);
    }    
    timer_array.stop();
    free(array);
  }

  BOOST_TEST_MESSAGE( "xylose::vec: " << timer_car );
  BOOST_TEST_MESSAGE( "   std::vec: " << timer_vec );
  BOOST_TEST_MESSAGE( "      array: " << timer_array );
  BOOST_CHECK( true );
}

BOOST_AUTO_TEST_CASE( iterator ) {
  typedef xylose::Vector<double,3> Vector;
  using xylose::V3;

  xylose::Timer timer_car,
              timer_vec,
              timer_array;

  timer_car.wall_time_label =
  timer_vec.wall_time_label =
  timer_array.wall_time_label = "s;  ";

  timer_car.cpu_time_label =
  timer_vec.cpu_time_label =
  timer_array.cpu_time_label = "s (cpu)";


  /* Time the iterator. */
  SomeOp<Vector> someOp;

  {
    xylose::segmented_vector< Vector, car_size > vec;
    /* assign everything. */
    for ( unsigned int i = 0; i < len ; ++i ) {
      vec.push_back(V3(0.,1.,(double)i));
    }

    timer_car.start();
    SomeOp<Vector> r = std::for_each(vec.begin(), vec.end(), someOp);
    timer_car.stop();
    BOOST_TEST_MESSAGE( "xylose::vec sum: " << r.sum.to_string(',') );
  }

  {
    std::vector< Vector > vec;
    /* assign everything. */
    for ( unsigned int i = 0; i < len ; ++i ) {
      vec.push_back(V3(0.,1.,(double)i));
    }

    timer_vec.start();
    SomeOp<Vector> r = std::for_each(vec.begin(), vec.end(), someOp);
    timer_vec.stop();
    BOOST_TEST_MESSAGE( " std::vec sum: " << r.sum.to_string(',') );
  }

  {
    Vector * array = NULL;
    /* assign everything. */
    array = (Vector*)malloc(sizeof(Vector) * len);
    for ( unsigned int i = 0; i < len ; ++i ) {
      array[i] = V3(0.,1.,(double)i);
    }

    timer_array.start();
    SomeOp<Vector> r = std::for_each(array, array+len, someOp);
    timer_array.stop();
    BOOST_TEST_MESSAGE( "    array sum: " << r.sum.to_string(',') );
    free(array);
  }

  BOOST_TEST_MESSAGE( "xylose::vec: " << timer_car );
  BOOST_TEST_MESSAGE( "   std::vec: " << timer_vec );
  BOOST_TEST_MESSAGE( "      array: " << timer_array );
  BOOST_CHECK( true );
}

} // namespace anon

