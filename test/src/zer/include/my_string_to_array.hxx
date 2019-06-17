#ifndef INCLUDE_MY_STRING_TO_ARRAY_HXX
#define INCLUDE_MY_STRING_TO_ARRAY_HXX
#include "my_namespace.hxx"

#include <string>
#include <vector>

my_namespace_start

namespace common { // common
namespace { // unnamed

  void string_to_array(const std::string &source
    , const char symbol
    , std::vector<std::string> &items
  ) {
    if (source.empty()) {
      return;
    }

    auto tmp_source{ source };
    do {
      const auto pos = tmp_source.find_first_of(symbol);
      if (pos != std::string::npos) {
        items.emplace_back(tmp_source.substr(0, pos));
        tmp_source = tmp_source.substr(pos + 1);
      } else {
        items.emplace_back(tmp_source);
        break;
      }
    } while (true);
  };

} // end namespace unnamed
} // end namespace common

my_namespace_end

#endif // INCLUDE_MY_STRING_TO_ARRAY_HXX
