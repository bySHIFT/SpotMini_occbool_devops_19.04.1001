#include "my_application.hxx"

my_namespace_start

namespace application {
  int contents::connector_get_version(std::string &version) const {
    info_last_error.clear();

    try {
      connector_connection con(connector_get_connection());
      connector_statement stmt(con->createStatement());
      connector_result_set res(stmt->executeQuery("SELECT VERSION();"));

      while (res->next()) {
        version = res->getString(1);
      }

      return config::state::ok;
    } catch (const sql::SQLException &e) {
      my_exception_MySQL(e);
    } catch (const std::exception &e) {
      my_exception_std(e);
    } catch (...) {
      my_exception_unknown();
    }

    return config::state::error;
  }

  int contents::connector_get_biz_version(
        config::my_biz_db_version &info) const {
    auto _fn_set_item = \
        [](const std::unique_ptr<sql::ResultSet> &res
        , config::my_biz_db_version &info) {
      info.version = \
          res->getString(config::my_biz_db_version::COL_IDX_VERSION);
      info.major = \
          res->getUInt(config::my_biz_db_version::COL_IDX_MAJOR);
      info.minor = \
          res->getUInt(config::my_biz_db_version::COL_IDX_MINOR);
      info.revision = \
          res->getUInt(config::my_biz_db_version::COL_IDX_REVISION);
    };
////////////////////////////////////////////////////////////////////////////////
    info_last_error.clear();

    try {
      connector_connection con(connector_get_connection());
      connector_statement stmt(con->createStatement());

      // Stored procedures (CALL) may return more than one result set
      // STATUS: first results
      // RESULTS: second results
      bool results_1st { true };
      stmt->execute("CALL " \
                    + config::connector_biz_proc_get_db_version \
                    + "();");
      do {
        connector_result_set res(stmt->getResultSet());
        if (!results_1st) {
        }

        while (res->next()) {
          if (!results_1st) {
            _fn_set_item(res, info);
          } else {
            if (connector_check_result_1st(res)) {
              return config::state::error;
            }
          }
        }

        results_1st = false;
      } while(stmt->getMoreResults());

      return config::state::ok;
    } catch (const sql::SQLException &e) {
      my_exception_MySQL(e);
    } catch (...) {
      my_exception_unknown();
    }

    return config::state::error;
  }
} // end namespace application

my_namespace_end
