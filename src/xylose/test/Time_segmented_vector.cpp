#define BOOST_TEST_MODULE  segmented_vector_Timing

#include <xylose/segmented_vector.hpp>

#include <xylose/Timer.h>
#include <xylose/Vector.h>

#include <boost/test/unit_test.hpp>

#include <vector>
#include <iostream>
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
}


BOOST_AUTO_TEST_SUITE( segmented_vector_Timing );

  BOOST_AUTO_TEST_CASE( insertion ) {
    typedef xylose::Vector<double,3> Vector;

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
        vec.push_back(V3(0,1,(double)i));
      }
      timer_car.stop();
    }

    {
      std::vector< Vector > vec;
      timer_vec.start();
      for ( unsigned int i = 0; i < len ; ++i ) {
        vec.push_back(V3(0,1,(double)i));
      }
      timer_vec.stop();
    }

    {
      Vector * array = NULL;
      timer_array.start();
      for ( unsigned int i = 0; i < len ; ++i ) {
        array = (Vector*)realloc(array, sizeof(Vector)*(i+1));
        array[i] = V3(0,1,(double)i);
      }
      timer_array.stop();
      free(array);
    }

    std::cout << "insertion timers:\n"
                 "xylose::vec:  " << timer_car << "\n"
                 " std::vec:  " << timer_vec << "\n"
                 "    array:  " << timer_array << std::endl;

    BOOST_CHECK_EQUAL( true, true );
  }

  BOOST_AUTO_TEST_CASE( iterator ) {
    typedef xylose::Vector<double,3> Vector;

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
        vec.push_back(V3(0,1,(double)i));
      }

      timer_car.start();
      SomeOp<Vector> r = std::for_each(vec.begin(), vec.end(), someOp);
      timer_car.stop();
      std::cout << "xylose::vec sum:  " << r.sum.to_string(',') << '\n';
    }

    {
      std::vector< Vector > vec;
      /* assign everything. */
      for ( unsigned int i = 0; i < len ; ++i ) {
        vec.push_back(V3(0,1,(double)i));
      }

      timer_vec.start();
      SomeOp<Vector> r = std::for_each(vec.begin(), vec.end(), someOp);
      timer_vec.stop();
      std::cout << " std::vec sum:  " << r.sum.to_string(',') << '\n';
    }

    {
      Vector * array = NULL;
      /* assign everything. */
      array = (Vector*)malloc(sizeof(Vector) * len);
      for ( unsigned int i = 0; i < len ; ++i ) {
        array[i] = V3(0,1,(double)i);
      }

      timer_array.start();
      SomeOp<Vector> r = std::for_each(array, array+len, someOp);
      timer_array.stop();
      std::cout << "array sum:  " << r.sum.to_string(',') << '\n';
      free(array);
    }

    std::cout << "iterator timers:\n"
                 "xylose::vec:  " << timer_car << "\n"
                 " std::vec:  " << timer_vec << "\n"
                 "    array:  " << timer_array << std::endl;

    BOOST_CHECK_EQUAL( true, true );
  }

BOOST_AUTO_TEST_SUITE_END();
