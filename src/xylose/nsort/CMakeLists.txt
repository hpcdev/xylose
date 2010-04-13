# add unit tests or sub-dirs that will add unit tests
add_subdirectory( map )

if( BUILD_TESTING )
    add_subdirectory( test )
endif()

