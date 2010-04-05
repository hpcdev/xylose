
#ifndef xylose_xml_error_h
#define xylose_xml_error_h

#include <string>
#include <stdexcept>

namespace xylose {
  namespace xml {

    /** The error exception class for this little XML library wrapper. */
    struct error : std::runtime_error {
      typedef std::runtime_error super;
      error(const std::string & s) : super(s) {}
    };

    struct nonsingle_result_error : error {
      nonsingle_result_error(const std::string & s) : error(s) {}
    };

    struct no_results : nonsingle_result_error {
      no_results(const std::string & s) : nonsingle_result_error(s) {}
    };

    struct too_many_results : nonsingle_result_error {
      too_many_results(const std::string & s) : nonsingle_result_error(s) {}
    };

  }/* namespace xylose::xml */
}/* namespace xylose */

#endif // xylose_xml_error_h
