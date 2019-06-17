#include "my_application.hxx"

my_namespace_start

namespace application {

  std::string contents::contents_get_biz_options(\
    const std::string &option) const try {
    return biz_options.at(option).value;
  } catch (const std::exception &e) {
    my_exception_std(e);

    exit(1); // 查询配置信息出错时, 直接退出程序, 不提供服务
  } catch(...) {
    my_exception_unknown();

    exit(1); // 查询配置信息出错时, 直接退出程序, 不提供服务
  }
} // end namespace application

my_namespace_end
