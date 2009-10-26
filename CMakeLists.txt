cmake_minimum_required( VERSION 2.6 )

include( CTest )

option( UNIT_TEST_XYLOSE "Enable unit tests for the XYLOSE library"
    ${UNIT_TEST_XYLOSE} )

if ( UNIT_TEST_XYLOSE )
  if( NOT Boost_INCLUDE_DIRS )
    find_package( Boost COMPONENTS test )
  endif()
endif()

add_subdirectory( src )

