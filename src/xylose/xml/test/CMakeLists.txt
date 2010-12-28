find_package( LibXml2 REQUIRED )
include_directories( ${LIBXML2_INCLUDE_DIR} )
add_definitions( ${LIBXML2_DEFINITIONS} )


macro( xml_xylose_unit_test test_name )
    xylose_unit_test( ${test_name} ${ARGN} )
    target_link_libraries( xylose.${test_name}.test ${LIBXML2_LIBRARIES} )
endmacro()

add_definitions( -DXML_FILENAME=\"${CMAKE_CURRENT_SOURCE_DIR}/file.xml\" )
xml_xylose_unit_test( Doc Doc.cpp )
xml_xylose_unit_test( vector_parse vector_parse.cpp )

find_package( physical COMPONENTS calc )
if ( physical_CALC_FOUND )
    add_definitions( ${physical_DEFINITIONS} )
    include_directories( ${physical_INCLUDE_DIRS} )

    xml_xylose_unit_test( physical_parse physical_parse.cpp )
    target_link_libraries( xylose.physical_parse.test ${physical_CALC_LIBRARY} )
endif()
