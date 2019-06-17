#include "my_application.hxx"
#include "my_time.hxx"

my_namespace_start

namespace application {

  int contents::connector_init_occbool_project_bimmodels( \
    const bool init_bimmodel_1st) {
    auto _fn_set_item = \
        [init_bimmodel_1st](const std::unique_ptr<sql::ResultSet> &res
        , zer::application::contents::bim_model_info &info) {
      using zer_c = zer::config::my_biz_tbl_occbool_bimmodel;

      auto &bimmodel = \
        (init_bimmodel_1st ? info.bimmodel_1st : info.bimmodel_2nd);

      bimmodel.id_model = \
          res->getString(zer_c::COL_IDX_IDINMODEL);
      bimmodel.category = \
          res->getInt(zer_c::COL_IDX_CATEGORY);
      bimmodel.elementid = \
          res->getInt(zer_c::COL_IDX_ELEMENTID);
      bimmodel.name = \
          res->getString(zer_c::COL_IDX_NAME);
      bimmodel.ex_sum_cost = \
          static_cast<double>( \
          res->getDouble(zer_c::COL_IDX_EX_SUMCOST));
      bimmodel.ex_self_cost = \
          static_cast<double>( \
          res->getDouble(zer_c::COL_IDX_EX_SELFCOST));
      bimmodel.ex_expression = \
          res->getString(zer_c::COL_IDX_EX_EXPRESSION);
    };
    auto _fn_reset_item = []( \
      const zer::config::my_biz_tbl_occbool_bimmodel &bimmodel_2nd
      , std::map<std::string, \
          zer::application::contents::bim_model_info> &bim_models) {
      for (auto &bim : bim_models) {
        if (bim.first == bimmodel_2nd.id_model) {
          bim.second.bimmodel_2nd = bimmodel_2nd;
          break;
        }
      }
    };
////////////////////////////////////////////////////////////////////////////////
    info_last_error.clear();

    try {
      connector_connection con(connector_get_connection());
      connector_statement stmt(con->createStatement());

      for(auto &item_project : occbool_projects) {
        const auto &item = item_project.project;

        std::stringstream ss_sql_execute;
        { // 初始化 执行存储过程的语句
          ss_sql_execute << "CALL "
            << config::connector_biz_proc_get_occbool_bimmodels_calc
            << "('"
            << item.bimmodel_ModelDBName
            << "', '"
            << contents_get_biz_options(zer::config::my_biz_tbl_options \
              ::code_occbool_project_bimmodel_categories)
            << (init_bimmodel_1st ? "', 0" : "', 1")
            << ");";
        }

        // Stored procedures (CALL) may return more than one result set
        // STATUS: first results
        // RESULTS: second results
        bool results_1st { true };
        stmt->execute(ss_sql_execute.str());
        do {
          connector_result_set res(stmt->getResultSet());
          if (init_bimmodel_1st && !results_1st) {
            item_project.bim_models.clear();
          }

          while (res->next()) {
            if (!results_1st) {
              zer::application::contents::bim_model_info info;
              _fn_set_item(res, info);
              if (init_bimmodel_1st) {
                item_project.bim_models.emplace( \
                  std::make_pair(info.bimmodel_1st.id_model, std::move(info)));
              } else {
                _fn_reset_item(info.bimmodel_2nd, item_project.bim_models);
              }
            } else {
              if (connector_check_result_1st(res)) {
                return config::state::error;
              }
            }
          }

          results_1st = false;
        } while(stmt->getMoreResults());
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
