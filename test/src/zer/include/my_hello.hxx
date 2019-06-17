#ifndef INCLUDE_MY_HELLO_HXX
#define INCLUDE_MY_HELLO_HXX
#include "my_namespace.hxx"
#include "my_config.h"
#include "my_time.hxx"

#include <string>
#include <sstream>

my_namespace_start

namespace { // unnamed
#ifndef _MSC_VER
  __attribute__((unused))
#endif
  static std::string welcome() {
    const std::string welcome("        欢迎, ");
    const std::string separator(40, '=');
    std::stringstream ss;
    ss << ""
       << separator << std::endl
       << separator << std::endl
       << welcome
       << std::endl << "    让我们来探索一些新功能。" << std::endl
       << separator << std::endl
       << separator << std::endl
       << ">>>> " << zer::common::time::get_current_time() << std::endl
       << "  VER: " << config::product_version_major
       << "." << std::setw(2) << std::setfill('0')
       << config::product_version_minor
       << std::endl << std::endl;

    return ss.str();
  }

} // end namespace unnamed

my_namespace_end
#endif // INCLUDE_MY_HELLO_HXX
