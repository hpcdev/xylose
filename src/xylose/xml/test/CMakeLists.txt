add_definitions( -DXML_FILENAME=\"${CMAKE_CURRENT_SOURCE_DIR}/file.xml\" )
xylose_unit_test( Doc Doc.cpp )
# re-enable this test when physical library integration occurs
#xylose_unit_test( physical_parse physical_parse.cpp )
xylose_unit_test( vector_parse vector_parse.cpp )
