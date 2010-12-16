/** \file
 * Simple test for xml::Doc class + physical::Quantity parser.
 * */


#ifndef   XML_FILENAME
#  define XML_FILENAME  file.xml
#endif

#ifndef   XML_FILENAME
#  define XML_FILENAME  file-2.xml
#endif

#include <xylose/XSTR.h>
#include <xylose/xml/Doc.h>

#include <iostream>
#include <cstdlib>

namespace xml = xylose::xml;


int main(int argc, char **argv) {
  const char * xml_filename = getenv("XML_FILENAME");
  if ( !xml_filename )
    xml_filename = XSTR(XML_FILENAME);

  xml::Doc doc(xml_filename);
  xml::Doc doc2(XSTR(XML_FILENAME2));

  xml::Context stuff = doc.find("/Stuff");
  stuff.extend( doc2.find("/other") );
  stuff.extend( xml::Text("\n    ") );
  stuff.extend( doc2.eval("//thingies") );
  stuff.extend( xml::Text("\n") );
  stuff.extend( doc2.root_context );
  doc.save("-");

  return 0;
}


