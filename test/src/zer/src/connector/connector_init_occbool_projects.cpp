#include "my_application.hxx"

my_namespace_start

namespace application {

  int contents::connector_init_occbool_projects() {
    auto _fn_set_item = \
        [](const std::unique_ptr<sql::ResultSet> &res
        , zer::config::my_biz_tbl_occbool_project &info) {
      using zer_c = zer::config::my_biz_tbl_occbool_project;

      info.bimmodel_RecordID = \
          res->getInt(zer_c::COL_IDX_BIMMODEL_RECORDID);
      info.project_Name = \
          res->getString(zer_c::COL_IDX_PROJECT_NAME);
      info.project_Type = \
          res->getString(zer_c::COL_IDX_PROJECT_TYPE);
      info.project_BimCodeTable = \
          res->getString(zer_c::COL_IDX_PROJECT_BIMCODETABLE);
      info.bimmodel_ModelDBName = \
          res->getString(zer_c::COL_IDX_BIMMODEL_MODELDBNAME);
      info.bimmodel_VersionName = \
          res->getString(zer_c::COL_IDX_BIMMODEL_VERSIONNAME);
      info.bimmodel_CreateTime = \
          res->getString(zer_c::COL_IDX_BIMMODEL_CREATETIME);
      info.bimmodel_CalcRule = \
          res->getString(zer_c::COL_IDX_BIMMODEL_CALCRULE);
    };
    ////////////////////////////////////////////////////////////////////////////////
    info_last_error.clear();

    try {
      connector_connection con(connector_get_connection());
      connector_statement stmt(con->createStatement());

      std::stringstream ss_sql_execute;
      { // 初始化 执行存储过程的语句
        ss_sql_execute << "CALL "
          << config::connector_biz_proc_get_occbool_projects_calc
          << "('"
          << contents_get_biz_options(zer::config \
            ::my_biz_tbl_options::code_occbool_project_bimmodel_record_ids)
          << "');";
      }

      // Stored procedures (CALL) may return more than one result set
      // STATUS: first results
      // RESULTS: second results
      bool results_1st { true };
      stmt->execute(ss_sql_execute.str());
      do {
        connector_result_set res(stmt->getResultSet());
        if (!results_1st) {
          occbool_projects.clear();
        }

        while (res->next()) {
          if (!results_1st) {
            occbool_calc_projects info;
            _fn_set_item(res, info.project);
            occbool_projects.emplace_back(std::move(info));
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
