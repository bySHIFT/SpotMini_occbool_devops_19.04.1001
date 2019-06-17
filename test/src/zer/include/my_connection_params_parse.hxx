#ifndef INCLUDE_MY_CONNECTION_PARAMS_PARSE_HXX
#define INCLUDE_MY_CONNECTION_PARAMS_PARSE_HXX
#include "my_namespace.hxx"
#include "my_config.h"
#include "my_string_to_array.hxx"

#include <string>
#include <sstream>
#include <regex>

my_namespace_start

namespace common {
namespace { // unnamed

  // 解析固定格式的内容
  // 格式: user:password@host:port
  // MySQL|RabbitMQ 的外部设定连接信息格式固定
  int parse_connection_params(const std::string &connection_params
    , zer::config::my_connection_information_base *ptr_connection_info) {
    do {
      if (!ptr_connection_info) {
        break;
      }
      if (connection_params.empty()) {
        break;
      }

      { // 解析固定格式的内容
        // 格式: root:mysqladm@192.168.1.111:3306
        const char symbol = '|';
        const std::string spliter(1, symbol);

        std::string dbStr { "" };
        { // 重新指定分隔符
          const std::regex _re(R"(:|@)");
          auto text = connection_params;
          std::stringstream ss;

          std::regex_replace(std::ostreambuf_iterator<char>(ss)
            , text.begin()
            , text.end()
            , _re
            , spliter);

          dbStr = ss.str();
        }

        std::string port { "" };
        std::vector<std::string *> items = {
          &ptr_connection_info->user
          , &ptr_connection_info->pswd
          , &ptr_connection_info->host
          , &port
        };

        { // 设定连接的相关配置信息
          const auto SIZE = items.size();
          std::vector<std::string> vecTmp;
          common::string_to_array(dbStr, symbol, vecTmp);
          if (vecTmp.size() == SIZE) {
            for (std::size_t i = 0; i < SIZE; ++i) {
              *items[i] = vecTmp[i];
            }
          }
          else if (vecTmp.size() == SIZE - 1) {
              *items[0] = vecTmp[0];
              *items[1] = "";
              *items[2] = vecTmp[1];
              *items[3] = vecTmp[2];
          }

          try {
            const auto n_port = \
              static_cast<std::uint32_t>(std::stoi(port));

            ptr_connection_info->port = n_port;
          } catch (...) {
            break;
          }

          return config::state::ok;
        }
      }

    } while(false);

    return zer::config::state::error;
  }

} // end namespace unnamed
} // end namespace common

my_namespace_end

#endif // INCLUDE_MY_CONNECTION_PARAMS_PARSE_HXX
