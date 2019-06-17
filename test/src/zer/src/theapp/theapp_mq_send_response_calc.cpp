#include "main.h"
#include "my_amqp.hxx"

#include <chrono>
#include <mutex>

int theapp_mq_send_response_calc(const std::chrono::system_clock::time_point &t_start)
{ // 获取 OCCBOOL 算量用回复信息（比较耗时）

  // 取得外部超时设定
  auto _fn_get_last_wait_timeout = []() {
    auto last_wait_timeout { 30 };
    std::string str_timeout = theApp.contents_get_biz_options(zer::config \
      ::my_biz_tbl_options::code_wait_timeout_from_empty_queue);
    try {
     const auto n_timeout = \
       static_cast<std::int32_t>(std::stoi(str_timeout));
     if (n_timeout >= 1 && n_timeout <60) {
       last_wait_timeout = n_timeout;
     }
    } catch (...) {
    }

    return last_wait_timeout;
  };
////////////////////////////////////////////////////////////////////////////////
  { // 提示 开始
    std::stringstream ss;
    ss << std::endl
       << ">> 正在获取 OCCBOOL 算量用回复数据, 请稍候..." << std::endl;
    my_cout(ss.str());
  }

  std::future<int> _fut_get_calc_results = \
    std::async(std::launch::async
      , [_fn_get_last_wait_timeout, t_start]() -> int {
        std::once_flag flag_expired;
        std::time_t time_expired = std::chrono::system_clock::to_time_t(
          std::chrono::system_clock::now()
            + std::chrono::hours(12));
        std::string time_expired_string = zer::common::time::to_time_string(
          std::chrono::system_clock::from_time_t(time_expired));
        do {
        const auto send_calc_total { theApp.get_calc_send_total() };
        const auto last_wait_timeout { _fn_get_last_wait_timeout() };

        std::size_t cnt_query { 0 };
        while(true) {
          std::stringstream ss;
          std::string error_message;
          std::uint32_t message_count_ready { 0 };
          std::uint32_t message_count_acked { 0 };
          zer::my_amqp::amqp_queue_messages_count(theApp.info_request_calc_mq
            , message_count_ready, error_message);
          zer::my_amqp::amqp_queue_messages_count(theApp.info_response_calc_mq
           , message_count_acked, error_message);

          const auto t_end = std::chrono::system_clock::now();

          ss << std::endl
             << zer::common::time::get_current_time()
             << " 查询第 " << (++cnt_query) << "次" << std::endl
             << ">> 终了时间: " << time_expired_string << std::endl
             << "  算量总构件数: " << theApp.get_calc_total() << std::endl
             << "  成功发送算量请求数: " << theApp.get_calc_send_total() << std::endl
             << "  Rabbit MQ: " << theApp.info_request_calc_mq.host << std::endl
             << "  队列名称: " << theApp.info_request_calc_mq.queue_name << std::endl
             << "  待处理数: " << message_count_ready << std::endl
             << "  Rabbit MQ: " << theApp.info_response_calc_mq.host << std::endl
             << "  队列名称: " << theApp.info_response_calc_mq.queue_name << std::endl
             << "  已计算数: " << message_count_acked << std::endl
             << "  超时设定: " << last_wait_timeout << "分钟" << std::endl
             << "  耗时： " << std::setiosflags(std::ios::fixed) << std::setprecision(2)
             << double(std::chrono::duration_cast<std::chrono::seconds>(t_end - t_start).count()) / std::chrono::minutes::period::num
             << "分钟" << std::endl
             << std::endl;

          my_cout(ss.str());

          if (0 == message_count_ready) {
            std::call_once(flag_expired, [&time_expired
                , &time_expired_string
                , last_wait_timeout](){
              time_expired = std::chrono::system_clock::to_time_t(
               std::chrono::system_clock::now()
                   + std::chrono::minutes(last_wait_timeout)); // 这个超时值, 应该在外围设定

              time_expired_string = zer::common::time::to_time_string(
                std::chrono::system_clock::from_time_t(time_expired));
            });
          }

          if (send_calc_total && send_calc_total == message_count_acked) {
              my_cout("OCCBOOL 算量已经完成, 开始准备分析...");
              return zer::config::state::ok;
          }

          if (0 == message_count_ready && send_calc_total
             && (send_calc_total != message_count_acked)
          ) {
             std::stringstream ss;
             ss << R"(待计算 OCCBOOL 算量队列为空, 等待耗时构件算量的完成或异常的超时处理...)"
                << std::endl;
             my_cout(ss.str());
          }

          const auto now_c = std::chrono::system_clock \
            ::to_time_t(std::chrono::system_clock::now());
          if (now_c > time_expired) {
            my_cout("OCCBOOL 算量已经完成(异常的超时处理), 开始准备分析...");
            return zer::config::state::ok;
          }

          std::this_thread::sleep_for(std::chrono::seconds(8));
        }
      } while(false);

      return zer::config::state::error;
  });

  my_waiting_for(_fut_get_calc_results);

  const int state = _fut_get_calc_results.get();
  { // 提示 结束
    std::stringstream ss;
    ss << std::endl
       << "<< 获取 OCCBOOL 算量用回复数据完毕(STATE: "
       << state << ")"
       << std::endl << std::endl;

    my_cout(ss.str());
  }

  return state;
}
