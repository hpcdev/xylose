add_definitions( -DXML_FILENAME=\\\"$(XML_FILENAME)\\\" )
xylose_unit_test( Doc Doc.cpp )
xylose_unit_test( physical_parse physical_parse.cpp )
xylose_unit_test( vector_parse vector_parse.cpp )
