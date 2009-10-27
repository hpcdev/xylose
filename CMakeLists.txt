cmake_minimum_required( VERSION 2.6 )

include( CTest )

option( UNIT_TEST_XYLOSE "Enable unit tests for the XYLOSE library"
    ${UNIT_TEST_XYLOSE} )

if ( UNIT_TEST_XYLOSE )
  if( NOT Boost_INCLUDE_DIRS )
    set( BOOST_USE_STATIC_LIBS ON )
    find_package( Boost REQUIRED COMPONENTS unit_test_framework )
  endif()
endif()

add_subdirectory( src )

