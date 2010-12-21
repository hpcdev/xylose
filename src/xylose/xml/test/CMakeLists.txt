add_definitions( -DXML_FILENAME=\"${CMAKE_CURRENT_SOURCE_DIR}/file.xml\" )
xylose_unit_test( Doc Doc.cpp )
xylose_unit_test( vector_parse vector_parse.cpp )

find_package( physical )
if ( physical_calc_FOUND )
    xylose_unit_test( physical_parse physical_parse.cpp )
    target_link_libraries( xylose.physical_parse.test ${physical_calc_LIBRARY} )
endif()
