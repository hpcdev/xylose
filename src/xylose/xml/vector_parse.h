#ifndef xylose_xml_vector_parse_h
#define xylose_xml_vector_parse_h

#include <xylose/xml/Doc.h>

#include <vector>

namespace xylose {
  namespace xml {

    template < typename T >
    static inline void parse_item( std::vector<T> & out, const Context & x ) {
      /* first make sure the vector is clean */
      out.clear();

      xml::Context::list x_list = x.eval("child::node()");// get all children
      xml::Context::list::iterator i = x_list.begin();
      xml::Context::list::iterator end = x_list.end();

      /* Now we iterate through the children trying parse_item<T> on each. */
      for ( ; i != end; ++i ) {
        const xml::Context & x1 = *i;
        try {
          T val = x1.parse< T >();
          out.push_back( val );
        } catch ( const xml::error & e ) {
          /* we ignore this error and keep trying other children. */
        }
      }

    }
  } /* namespace xylose::xml */
} /* namespace xylose */
#endif // xylose_xml_vector_parse_h
