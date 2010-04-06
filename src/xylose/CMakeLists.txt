# Base library for various useful utilities.  This project should have no
# external dependencies.  All that is required to compile the code in this
# project is a standards compliant C++ compiler.

project( Xylose )

set( ${PROJECT_NAME}_HEADERS 
    AbstractFactory.hpp
    Factory.hpp
    Index.hpp
    logger.h
    pool_allocator.hpp
    power.h
    segmented_vector.hpp
    Singleton.hpp
    Stack.hpp
    SyncLock.h
    Timer.h
    TypedFactory.hpp
    Vector.h
    win_compat.hpp
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
    target_link_libraries( ${LIBXML2_LIBRARIES} )
endif()

# utility macro to add a unit test to be shared by all subdirectories.
macro( xylose_unit_test test_name )
    find_package( Boost REQUIRED COMPONENTS unit_test_framework )
    include_directories( ${Boost_INCLUDE_DIRS} )
    add_executable( xylose.${test_name}.test ${ARGN} )
    target_link_libraries( xylose.${test_name}.test 
        Xylose 
        ${Boost_LIBRARIES} )
    add_test( xylose.${test_name} xylose.${test_name}.test )
endmacro()

# add sub-directories to get the unit tests added recursively
add_subdirectory( random )
add_subdirectory( xml )

if( BUILD_TESTING )
    add_subdirectory( test )
endif()

