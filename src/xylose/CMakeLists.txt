# Base library for various useful utilities.  This project should have no
# external dependencies.  All that is required to compile the code in this
# project is a standards compliant C++ compiler.

project( Xylose )

set( VERSION xylose-0.1.1-1-g4bd0cc3 )

if( MSVC )
    set( USE_WINSOCK TRUE )
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/compat/config.hpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/compat/config.hpp
    )
endif()

add_definitions(
    -DXYLOSE_VERSION=\"${VERSION}\"
)

set( ${PROJECT_NAME}_HEADERS 
    AbstractFactory.hpp
    bits.hpp
    data_set.h
    detail/Iterator.hpp
    Factory.hpp
    Index.hpp
    logger.h
    pool_allocator.hpp
    power.h
    random/Crappy.hpp
    random/detail/RandBase.hpp
    random/Kiss.hpp
    random/MersenneTwister.hpp
    segmented_vector.hpp
    Singleton.hpp
    Stack.hpp
    strutil.h
    Swap.hpp
    SyncLock.h
    Timer.h
    TypedFactory.hpp
    upper_triangle.h
    Vector.h
    compat/unistd.hpp
    compat/stdio.hpp
    compat/compat.hpp
    compat/sys/times.hpp
    compat/sys/time.hpp
    compat/math.hpp
    compat/strings.hpp
    xml/Context.h
    xml/Doc.h
    xml/error.h
    xml/physical_parse.h
    xml/vector_parse.h
    XSTR.h
)

set( ${PROJECT_NAME}_SOURCES 
    Index.cpp
    logger.c
    power.c
    segmented_vector.cpp
    Singleton.cpp
    Stack.cpp
    Timer.cpp
    compat/sys/time.cpp
    compat/math.c
)

find_package( LibXml2 )

if( LIBXML2_FOUND )
    include_directories( ${LIBXML2_INCLUDE_DIR} )
    add_definitions( ${LIBXML2_DEFINITIONS} )
endif()

add_library( ${PROJECT_NAME} 
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

if( LIBXML2_FOUND )
    target_link_libraries( ${PROJECT_NAME} ${LIBXML2_LIBRARIES} )
endif()

# utility macro to add a unit test to be shared by all subdirectories.
macro( xylose_unit_test test_name )
    set(BOOST_USE_STATIC_LIBS_TMP ${Boost_USE_STATIC_LIBS})
    set(Boost_USE_STATIC_LIBS ON)
    find_package( Boost REQUIRED COMPONENTS unit_test_framework )
    set(BOOST_USE_STATIC_LIBS ${Boost_USE_STATIC_LIBS_TMP})
    include_directories( ${Boost_INCLUDE_DIRS} )
    add_executable( xylose.${test_name}.test ${ARGN} )
    target_link_libraries( xylose.${test_name}.test 
        ${PROJECT_NAME}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )
    add_test( xylose.${test_name} xylose.${test_name}.test )
endmacro()

# add sub-directories to get the unit tests added recursively
add_subdirectory( nsort )
add_subdirectory( random )
if( LIBXML2_FOUND )
    add_subdirectory( xml )
endif()

if( BUILD_TESTING )
    add_subdirectory( test )
endif()

