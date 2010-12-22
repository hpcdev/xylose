# xylose utility library
# Base library for various useful utilities.  This project should have few
# external dependencies (boost, libxml).  For many components of this project,
# only a standards compliant C++ compiler is required. 

cmake_minimum_required( VERSION 2.6 )


project( xylose )

include( CTest )

################################################################################
# Obtain the xylose version
################################################################################
set( VERSION )

find_program( GIT_EXECUTABLE NAMES git )
if( GIT_EXECUTABLE )
  exec_program(
    ${GIT_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR} ARGS describe
    OUTPUT_VARIABLE VERSION
    RETURN_VALUE GIT_RETVAL )
  if( NOT ${GIT_RETVAL} EQUAL 0 )
    # Something went wrong, fall back on other methods.
    # We still didn't find a version, so it's really unknown.
    set( VERSION "xylose-unknownversion" )
  endif()
endif()

message( STATUS "xylose VERSION: ${VERSION}" )

if( MSVC )
    set( USE_WINSOCK TRUE )
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/src/xylose/compat/config.hpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/src/xylose/compat/config.hpp
    )
endif()

# include the current directory for framework style includes
set( ${PROJECT_NAME}_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}/src
)

set( ${PROJECT_NAME}_DEFINITIONS -DXYLOSE_VERSION=${VERSION} )

add_definitions( ${${PROJECT_NAME}_DEFINITIONS} )
include_directories( ${${PROJECT_NAME}_INCLUDE_DIRS} )

set( ${PROJECT_NAME}_HEADERS 
    src/xylose/AbstractFactory.hpp
    src/xylose/bits.hpp
    src/xylose/data_set.h
    src/xylose/detail/Iterator.hpp
    src/xylose/Factory.hpp
    src/xylose/Index.hpp
    src/xylose/logger.h
    src/xylose/pool_allocator.hpp
    src/xylose/power.h
    src/xylose/random/Crappy.hpp
    src/xylose/random/detail/RandBase.hpp
    src/xylose/random/Kiss.hpp
    src/xylose/random/MersenneTwister.hpp
    src/xylose/segmented_vector.hpp
    src/xylose/Singleton.hpp
    src/xylose/Stack.hpp
    src/xylose/strutil.h
    src/xylose/Swap.hpp
    src/xylose/SyncLock.h
    src/xylose/Timer.h
    src/xylose/TypedFactory.hpp
    src/xylose/upper_triangle.h
    src/xylose/Vector.h
    src/xylose/compat/unistd.hpp
    src/xylose/compat/stdio.hpp
    src/xylose/compat/compat.hpp
    src/xylose/compat/sys/times.hpp
    src/xylose/compat/sys/time.hpp
    src/xylose/compat/math.hpp
    src/xylose/compat/strings.hpp
    src/xylose/xml/Context.h
    src/xylose/xml/Doc.h
    src/xylose/xml/error.h
    src/xylose/xml/physical_parse.h
    src/xylose/xml/vector_parse.h
    src/xylose/XSTR.h
)

set( ${PROJECT_NAME}_SOURCES 
    src/xylose/Index.cpp
    src/xylose/logger.c
    src/xylose/power.c
    src/xylose/segmented_vector.cpp
    src/xylose/Singleton.cpp
    src/xylose/Stack.cpp
    src/xylose/Timer.cpp
    src/xylose/compat/sys/time.cpp
    src/xylose/compat/math.c
)

add_library( ${PROJECT_NAME} 
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

LIST(APPEND targets_to_install ${PROJECT_NAME})

find_package( appspack )
find_package( LibXml2 )
find_package( Threads )
find_package( Boost COMPONENTS program_options )
if ( appspack_FOUND AND LIBXML2_FOUND AND
     THREADS_FOUND AND CMAKE_USE_PTHREADS_INIT AND
     Boost_PROGRAM_OPTIONS_FOUND )
    add_definitions(
        ${appspack_DEFINITIONS}
        ${LIBXML2_DEFINITIONS}
        ${Boost_DEFINITIONS}
    )
    include_directories(
        ${appspack_INCLUDE_DIRS}
        ${LIBXML2_INCLUDE_DIR}
        ${Boost_INCLUDE_DIRS}
    )

    enable_language( Fortran )
    add_executable( appspack_pthread
        src/xylose/fit/appspack/appsPack.cpp
        ${appspack_DUMMY_FORTRAN_FILE}
    )
    target_link_libraries( appspack_pthread
        ${PROJECT_NAME}
        ${appspack_LIBRARIES}
        ${LIBXML2_LIBRARIES}
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
    )
    if( THREADS_HAVE_PTHREAD_ARG )
        set_source_files_properties(
            src/xylose/fit/appspack/appsPack.cpp
            COMPILE_FLAGS ${CMAKE_THREAD_LIBS_INIT}
        )
    endif()
    set_target_properties( appspack_pthread
        PROPERTIES
        LINK_FLAGS ${CMAKE_THREAD_LIBS_INIT}
    )
    LIST(APPEND targets_to_install appspack_pthread)
endif()


# Stuff to help a simple local find module for this project
MESSAGE(STATUS "Creating xylose-config.cmake")
FILE(WRITE
    ${CMAKE_BINARY_DIR}/xylose-config.cmake
    "set( ${PROJECT_NAME}_VERSION \n"
    "     ${VERSION}\n"
    ")\n"
    "set( ${PROJECT_NAME}_DEFINITIONS \n"
    "   ${${PROJECT_NAME}_DEFINITIONS}\n"
    ")\n"
    "set( ${PROJECT_NAME}_INCLUDE_DIRS\n"
    "   ${${PROJECT_NAME}_INCLUDE_DIRS}\n"
    ")\n"
    "set( ${PROJECT_NAME}_LIBRARY\n"
    "     ${PROJECT_NAME}\n"
    ")\n"
    "set( ${PROJECT_NAME}_LIBRARIES\n"
    "     ${PROJECT_NAME}\n"
    ")\n"
)

install( TARGETS ${targets_to_install}
    ARCHIVE DESTINATION lib/static
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)


# add source directory to get the unit tests recursively
add_subdirectory( src )

