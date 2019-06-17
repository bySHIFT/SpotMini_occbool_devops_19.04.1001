#ifndef INCLUDE_MY_COUT_HXX
#define INCLUDE_MY_COUT_HXX
#include "my_namespace.hxx"
#include "my_time.hxx"

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <thread>

#if defined(_MSC_VER)
#include <Windows.h>
#include <io.h>
#include <direct.h>

#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#else
#include <unistd.h>
#include <sys/stat.h>

#define ACCESS access
#define MKDIR(a) mkdir((a), 0755)
#endif

extern std::mutex io_my_cout;
extern std::once_flag flag_init_log_file_my_cout;
extern std::fstream *ptr_my_log_stream_my_cout;

my_namespace_start

namespace common { // common
namespace { // unnamed

#ifndef _MSC_VER
  __attribute__((unused))
#endif
  void cout(const std::string &msg) {
    std::lock_guard<std::mutex> guard(io_my_cout);
    std::call_once(flag_init_log_file_my_cout
      , []() {
        std::string log_file_name{ "" };
        { // 初始化输出日志文件名称
          const char pszDir[] = "log";
          { // 创建目录
            
            auto ret = ACCESS(pszDir, 0);
            if (ret != 0) { // 如果不存在,创建
              MKDIR(pszDir);
            }
          }

          const auto now = zer::common::time::get_current_time_t();
          std::hash<std::thread::id> hash_fn;
          std::stringstream ss;
          ss << R"(./)" << pszDir
             << R"(/log_occbool_devops__)"
             << now << "__"
             << std::setw(22) << std::setfill('0') << std::right
             << hash_fn(std::this_thread::get_id()) << ".log";

          log_file_name = ss.str();
        }

        static std::fstream s(log_file_name, std::ios_base::out);
        ptr_my_log_stream_my_cout = &s;
    });

    std::cout << msg << std::flush;

    if (ptr_my_log_stream_my_cout && ptr_my_log_stream_my_cout->good()) {
        ptr_my_log_stream_my_cout->write(msg.data(), msg.size());
        ptr_my_log_stream_my_cout->flush();
    }
  }

  inline
  void my_cout_text_red()
  {
#if defined(_MSC_VER)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
    std::cout << "\033[1;31m";
#endif
  }

  inline
  void my_cout_text_green()
  {
#if defined(_MSC_VER)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else
    std::cout << "\033[1;32m";
#endif
  }

  inline
  void my_cout_text_grey()
  {
#if defined(_MSC_VER)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
#else
    std::cout << "\033[0m";
#endif
  }

  inline
  void my_cout_text_default()
  {
    return my_cout_text_grey();
  }


} // end namespace unnamed
} // end namespace common

#define my_cout(msg) zer::common::cout(msg);

my_namespace_end

#endif // INCLUDE_MY_COUT_HXX
