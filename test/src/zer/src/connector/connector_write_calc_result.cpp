#include "my_application.hxx"
#include "my_time.hxx"

#include <regex>

my_namespace_start

namespace application {

  // INSERT multiple rows. Example:
  // INSERT INTO tbl_name(a, b, c) VALUES(1, 2, 3), (4, 5, 6), (7, 8, 9);
  int contents::connector_write_calc_result() const {
    info_last_error.clear();

    try {
      // 注意对应的顺序号
      std::string insert_into_1st { "" };
      {
        std::stringstream ss;
        ss << "INSERT INTO "
           << theApp.calc_result_table_name
           << R"( VALUES(?
  , ?
  , ?
  , ?
  , ?
  , ?
  , ?
  , ?
  , ?
  , ?
  , ?
  , ?
  , ?
  , ?
  , ?
  , ?
  , ?
  , ?
  , ?
  , ?
  , ?)
        )";

        insert_into_1st = ss.str();
      }

      connector_connection con(connector_get_connection());
      connector_prepared_statement prep_stmt( \
        con->prepareStatement(insert_into_1st));
      for (const auto &pj : theApp.occbool_projects) {
        for (const auto &bim_info : pj.bim_models) {
          prep_stmt->setInt(1, 0);
          prep_stmt->setInt(2, pj.project.bimmodel_RecordID);
          prep_stmt->setString(3, pj.project.project_Name);
          prep_stmt->setString(4, pj.project.project_Type);
          prep_stmt->setString(5, pj.project.project_BimCodeTable);
          prep_stmt->setString(6, pj.project.bimmodel_ModelDBName);
          prep_stmt->setString(7, pj.project.bimmodel_VersionName);
          prep_stmt->setString(8, pj.project.bimmodel_CreateTime);
          prep_stmt->setString(9, pj.project.bimmodel_CalcRule);
          prep_stmt->setString(10, bim_info.second.bimmodel_1st.id_model);
          prep_stmt->setInt(11, bim_info.second.bimmodel_1st.category);
          prep_stmt->setInt(12, bim_info.second.bimmodel_1st.elementid);
          prep_stmt->setString(13, bim_info.second.bimmodel_1st.name);
          prep_stmt->setDouble(14, bim_info.second.bimmodel_1st.ex_sum_cost);
          prep_stmt->setDouble(15, bim_info.second.bimmodel_1st.ex_self_cost);
          prep_stmt->setString(16, bim_info.second.bimmodel_1st.ex_expression);
          prep_stmt->setDouble(17, bim_info.second.bimmodel_2nd.ex_sum_cost);
          prep_stmt->setDouble(18, bim_info.second.bimmodel_2nd.ex_self_cost);
          prep_stmt->setString(19, bim_info.second.bimmodel_2nd.ex_expression);
          prep_stmt->setInt(20, bim_info.second.state);
          prep_stmt->setString(21, zer::common::time::get_current_time());

          prep_stmt->executeUpdate();
        }
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

} // end namespace application

my_namespace_end
