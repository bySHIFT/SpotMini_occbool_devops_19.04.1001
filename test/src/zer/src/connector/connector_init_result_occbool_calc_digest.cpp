#include "my_application.hxx"

my_namespace_start

namespace application {
  int contents::connector_init_result_occbool_calc_digest() {
    auto _fn_set_item = \
        [](const std::unique_ptr<sql::ResultSet> &res
        , config::my_biz_tbl_result_occbool_calc_digest &info) {
      info.error = res->getInt(config \
        ::my_biz_tbl_result_occbool_calc_digest::COL_IDX_STATE);
      info.digest = res->getString(config \
        ::my_biz_tbl_result_occbool_calc_digest::COL_IDX_DIGEST);
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
        + config::connector_biz_proc_get_result_occbool_calc_digest \
        + "('"
        + theApp.calc_result_table_name
        + "');");
      do {
        connector_result_set res(stmt->getResultSet());
        if (!results_1st) {
        }

        while (res->next()) {
          if (!results_1st) {
            _fn_set_item(res, info_result_occbool_calc_digest);
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
