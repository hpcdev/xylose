#ifndef xylose_xml_physical_parse_h
#define xylose_xml_physical_parse_h

#include <xylose/xml/Doc.h>
#include <physical/calc/Driver.h>

namespace xylose {
  namespace xml {
    static inline void parse_item( runtime::physical::Quantity & out,
                                   const Context & x) {
      using runtime::physical::calc::Driver;
      out = Driver::instance().eval( x.text() );
    }
  } /* namespace xylose::xml */
} /* namespace xylose */
#endif // xylose_xml_physical_parse_h
