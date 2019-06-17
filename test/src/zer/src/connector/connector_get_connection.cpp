#include "my_application.hxx"

my_namespace_start

namespace application {
  sql::Connection *contents::connector_get_connection() const {
    info_last_error.clear();

    const auto driver = ptr_connector_mysql_driver;
    do {
      if (!driver) {
        break;
      }

      try {
        const auto &info = info_connector_mysql;

        const std::string &user { info.user };
        const std::string &pass { info.pswd };
        std::string url { "" };
        { // initial url
          std::stringstream ss;
          ss << R"(tcp://)" << info.host
             << R"(:)" << info.port;

          url = ss.str();
        }

        auto con = driver->connect(url, user, pass);
        if (con) {
          con->setSchema(info.database);

          { // SET client character set
            // create a no prepared statement
            connector_statement stmt(con->createStatement());
#if _MSC_VER
            // Windows 平台用
            stmt->execute("SET NAMES GBK;");
#else
            stmt->execute("SET NAMES utf8;");
#endif
          }
        }

        return con;
      } catch (const sql::SQLException &e) {
        my_exception_MySQL(e);

        exit(1); // 连接错误时, 直接退出程序, 不提供服务
      } catch (const std::exception &e) {
        my_exception_std(e);

        exit(1); // 连接错误时, 直接退出程序, 不提供服务
      } catch (...) {
        my_exception_unknown();

        exit(1); // 连接错误时, 直接退出程序, 不提供服务
      }

    } while (false);

    return nullptr;
  }

} // end namespace application

my_namespace_end
