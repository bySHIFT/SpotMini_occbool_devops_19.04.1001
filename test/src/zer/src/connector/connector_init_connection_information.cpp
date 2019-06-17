#include "my_application.hxx"
#include "my_connection_params_parse.hxx"

my_namespace_start

namespace application {

  int contents::connector_init_connection_information() try {
      std::string connection{ "" };
      get_environment(config::connector_connection_environment, connection);

      if (connection.empty()) {
        // 未设定环境变量时, 返回 ok
        // 此时按照默认连接配置(root:root@localhost:3306)
        return config::state::ok;
      }

      return zer::common::parse_connection_params(connection
        , &info_connector_mysql);
//  } catch (const std::exception &e) {
//    // my_exception_std(e);

//    return config::state::error;
  } catch(...) {
    // my_exception_unknown();

    return config::state::error;
  }
} // end namespace application

my_namespace_end
