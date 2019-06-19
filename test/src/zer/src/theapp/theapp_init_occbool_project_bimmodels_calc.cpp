#include "main.h"

int theapp_init_occbool_project_bimmodels_calc()
{ // 初始化 OCCBOOL 算量用的工程信息
  auto _fn_cout_occbool_projects_info = []() {
    const auto cnt = theApp.occbool_projects.size();
    const auto categories = theApp.contents_get_biz_options( \
          zer::config::my_biz_tbl_options \
          ::code_occbool_project_bimmodel_categories);

    int idx { 0 };
    for(const auto &item_project : theApp.occbool_projects) {
      const auto item = item_project.project;
      std::stringstream ss;
      ss << "自动测试算量工程构件信息"
         << "[" << (++idx) << "/" << cnt << "]" << std::endl
         << "  记录ID: " << item.bimmodel_RecordID << std::endl
         << "  工程名称: " << item.project_Name << std::endl
         << "  工程类型: " << item.project_Type << std::endl
         << "  工程用BIMCODE表: " << item.project_BimCodeTable << std::endl
         << "  构件库名称: " << item.bimmodel_ModelDBName << std::endl
         << "  版本: " << item.bimmodel_VersionName << std::endl
         << "  创建日期: " << item.bimmodel_CreateTime << std::endl
         << "  计算构类别: " << categories << std::endl
         << "  计算构件量: " << item_project.bim_models.size() << std::endl
         << std::endl;

      my_cout(ss.str());
    }
  };

  auto _fn_init_calc_request_mq = [](){
    for (auto &project : theMutableApp.occbool_projects) {
      const auto bimmodel_RecordID = project.project.bimmodel_RecordID;
      for (auto &bim_info : project.bim_models) {
        static const char fmt_mq_usage[] = R"({
  "MODELID": "%s",
  "CMD": "CALC",
  "VERSION": "%s",
  "ELECATEGORY": %d,
  "IPADDRESS": "%s",
  "PORT": "%d",
  "RESPONSEQUEUENAME": "%s",
  "MODELRECORDID": %d,
  "TIMESTAMP": "%s"
})";
        auto &bimmodel =bim_info.second.bimmodel_1st;
        const auto timestamp = zer::common::time::get_current_time();
        const auto sz = std::snprintf(nullptr
          , 0
          , fmt_mq_usage
          , bimmodel.id_model.data()
          , zer::config::product_version.data()
          , bimmodel.category
          , theApp.info_response_calc_mq.host.data()
          , theApp.info_response_calc_mq.port
          , theApp.info_response_calc_mq.queue_name.data()
          , bimmodel_RecordID
          , timestamp.data()
        );
        std::vector<char> buf( \
              static_cast<std::uint32_t>(sz + 1)); // 注意为空终止符 +1
        std::snprintf(&buf[0]
          , buf.size()
          , fmt_mq_usage
          , bimmodel.id_model.data()
          , zer::config::product_version.data()
          , bimmodel.category
          , theApp.info_response_calc_mq.host.data()
          , theApp.info_response_calc_mq.port
          , theApp.info_response_calc_mq.queue_name.data()
          , bimmodel_RecordID
          , timestamp.data()
        );

        bim_info.second.calc_mq_conents.request_message.assign(&buf[0]);
      }
    }
  };
////////////////////////////////////////////////////////////////////////////////
  { // 提示 开始
    std::stringstream ss;
    ss << std::endl
       << ">> 正在初始化 OCCBOOL 算量用的工程构件信息数据, 请稍候..." << std::endl;
    my_cout(ss.str());
  }

  // 获取业务的配置信息
  std::future<int> _fut_init_calc_informations = \
      std::async(std::launch::async
    , [_fn_cout_occbool_projects_info
       , _fn_init_calc_request_mq
      ]() -> int {
      do {
        if (theMutableApp.connector_init_occbool_projects()) {
          break;
        }

        if (theApp.occbool_projects.empty()) {
          std::stringstream ss;
          {
            ss << "  未获取到当前配置的 OCCBOOL 算量用 工程情报" << std::endl
               << "    设定的 OCCBOOL 算量用 工程情报: "
               << theApp.contents_get_biz_options(zer::config \
                  ::my_biz_tbl_options::code_occbool_project_bimmodel_record_ids)
               << std::endl;
          }

          my_cout(ss.str());
          break;
        }

        if (theMutableApp.connector_init_occbool_project_bimmodels(true)) {
          std::stringstream ss;
          {
            ss << "  未获取到当前配置的 OCCBOOL 算量用 构件情报" << std::endl
               << "    设定的 OCCBOOL 算量用 工程情报: "
               << theApp.contents_get_biz_options(zer::config \
                 ::my_biz_tbl_options::code_occbool_project_bimmodel_record_ids)
               << "    设定的 OCCBOOL 算量用 构件类别情报: "
               << theApp.contents_get_biz_options(zer::config \
                  ::my_biz_tbl_options::code_occbool_project_bimmodel_categories)
               << std::endl;
          }

          my_cout(ss.str());
          break;
        }

        { // 初始化 OCCBOOL 算量请求MQ命令
          _fn_init_calc_request_mq();
        }

        { // 输出即将要处理的工程信息
          _fn_cout_occbool_projects_info();
        }

        return zer::config::state::ok;
      } while(false);

      return zer::config::state::error;
  });

  my_waiting_for(_fut_init_calc_informations);

  const int state = _fut_init_calc_informations.get();
  {
    std::stringstream ss;
    ss << std::endl
       << "<< OCCBOOL 算量用的工程构件信息数据初始化完毕(STATE: "
       << state << ")"
       << std::endl << std::endl;
    my_cout(ss.str());
  }

  return state;
}
