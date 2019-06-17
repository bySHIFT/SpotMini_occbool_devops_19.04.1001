#ifndef INCLUDE_MY_TIME_HXX
#define INCLUDE_MY_TIME_HXX
#include "my_namespace.hxx"

#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

my_namespace_start

namespace common { // common
namespace time {

  static std::string to_time_string(const std::chrono::system_clock::time_point &tp) {
    const auto tp_t = std::chrono::system_clock::to_time_t(tp);
    struct tm *tp_lt{ nullptr };
#if _MSC_VER
    struct tm tmp_tp_lt;
    localtime_s(&tmp_tp_lt, &tp_t);
    tp_lt = &tmp_tp_lt;
#else
    tp_lt = std::localtime(&tp_t);
#endif

    std::stringstream ss;
    ss << std::put_time(tp_lt, R"(%F %T)");

    return ss.str();
  }

#ifndef _MSC_VER
  __attribute__((unused))
#endif
  static std::string get_current_time() {
    return to_time_string(std::chrono::system_clock::now());
  }

  inline
  static std::time_t get_current_time_t() {
    return std::chrono::system_clock::to_time_t(
      std::chrono::system_clock::now());
  }

} // end namespace time
} // end namespace common
my_namespace_end
#endif // INCLUDE_MY_TIME_HXX
