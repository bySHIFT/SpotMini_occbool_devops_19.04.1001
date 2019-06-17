#include "my_application.hxx"

my_namespace_start

namespace application {
  int contents::connector_try_check() {
    info_last_error.clear();

    { // 输出连接 MySQL 的链接信息
      std::stringstream ss;
      ss << ">> 请求链接数据服务..." << std::endl
         << "  链接信息: tcp://" << info_connector_mysql.user
         << "@" << info_connector_mysql.host
         << R"(:)" << info_connector_mysql.port
         << std::endl;
      my_cout(ss.str());
    }

    do {
      static std::once_flag once;
      try {
        std::call_once(once, [this]() {
          ptr_connector_mysql_driver = \
              sql::mysql::get_driver_instance();
        });
      } catch (const sql::SQLException &e) {
        my_exception_MySQL(e);
      } catch (const std::exception &e) {
        my_exception_std(e);
      } catch (...) {
        my_exception_unknown();
      }

      if (!ptr_connector_mysql_driver) {
        break;
      }

      { // 当前MySQL版本信息
        std::string my_sql_version {""};
        if (connector_get_version(my_sql_version)) {
          break;
        }

        std::stringstream ss;
        ss << "  当前 MySQL 版本信息： " << my_sql_version << std::endl;
        my_cout(ss.str());
      }

      { // 业务数据库版本信息
        config::my_biz_db_version info;
        if (connector_get_biz_version(info)) {
          break;
        }

        std::stringstream ss;
        ss << "  当前 "
           << config::product_name
           << " 使用数据库版本信息： "
           << info.version
           << std::endl;
        my_cout(ss.str());
      }

      return config::state::ok;
    } while(false);

    return config::state::error;
  }
}

my_namespace_end
