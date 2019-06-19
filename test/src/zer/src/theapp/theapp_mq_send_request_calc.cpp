#include "main.h"
#include "my_amqp.hxx"

int theapp_mq_send_request_calc()
{ // OCCBOOL 算量用请求信息（比较耗时）
  auto _fn_set_send_state = []
    (const std::list<zer::my_amqp::send_data> &contents) {
    for (const auto &send_data : contents) {
      if (send_data.state != zer::config::state::ok) {
        continue;
      }

      const auto &bim_guid = send_data.bundle_data;
      for (auto &project : theMutableApp.occbool_projects) {
        for (auto &bim_model : project.bim_models) {
          if (bim_model.first == bim_guid) {
            bim_model.second.state = \
              zer::application::contents::bim_model_info::STATE::SEND;
          }
        }
      }
    }
  };
////////////////////////////////////////////////////////////////////////////////
  { // 提示 开始
    std::stringstream ss;
    ss << std::endl
       << ">> 正在发送 OCCBOOL 算量用请求数据, 请稍候..." << std::endl;
    my_cout(ss.str());
  }

  std::future<int> _fut_send_calc_request = \
    std::async(std::launch::async
      , [_fn_set_send_state]() -> int {
        { // 清理 MQ 队列的数据
          std::string error_message;
          zer::my_amqp::amqp_queue_purge(theApp.info_request_calc_mq
            , error_message);
          zer::my_amqp::amqp_queue_purge(theApp.info_response_calc_mq
            , error_message);
        }
        do {
          const auto mq_request_calc = theApp.info_request_calc_mq;
          std::list<zer::my_amqp::send_data> contents;
          for (const auto &project : theApp.occbool_projects) {
            for (const auto &bim_model : project.bim_models) {
              zer::my_amqp::send_data content;
              content.bundle_data = bim_model.first;
              content.message = bim_model.second \
                .calc_mq_conents.request_message;

              { // 排序键的设定
                std::stringstream ss;
                const auto catalog = bim_model.second.bimmodel_1st.category;
                // catalog: 7 板最优先计算
                ss << "CAT=" << std::setw(10) << std::setfill((catalog == 7 ? '9': '0')) << std::right << std::to_string(catalog)
                    << "_ELEMENTID=" << std::setw(16) << std::setfill('0') << std::right << std::to_string(bim_model.second.bimmodel_1st.elementid)
                    << "_GUID=" << bim_model.first;

                content.bundle_order_key = ss.str();
              }

              contents.emplace_back(std::move(content));
            }
          }

          { // 整理发送 MQ 顺
            // 构件类别值大的优先计算 (防水|板 比较耗时, 优先计算)
              contents.sort([](const zer::my_amqp::send_data &lhs, const zer::my_amqp::send_data &rhs) {
                  return (lhs.bundle_order_key > rhs.bundle_order_key);
              });
          }

          std::string error_message;
          if (true && zer::my_amqp::amqp_sendstring(mq_request_calc
                , contents, error_message)) {
            my_cout(error_message);
            break;
          }

          { // 设定发送成功的构件
            _fn_set_send_state(contents);
          }

          { // 输出发送算量请求信息
            std::stringstream ss;
            ss << ">> 发送 OCCBOOL 算量用请求数据 成功" << std::endl
               << "    Rabbit MQ: " << theApp.info_request_calc_mq.host << std::endl
               << "    队列名称: " << theApp.info_request_calc_mq.queue_name << std::endl
               << "    算量总构件数: " << theApp.get_calc_total() << std::endl
               << "    成功发送算量请求数: " << theApp.get_calc_send_total() << std::endl
               << std::endl;

            my_cout(ss.str());
          }

          return zer::config::state::ok;
      } while(false);

      return zer::config::state::error;
  });

  my_waiting_for(_fut_send_calc_request);

  const int state = _fut_send_calc_request.get();
  {
    std::stringstream ss;
    ss << std::endl
       << "<< 发送 OCCBOOL 算量用请求数据完毕(STATE: "
       << state << ")"
       << std::endl << std::endl;

    my_cout(ss.str());
  }

  return state;
}
