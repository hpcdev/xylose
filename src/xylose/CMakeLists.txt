
# add sub-directories to get the unit tests added recursively
add_subdirectory( nsort )
add_subdirectory( random )

find_package( LibXml2 )
if( LIBXML2_FOUND )
    add_subdirectory( xml )
endif()

if( BUILD_TESTING )
    add_subdirectory( test )
endif()

