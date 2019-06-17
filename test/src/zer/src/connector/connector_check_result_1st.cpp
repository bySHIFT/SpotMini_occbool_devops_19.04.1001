#include "my_application.hxx"

my_namespace_start

namespace application {
  int contents::connector_check_result_1st(
        const std::unique_ptr<sql::ResultSet> &res) const {
    if (!res) {
      return zer::config::state::error;
    }

    const auto error = res->getInt(config::db_result_1st::COL_IDX_ERROR_NO);
    if (zer::config::state::ok != error) {
      std::stringstream ss;
      ss << "获取记录集发生错误("
         << "ErrorID: " << error
         << ", 内容: "
         << res->getString(config::db_result_1st::COL_IDX_ERROR_MESSAGE)
         << ")" << std::endl;

      info_last_error = config::last_error{ error, ss.str() };
      my_cout(info_last_error.error_message);

      return zer::config::state::error;
    }

    return zer::config::state::ok;
  }
}

my_namespace_end
