# utility macro to add a unit test to be shared by all subdirectories.
macro( xylose_unit_test test_name )
    set(BOOST_USE_STATIC_LIBS_TMP ${Boost_USE_STATIC_LIBS})
    set(Boost_USE_STATIC_LIBS ON)
    find_package( Boost REQUIRED COMPONENTS unit_test_framework )
    set(BOOST_USE_STATIC_LIBS ${Boost_USE_STATIC_LIBS_TMP})
    include_directories( ${Boost_INCLUDE_DIRS} )
    add_definitions( ${Boost_DEFINITIONS} )
    add_executable( xylose.${test_name}.test ${ARGN} )
    target_link_libraries( xylose.${test_name}.test 
        ${PROJECT_NAME}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )
    add_test( xylose.${test_name} xylose.${test_name}.test )
endmacro()


# add xylose directory to get the unit tests added recursively
add_subdirectory( xylose )
