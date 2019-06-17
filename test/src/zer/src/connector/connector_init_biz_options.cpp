#include "my_application.hxx"

my_namespace_start

namespace application {

  int contents::connector_init_biz_options() {
    auto _fn_set_item = \
        [](const std::unique_ptr<sql::ResultSet> &res
        , zer::config::my_biz_tbl_options &info) {
      info.code = \
          res->getString(config::my_biz_tbl_options::COL_IDX_CODE);
      info.value = \
          res->getString(config::my_biz_tbl_options::COL_IDX_VALUE);
      info.comment = \
          res->getString(config::my_biz_tbl_options::COL_IDX_COMMENT);
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
                    + config::connector_biz_proc_get_biz_options \
                    + "();");
      do {
        connector_result_set res(stmt->getResultSet());
        if (!results_1st) {
          biz_options.clear();
        }

        while (res->next()) {
          if (!results_1st) {
            config::my_biz_tbl_options info;
            _fn_set_item(res, info);
            biz_options.emplace(std::make_pair(info.code, info));
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
    } catch (const std::exception &e) {
      my_exception_std(e);
    } catch (...) {
      my_exception_unknown();
    }

    return config::state::error;
  }

} // end namespace application

my_namespace_end
