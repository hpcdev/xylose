cmake_minimum_required( VERSION 2.6 )

include( CTest )

option( UNIT_TEST_XYLOSE "Enable unit tests for the XYLOSE library"
    ${UNIT_TEST_XYLOSE} )

add_subdirectory( src )
