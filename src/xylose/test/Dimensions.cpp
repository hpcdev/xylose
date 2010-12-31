
#include <xylose/Dimensions.hpp>

#define BOOST_TEST_MODULE  Dimensions

#include <boost/test/unit_test.hpp>
#include <sstream>

BOOST_AUTO_TEST_SUITE( Basic ); // {

  BOOST_AUTO_TEST_CASE( ndims ) {
    unsigned int ndims = -1u;
    {
      xylose::Dimensions<0,1,2> dims;
      ndims = dims.ndims;
      BOOST_CHECK_EQUAL( ndims, 3u );
    }
  
    {
      xylose::Dimensions<0,1> dims;
      ndims = dims.ndims;
      BOOST_CHECK_EQUAL( ndims, 2u );
    }
  
    {
      xylose::Dimensions<0> dims;
      ndims = dims.ndims;
      BOOST_CHECK_EQUAL( ndims, 1u );
    }
  }
  
  BOOST_AUTO_TEST_CASE( dir ) {
    unsigned int dir0 = -1u, dir1 = -1u, dir2 = -1u;
  
    {
      xylose::Dimensions<0,1,2> dims;
      dir0 = dims.dir0; dir1 = dims.dir1; dir2 = dims.dir2;
      BOOST_CHECK_EQUAL( dir0, 0u );
      BOOST_CHECK_EQUAL( dir1, 1u );
      BOOST_CHECK_EQUAL( dir2, 2u );
    }
  
    {
      xylose::Dimensions<0,1> dims;
      dir0 = dims.dir0; dir1 = dims.dir1;
      BOOST_CHECK_EQUAL( dir0, 0u );
      BOOST_CHECK_EQUAL( dir1, 1u );
    }
  
    {
      xylose::Dimensions<0> dims;
      dir0 = dims.dir0;
      BOOST_CHECK_EQUAL( dir0, 0u );
    }
  }

BOOST_AUTO_TEST_SUITE_END(); /* } Dimensions::Basic */



BOOST_AUTO_TEST_SUITE( reverse ); // {

  BOOST_AUTO_TEST_CASE( ndims ) {
    unsigned int ndims = -1u;
    {
      xylose::Dimensions<0,1,2>::reverse dims;
      ndims = dims.ndims;
      BOOST_CHECK_EQUAL( ndims, 3u );
    }
  
    {
      xylose::Dimensions<0,1>::reverse dims;
      ndims = dims.ndims;
      BOOST_CHECK_EQUAL( ndims, 2u );
    }
  
    {
      xylose::Dimensions<0>::reverse dims;
      ndims = dims.ndims;
      BOOST_CHECK_EQUAL( ndims, 1u );
    }
  }

  BOOST_AUTO_TEST_CASE( dir ) {
    unsigned int dir0 = -1u, dir1 = -1u, dir2 = -1u;
  
    {
      xylose::Dimensions<0,1,2>::reverse dims;
      dir0 = dims.dir0; dir1 = dims.dir1; dir2 = dims.dir2;
      BOOST_CHECK_EQUAL( dir0, 2u );
      BOOST_CHECK_EQUAL( dir1, 1u );
      BOOST_CHECK_EQUAL( dir2, 0u );
    }
  
    {
      xylose::Dimensions<0,1>::reverse dims;
      dir0 = dims.dir0; dir1 = dims.dir1;
      BOOST_CHECK_EQUAL( dir0, 1u );
      BOOST_CHECK_EQUAL( dir1, 0u );
    }
  
    {
      xylose::Dimensions<0>::reverse dims;
      dir0 = dims.dir0;
      BOOST_CHECK_EQUAL( dir0, 0u );
    }
  }

BOOST_AUTO_TEST_SUITE_END(); /* } Dimensions::reverse */



BOOST_AUTO_TEST_SUITE( ostream ); // {

  BOOST_AUTO_TEST_CASE( Dim3d ) {
    xylose::Dimensions<0,1,2> dims;
    std::ostringstream out;
    out << dims;
    BOOST_CHECK_EQUAL( out.str(), "3D-{0,1,2}" );
  }

  BOOST_AUTO_TEST_CASE( Dim2d ) {
    xylose::Dimensions<4,5> dims;
    std::ostringstream out;
    out << dims;
    BOOST_CHECK_EQUAL( out.str(), "2D-{4,5}" );
  }

  BOOST_AUTO_TEST_CASE( Dim1d ) {
    xylose::Dimensions<10> dims;
    std::ostringstream out;
    out << dims;
    BOOST_CHECK_EQUAL( out.str(), "1D-{10}" );
  }

BOOST_AUTO_TEST_SUITE_END(); /* } Dimensions::ostream */

