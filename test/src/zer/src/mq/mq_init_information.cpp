#include "my_application.hxx"
#include "my_connection_params_parse.hxx"
#include "my_amqp.hxx"

#include <algorithm>

my_namespace_start

namespace application {

  int contents::mq_init_information() try {
    using zer_c = zer::config::my_biz_tbl_options;
    info_last_error.clear();

    config::my_calc_rabbit_mq info_request;
    config::my_calc_rabbit_mq info_response;
    { // 初始化队列名称
      info_request.queue_name = \
          contents_get_biz_options( \
            zer_c::code_mq_calc_request_cmd_name);
      info_response.queue_name = \
          contents_get_biz_options( \
            zer_c::code_mq_calc_response_cmd_name);
    }

    const auto connection_request_params = \
        contents_get_biz_options( \
          zer_c::code_mq_calc_request_host_conn_params);
    const auto connection_response_params = \
        contents_get_biz_options( \
          zer_c::code_mq_calc_response_host_conn_params);

    struct calc_mq_item{
      config::my_calc_rabbit_mq &calc_rabbit_mq;
      const std::string &params;
    } calc_mq_items[] = {
    { info_request, connection_request_params }
    , { info_response, connection_response_params }
  };

    std::for_each(std::begin(calc_mq_items)
      , std::end(calc_mq_items)
      , [](calc_mq_item &item) {
      do {
        const std::string connection { item.params };
        if (connection.empty()) {
          break;
        }

        zer::common::parse_connection_params(connection, &item.calc_rabbit_mq);
      } while (false);
    });

    { // 重置 OCCBOOL 用的 Rabbit MQ 配置信息
      info_request_calc_mq = info_request;
      info_response_calc_mq = info_response;

      // 算量回复用的 MQ 配置的 auto_delete 应设定为: true
      info_response_calc_mq.auto_delete = true;
    }

    do {
      std::string error_msg_mq_request { "" };
      std::string error_msg_mq_response { "" };

      const auto result_request = zer::my_amqp \
        ::amqp_exchange_declare(info_request_calc_mq, error_msg_mq_request);
      const auto result_response = zer::my_amqp \
        ::amqp_exchange_declare(info_response_calc_mq, error_msg_mq_response);

      { // 输出一些配置的信息
        std::stringstream ss;
        ss << std::endl
           << "  请求 OCCBOOL 算量MQ 信息:" << std::endl
           << "    服务器链接信息: "
           << connection_request_params
           << std::endl << "    队列名称: "
           << info_request.queue_name
           << std::endl << "    验证 RabbitMQ 服务器: "
           << (result_request ? "失败： " + error_msg_mq_request : "成功")
           << std::endl;
        ss << "  回复 OCCBOOL 算量结果MQ 信息:" << std::endl
           << "    服务器链接信息: "
           << connection_response_params
           << std::endl << "    队列名称: "
           << info_response.queue_name
           << std::endl << "    验证 RabbitMQ 服务器: "
           << (result_response ? "失败： " + error_msg_mq_response : "成功")
           << std::endl;
        my_cout(ss.str());
      }

      if (result_request == result_response
          && result_response == zer::config::state::ok) {
        return config::state::ok;
      }

      return config::state::error;
    } while(false);

    return config::state::error;
  } catch (...) {
    my_exception_unknown();

    return config::state::error;
  }

} // end namespace application

my_namespace_end
