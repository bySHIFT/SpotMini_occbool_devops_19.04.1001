#include "main.h"

extern int theapp_init_biz_options();
extern int theapp_init_occbool_project_bimmodels_calc();
extern int theapp_mq_send_request_calc();
extern int theapp_mq_send_response_calc(const std::chrono::system_clock::time_point &t_start);
extern int theapp_calc_analyzer(const std::chrono::system_clock::time_point &t_start);

std::mutex io_my_cout;
std::once_flag flag_init_log_file_my_cout;
std::fstream *ptr_my_log_stream_my_cout{ nullptr };

int main()
{
  { // 输出版本信息
    zer::common::my_cout_text_green();
    my_cout(zer::welcome());
    zer::common::my_cout_text_default();
  }

  do {
#ifdef _MSC_VER
    struct _t21 {
      ~_t21() {
        my_cout("\r\n>>>> 5秒后自动关闭, 请稍等...\r\n");
        std::future<void> fut_waiting = std::async(std::launch::async, []() {
          std::this_thread::sleep_for(std::chrono::seconds(5));
        });

        my_waiting_for(fut_waiting);

        fut_waiting.get();
      }
    } guard_for_waiting;
#endif

    const auto t_start = std::chrono::system_clock::now();

    // 设定连接 MySQL 的连接信息
    if (theMutableApp.connector_init_connection_information()) {
      std::stringstream ss;
      ss << ">>>> 设定连接 MySQL 的连接信息有误" << std::endl
         << "  设定格式: CI_CONNECTOR_MYSQL_CONNECTION=user:password@host:port"
         << std::endl;
    }

    if (theMutableApp.connector_try_check()) { // 0: SUCCESSFUL
      // 测试连接 MySQL数据库 不成功时， 退出程序
      break;
    }

    // 初始化业务的配置信息（比较耗时）
    if (theapp_init_biz_options()) {
      break;
    }

    // 初始化 OCCBOOL 算量用的工程构件信息（比较耗时）
    if (theapp_init_occbool_project_bimmodels_calc()) {
      break;
    }

    // 发送 OCCBOOL 算量用请求信息（比较耗时）
    if (theapp_mq_send_request_calc()) {
      break;
    }

    // 获取 OCCBOOL 算量用回复信息（比较耗时）
    if (theapp_mq_send_response_calc(t_start)) {
      break;
    }

    // 分析 OCCBOOL 算量结果（比较耗时）
    if (theapp_calc_analyzer(t_start)) {
      break;
    }

    return zer::config::state::ok;
  } while(false);

  return zer::config::state::error;
}
