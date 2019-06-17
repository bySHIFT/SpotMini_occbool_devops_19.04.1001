#include "main.h"
#include "my_amqp.hxx"
#include "my_time.hxx"

#include <boost/property_tree/json_parser.hpp>

namespace internal {
namespace { // unnamed
void fn_set_bim_state(const std::string guid
  , const int calc_return_code
  , const std::string &response_contents){
  for (auto &project : theMutableApp.occbool_projects) {
    for (auto &bim_info : project.bim_models) {
      if (bim_info.first == guid) {
        bim_info.second.state = \
          (calc_return_code ? \
          zer::application::contents::bim_model_info::STATE::ACKED \
          : zer::application::contents::bim_model_info::STATE::SUCCESS_CALC \
        );

        bim_info.second.calc_mq_conents.response_message = \
          response_contents;

        return;
      }
    }
  }
}

int fn_get_calc_response(std::list<std::string> &calc_response_messages) {
  std::string error_message;
  return zer::my_amqp::amqp_queue_get_messages(theApp.info_response_calc_mq
    , calc_response_messages
    , error_message);
}

int fn_set_calc_response(const std::list<std::string> &calc_response_messages) {
  for (const auto &contents : calc_response_messages) {
    try {
      std::stringstream ss(contents);
      boost::property_tree::ptree root;
      boost::property_tree::read_json<boost::property_tree::ptree>(ss, root);

      const auto cmd_type = root.get<std::string>("CMD", "");
      if (cmd_type != "CALCRESP") {
        continue;
      }

      const auto guid = root.get<std::string>("MODELID", "");
      const auto return_code = root.get<std::string>("RETURNCODE", "");

      const auto n_return_code = \
        static_cast<std::int32_t>(std::stoi(return_code));

      internal::fn_set_bim_state(guid, n_return_code, contents);
    } catch (...) {
    }
  }

  return zer::config::state::ok;
}

int fn_write_db_calc_result(){
  { // 初始化存储前后算量结果的表名称
    // 表名称长度: 64
    // 现在的值如: tbl_occbool_devops_result__1555550792__0004504879815436742241
    const auto now = zer::common::time::get_current_time_t();
    std::hash<std::thread::id> hash_fn;
    std::stringstream ss;
    ss << "tbl_occbool_devops_result__"
       << now << "__"
       << std::setw(22) << std::setfill('0') << std::right
       << hash_fn(std::this_thread::get_id());

    theMutableApp.calc_result_table_name = ss.str();
  }

  { // 追加测试历史记录
    std::string cmd_insert_table { "" };
    { // 初始化创建表的语句
      std::stringstream ss;
      ss << "INSERT INTO tbl_devops_history(result_calc_tbl_name)"
         << " VALUES('" << theApp.calc_result_table_name << "');";

      cmd_insert_table = ss.str();
    }
    if (theApp.connector_execute(cmd_insert_table)) {
      return zer::config::state::error;
    }
  }

  { // 创建计算结果内容表
    std::string cmd_create_table { "" };
    { // 初始化创建表的语句
      std::stringstream ss;
      ss << "CREATE TABLE IF NOT EXISTS "
         << theApp.calc_result_table_name << std::endl
         << R"(
(
idx INTEGER UNSIGNED PRIMARY KEY AUTO_INCREMENT
, bimmodel_RecordID INTEGER
, project_Name VARCHAR(64)
, project_Type VARCHAR(64)
, project_BimCodeTable VARCHAR(64)
, bimmodel_ModelDBName VARCHAR(64)
, bimmodel_VersionName VARCHAR(64)
, bimmodel_CreateTime VARCHAR(64)
, bimmodel_CalcRule VARCHAR(64)
, bim_IDinModel VARCHAR(64)
, bim_Category INTEGER
, bim_ElementID INTEGER
, bim_Name VARCHAR(128)
, bim_SumCost_EX DOUBLE DEFAULT 0.00
, bim_SelfCost_EX DOUBLE DEFAULT 0.00
, bim_Expression_EX TEXT DEFAULT NULL
, bim_SumCost DOUBLE DEFAULT 0.00
, bim_SelfCost DOUBLE DEFAULT 0.00
, bim_Expression TEXT DEFAULT NULL
, devops_STATE INTEGER
, devops_CreateTime VARCHAR(64)
);
)"
        << std::endl;

      cmd_create_table = ss.str();
    }

    if (theApp.connector_execute(cmd_create_table)) {
      return zer::config::state::error;
    }
  }

  if (theApp.connector_write_calc_result()) {
    return zer::config::state::error;
  }

  return zer::config::state::ok;
}

} // end namespace unnamed
} // end namespace internal

int theapp_calc_analyzer(const std::chrono::system_clock::time_point &t_start)
{ // 分析 OCCBOOL 算量结果（比较耗时）
  struct calc_dashboard {
    std::size_t total_calc { 0 };
    std::size_t total_request { 0 };
    std::size_t total_response { 0 };
    std::size_t total_response_calc_ok { 0 };
    zer::config::my_biz_tbl_result_occbool_calc_digest calc_digest;

    void cout(const std::chrono::system_clock::time_point &t_start) const {
      std::stringstream ss;
      ss << std::endl << zer::common::time::get_current_time() << std::endl
         << "  算量总构件数: " << total_calc << std::endl
         << "  成功发送算量请求数: " << total_request << std::endl
         << "  算量回复数: " << total_response << std::endl
         << "  算量回复数(成功): " << total_response_calc_ok << std::endl
         << "  算量回复数(失败): " << (total_response - total_response_calc_ok)
                                 << std::endl
         << "  未计算构件数: " << (total_calc - total_response_calc_ok)
                             << std::endl
         << std::endl
         << "测试状态: " << (calc_digest.error ? "失败" : "成功") << std::endl
         << "摘要信息: " << calc_digest.digest << std::endl
         << std::endl << std::endl
         << std::string(60, '=') << std::endl
         << std::string(60, '=') << std::endl
         << "测试状态摘要信息可参照: " << std::endl
         << "  MySQL: " << theApp.info_connector_mysql.host
         << ":" << theApp.info_connector_mysql.port << std::endl
         << "  CALL " << theApp.info_connector_mysql.database << "."
         << zer::config::connector_biz_proc_get_result_occbool_calc_digest
         << "('" << theApp.calc_result_table_name << "');" << std::endl
         << std::endl;

      my_cout(ss.str());
      {
        const auto t_end = std::chrono::system_clock::now();

        std::stringstream ss;
        ss << std::endl << zer::common::time::get_current_time() << std::endl
            << "总用时(单位分钟)： "
            << double(std::chrono::duration_cast<std::chrono::seconds>(t_end - t_start).count()) / std::chrono::minutes::period::num
            << std::endl
            << "测试状态: ";

        my_cout(ss.str());

        if (calc_digest.error) {
          zer::common::my_cout_text_red();
        } else {
          zer::common::my_cout_text_green();
        }

        my_cout(calc_digest.error ? "失败\r\n" : "成功\r\n");
        zer::common::my_cout_text_default();
      }
    }
  } my_calc_dashboard;

  { // 提示 开始
    std::stringstream ss;
    ss << std::endl
       << ">> 正在分析 OCCBOOL 算量结果数据, 请稍候..." << std::endl;
    my_cout(ss.str());
  }

  std::future<int> _fut_calc_analyzer = \
    std::async(std::launch::async
    , [ &t_start
        , &my_calc_dashboard]() -> int {
    do {
      std::list<std::string> calc_response_messages;
      if (internal::fn_get_calc_response(calc_response_messages)) {
        break;
      }

      if (internal::fn_set_calc_response(calc_response_messages)) {
        break;
      }

      if (theMutableApp.connector_init_occbool_project_bimmodels(false)) {
        break;
      }

      if (internal::fn_write_db_calc_result()) {
        break;
      }

      if (theMutableApp.connector_init_result_occbool_calc_digest()) {
        break;
      }

      my_calc_dashboard.total_calc = theApp.get_calc_total();
      my_calc_dashboard.total_request = theApp.get_calc_send_total();
      my_calc_dashboard.total_response = calc_response_messages.size();
      my_calc_dashboard.total_response_calc_ok = \
        theApp.get_calc_success_total();
      my_calc_dashboard.calc_digest = theApp.info_result_occbool_calc_digest;

      my_calc_dashboard.cout(t_start);

      return my_calc_dashboard.calc_digest.error;
    } while(false);

    return zer::config::state::error;
  });

  my_waiting_for(_fut_calc_analyzer);

  const int state = _fut_calc_analyzer.get();
  {
    std::stringstream ss;
    ss << std::endl
       << "<< 分析 OCCBOOL 算量结果数据完毕(STATE: "
       << state << ")"
       << std::endl << std::endl;

    my_cout(ss.str());
  }

  return state;
}
