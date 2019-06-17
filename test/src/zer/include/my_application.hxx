#ifndef INCLUDE_MY_APPLICATION_HXX
#define INCLUDE_MY_APPLICATION_HXX
#include "my_namespace.hxx"
#include "my_config.h"
#include "my_cout.hxx"
#include "my_message.hxx"

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>

#include <cstdint>
#include <cstdlib>
#include <functional>
#include <future>
#include <iomanip>
#include <iterator>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

my_namespace_start

namespace application {

  using connector_connection = std::unique_ptr<sql::Connection>;
  using connector_statement = std::unique_ptr<sql::Statement>;
  using connector_prepared_statement = std::unique_ptr<sql::PreparedStatement>;
  using connector_result_set = std::unique_ptr<sql::ResultSet>;

  struct contents {
    struct bim_model_info {
      enum STATE {
        TODO = 1
        , SEND = TODO + (1 << 1)
        , ACKED = SEND + (1 << 2)
        , SUCCESS_CALC = ACKED + (1 << 3)
      };
      zer::config::my_biz_tbl_occbool_bimmodel bimmodel_1st; // 算量前
      zer::config::my_biz_tbl_occbool_bimmodel bimmodel_2nd; // 算量后
      zer::config::my_mq_occbool_calc calc_mq_conents;
      STATE state { TODO };
    };

    struct occbool_calc_projects {
      zer::config::my_biz_tbl_occbool_project project;
      std::map<std::string, bim_model_info> \
          bim_models;
    };

    mutable zer::config::last_error info_last_error;

    zer::config::my_connector info_connector_mysql;

    zer::config::my_calc_rabbit_mq info_request_calc_mq;
    zer::config::my_calc_rabbit_mq info_response_calc_mq;

    sql::mysql::MySQL_Driver *ptr_connector_mysql_driver { nullptr };

    std::map<std::string, zer::config::my_biz_tbl_options> biz_options;
    std::list<occbool_calc_projects> occbool_projects;

    std::string calc_result_table_name { "" };
    zer::config::my_biz_tbl_result_occbool_calc_digest \
      info_result_occbool_calc_digest;

    int connector_init_connection_information();
    int connector_try_check();
    int connector_init_biz_options();
    int connector_init_occbool_projects();
    int connector_init_occbool_project_bimmodels(const bool init_bimmodel_1st);
    int connector_init_result_occbool_calc_digest();

    int mq_init_information();

    sql::Connection *connector_get_connection() const;
    int connector_check_result_1st(\
          const std::unique_ptr<sql::ResultSet> &res) const;
    int connector_get_version(std::string &version) const;
    int connector_get_biz_version(config::my_biz_db_version &info)const ;
    int connector_execute(const std::string &statement) const;
    int connector_execute(const std::string &prepare_statement
      , std::function<void(connector_prepared_statement &prep_stmt)> _fn_set_prepare_statement) const;
    int connector_write_calc_result() const;

    std::string contents_get_biz_options(const std::string &option) const;
    std::size_t get_calc_total() const;
    std::size_t get_calc_send_total() const;
    std::size_t get_calc_success_total() const;
  };

  const contents &get_my_application_contents() noexcept;
  contents &get_mutable_my_application_contents() noexcept;

  int get_environment(const std::string &key, std::string &value);
} // end namespace application

my_namespace_end

#include "my_application_define.h"

#endif // INCLUDE_MY_APPLICATION_HXX
