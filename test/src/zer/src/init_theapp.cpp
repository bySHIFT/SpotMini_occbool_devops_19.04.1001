#include "my_application.hxx"

my_namespace_start

namespace config {

  using zer_opt = zer::config::my_biz_tbl_options;
  const std::string zer_opt::code_biz_version = \
      "38D4F118-5A8F-11E9-BC87-0242AC120004";
  // ##################################################
  // OCCBOOL 算量用 MQ 消息的相关情报
  // ##################################################
  // 请求用
  const std::string zer_opt::code_mq_calc_request_host_conn_params \
      = "CDF8D384-5A94-11E9-BC87-0242AC120004";
  const std::string zer_opt::code_mq_calc_request_cmd_name \
      = "04DE1CCA-5A94-11E9-BC87-0242AC120004";
  // 回复用
  const std::string zer_opt::code_mq_calc_response_host_conn_params \
      = "9123F970-5A95-11E9-BC87-0242AC120004";
  const std::string zer_opt::code_mq_calc_response_cmd_name \
      = "991C9338-5A95-11E9-BC87-0242AC120004";

  // ##################################################
  // OCCBOOL 算量用 工程情报
  //   数据来源 hgdataserver.BIMMODEL.RecordID
  // ##################################################
  const std::string zer_opt::code_occbool_project_bimmodel_record_ids \
      = "946652CB-5B4F-11E9-BC87-0242AC120004";

  // ##################################################
  // OCCBOOL 算量用 计算构件类别情报
  //   数据来源 hgdataserver.BIMMODEL.RecordID
  // ##################################################
  const std::string zer_opt::code_occbool_project_bimmodel_categories \
      = "07C829C3-5B6A-11E9-BC87-0242AC120004";

  // #########################################################################
  // 待计算 OCCBOOL 算量队列为空, 等待耗时构件算量的完成或异常的超时处理情报
  // #########################################################################
  const std::string zer_opt::code_wait_timeout_from_empty_queue \
      = "8C570A87-6021-11E9-BC87-0242AC120004";

  // #########################################################################
  // 算量结果对比用的 公差 情报
  // #########################################################################
  const std::string zer_opt::code_tolerance \
      = "E521F9DE-60EA-11E9-BC87-0242AC120004";

  // #########################################################################
  // OCCBOOL 算量用 同类型构件计算最大个数 情报
  // #########################################################################
  const std::string zer_opt::code_max_count_category \
      = "1D8AF05B-6748-11E9-BC87-0242AC120004";

} // end namespace config

namespace application {
const contents &get_my_application_contents() noexcept {
  static contents my_contents;
  return my_contents;
}

contents &get_mutable_my_application_contents() noexcept {
  auto &my_contents = get_my_application_contents();
  return const_cast<contents &>(my_contents);
}

int get_environment(const std::string &key, std::string &value) try {
  std::string value_environment{ "" };
  char *buf{ nullptr };
#ifdef _MSC_VER
  {
    std::size_t sz{ 0 };
    if (_dupenv_s(&buf
      , &sz
      , key.data())) {
      return zer::config::state::error;
    }
    if (buf) {
      value_environment = buf;
      free(buf);
    }
  }
#else
  buf = std::getenv(key.data());
  if (buf) {
    value_environment = buf;
  }
#endif

  std::stringstream ss;
  ss << "  获取环境变量设定 " << std::endl
     << "    " << key
     << "= " << (value_environment.empty() ? "未设定" : value_environment)
     << std::endl;

  zer::common::my_cout_text_green();
  my_cout(ss.str());
  zer::common::my_cout_text_default();

  if (value_environment.empty()) {
    return zer::config::state::error;
  }

  value = std::move(value_environment);
  return zer::config::state::ok;
} catch(...) {
  std::stringstream ss;
  {
    ss << std::endl
       << "  获取环境变量: " << key << " 发生异常" << std::endl;
  }

    my_cout(key);
    return zer::config::state::error;
}

}

my_namespace_end
