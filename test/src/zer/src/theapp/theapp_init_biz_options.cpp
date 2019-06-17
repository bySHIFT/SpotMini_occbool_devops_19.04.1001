#include "main.h"

int theapp_init_biz_options()
{ // 初始化数据（比较耗时）
  // 获取业务的配置信息
  auto _fn_reset_biz_options = []() {
      struct biz_bridge {
          std::string biz_guid_name{ "" };
          std::string biz_envi_name{ "" };
          biz_bridge(const std::string &guid, const std::string &key)
            : biz_guid_name(guid), biz_envi_name(key)
          {}
      } ci_bizs[] = {
        { zer::config::my_biz_tbl_options::code_mq_calc_request_host_conn_params, "CI_MQ_CALC_REQUEST_HOST_CONN_PARAMS" }
        , { zer::config::my_biz_tbl_options::code_mq_calc_request_cmd_name, "CI_MQ_CALC_REQUEST_CMD_NAME" }
        , { zer::config::my_biz_tbl_options::code_mq_calc_response_host_conn_params, "CI_MQ_CALC_RESPONSE_HOST_CONN_PARAMS" }
        , { zer::config::my_biz_tbl_options::code_mq_calc_response_cmd_name, "CI_MQ_CALC_RESPONSE_CMD_NAME" }
        , { zer::config::my_biz_tbl_options::code_occbool_project_bimmodel_record_ids, "CI_OCCBOOL_PROJECT_BIMMODEL_RECORD_IDS" }
        , { zer::config::my_biz_tbl_options::code_occbool_project_bimmodel_categories, "CI_OCCBOOL_PROJECT_BIMMODEL_CATEGORIES" }
        , { zer::config::my_biz_tbl_options::code_wait_timeout_from_empty_queue, "CI_WAIT_TIMEOUT_FROM_EMPTY_QUEUE" }
        , { zer::config::my_biz_tbl_options::code_tolerance, "CI_TOLERANCE" }
        , { zer::config::my_biz_tbl_options::code_max_count_category, "CI_MAX_COUNT_CATEGORY" }
      };

      // 注意对应的顺序号
      const std::string update_tbl_options_1st{ R"(UPDATE tbl_options SET `value`= ? WHERE `code`= ?;)" };
      for (const auto &item : ci_bizs) {
        std::string value{ "" };
        if (zer::application::get_environment(item.biz_envi_name, value)) {
          continue;
        }

        if (value.empty()) {
          continue;
        }

        // 设定的环境变量, 需要写入数据库
        // 方便测试追踪
        const auto &biz_guid_name{ item.biz_guid_name };
        theApp.connector_execute(update_tbl_options_1st
          , [&value, &biz_guid_name](zer::application::connector_prepared_statement &prep_stmt) {
          prep_stmt->setString(1, value);
          prep_stmt->setString(2, biz_guid_name);
        });
      }
  };
////////////////////////////////////////////////////////////////////////////////
  { // 提示 开始
    std::stringstream ss;
    ss << std::endl
       << ">> 正在初始化相关配置数据, 请稍候..." << std::endl;
    my_cout(ss.str());
  }

  std::future<int> _fut_init_biz_potions = \
    std::async(std::launch::async
      , [&_fn_reset_biz_options]() -> int {
        do {
          // 替换业务配置信息
          { // 优先环境变量的设定
              _fn_reset_biz_options();
          }

          // 初始化业务配置信息
          if (theMutableApp.connector_init_biz_options()) {
            break;
          }

          // 初始化 MQ 连接配置信息
          if (theMutableApp.mq_init_information()) {
            break;
          }

          return zer::config::state::ok;
        } while(false);

        return zer::config::state::error;
  });

  my_waiting_for(_fut_init_biz_potions);

  const int state = _fut_init_biz_potions.get();
  {
    std::stringstream ss;
    ss << std::endl
       << "<< 相关配置数据初始化完毕(STATE: "
       << state << ")"
       << std::endl << std::endl;

    my_cout(ss.str());
  }

  return state;
}
