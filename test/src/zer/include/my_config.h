#ifndef INCLUDE_MY_CONFIG_H
#define INCLUDE_MY_CONFIG_H
#include "my_namespace.hxx"

#include <string>
#include <cstdint>

my_namespace_start

namespace config {
  enum state {
    ok = 0
    , error
    , unknown
  };

  const std::string product_name = "zer";
  const int product_version_major = 19;
  const int product_version_minor = 04;
  const std::string product_version = "19.04";

  // 连接 MySQL 的连接信息
  // 格式: root:mysqladm@192.168.1.111:3306
  const std::string connector_connection_environment = \
      "CI_CONNECTOR_MYSQL_CONNECTION";

  const std::string connector_biz_proc_get_db_version = \
      "proc_get_db_version";
  const std::string connector_biz_proc_get_biz_options = \
      "proc_get_biz_options";
  const std::string connector_biz_proc_get_occbool_projects_calc = \
      "proc_get_occbool_projects_calc";
  const std::string connector_biz_proc_get_occbool_bimmodels_calc = \
      "proc_get_occbool_bimmodels_calc";
  const std::string connector_biz_proc_get_result_occbool_calc_digest = \
      "proc_get_result_occbool_calc_digest";

  const std::string mq_amqp_occbool_devops_exchange = \
      "CALC_OCCBOOL_DEVOPS_EXCH";
  const std::string mq_amqp_occbool_devops_exchange_type = \
      "direct";
  const std::string mq_amqp_occbool_devops_routing = \
      "CALC_OCCBOOL_DEVOPS_ROUT";
  const int mq_amqp_occbool_devops_channel = 1;

  struct last_error {
    int error_no { 0 };
    std::string error_message { "" };

    last_error(const int no = 0, const std::string &msg = "")
      : error_no(no)
      , error_message(msg) {
    }
    void clear() noexcept {
      error_no = 0;
      error_message = "";
    }
  };

  // MySQL|RabbitMQ 的外部设定连接信息格式固定
  // 格式: user:password@host:port
  struct my_connection_information_base {
    std::string host { "" };
    std::uint32_t port { 0 };

    std::string user { "" };
    std::string pswd { "" };

    my_connection_information_base(const std::string &host_
      , const std::uint32_t port_
      , const std::string user_
      , const std::string pswd_
    ) : host(host_)
      , port(port_)
      , user(user_)
      , pswd(pswd_)
    {}

    virtual ~my_connection_information_base() {
    }
  };

  struct my_connector : my_connection_information_base {
    std::string database { "" };

    my_connector(const std::string &host_ = "mysql"
      , const std::uint32_t port_ = 3306
      , const std::string user_ = "root"
      , const std::string pswd_ = ""
      , const std::string database_ = config::product_name
      ) : my_connection_information_base(host_, port_, user_, pswd_)
      , database(database_)
    {}
  };

  struct my_calc_rabbit_mq : my_connection_information_base {
    std::string queue_name { "" };
    bool auto_delete { false };

    my_calc_rabbit_mq(const std::string &host_ = "rabbitmq"
      , const std::uint32_t port_ = 5672
      , const std::string user_ = "guest"
      , const std::string pswd_ = "guest"
      , const std::string queue_name_ = ""
      , const bool auto_delete_ = false
      ) : my_connection_information_base(host_, port_, user_, pswd_)
      , queue_name(queue_name_)
      , auto_delete(auto_delete_)
    {}
  };

  struct db_result_1st {
    enum {
      COL_IDX_ERROR_NO = 1
      , COL_IDX_ERROR_MESSAGE
    };

    std::int32_t error_no { 0 };
    std::string error_message { "" };
  };

  struct my_biz_db_version {
    enum {
      COL_IDX_VERSION = 1
      , COL_IDX_MAJOR
      , COL_IDX_MINOR
      , COL_IDX_REVISION
    };

    std::string version { "" };
    std::uint32_t major { 0 };
    std::uint32_t minor { 0 };
    std::uint32_t revision { 0 };
  };

  struct my_biz_tbl_options {
    enum {
      COL_IDX_CODE = 1
      , COL_IDX_VALUE
      , COL_IDX_COMMENT
    };

    static const std::string code_biz_version;

    // #########################################################################
    // OCCBOOL 算量用 MQ 消息的相关情报
    // #########################################################################
    // 请求用
    static const std::string code_mq_calc_request_host_conn_params;
    static const std::string code_mq_calc_request_cmd_name;
    // 回复用
    static const std::string code_mq_calc_response_host_conn_params;
    static const std::string code_mq_calc_response_cmd_name;

    // #########################################################################
    // OCCBOOL 算量用 工程情报
    //   数据来源 hgdataserver.BIMMODEL.RecordID
    // #########################################################################
    static const std::string code_occbool_project_bimmodel_record_ids;

    // #########################################################################
    // OCCBOOL 算量用 计算构件类别情报
    //   数据来源 hgdataserver.BIMMODEL.RecordID
    // #########################################################################
    static const std::string code_occbool_project_bimmodel_categories;

    // #########################################################################
    // 待计算 OCCBOOL 算量队列为空, 等待耗时构件算量的完成或异常的超时处理情报
    // #########################################################################
    static const std::string code_wait_timeout_from_empty_queue;

    // #########################################################################
    // 算量结果对比用的 公差 情报
    // #########################################################################
    static const std::string code_tolerance;

    // #########################################################################
    // OCCBOOL 算量用 同类型构件计算最大个数 情报
    // #########################################################################
    static const std::string code_max_count_category;

    std::string code { "" };
    std::string value { "" };
    std::string comment { "" };
  };

  struct my_biz_tbl_occbool_project {
    enum {
      COL_IDX_BIMMODEL_RECORDID = 1
      , COL_IDX_PROJECT_NAME
      , COL_IDX_PROJECT_TYPE
      , COL_IDX_PROJECT_BIMCODETABLE
      , COL_IDX_BIMMODEL_MODELDBNAME
      , COL_IDX_BIMMODEL_VERSIONNAME
      , COL_IDX_BIMMODEL_CREATETIME
      , COL_IDX_BIMMODEL_CALCRULE
    };

    std::int32_t bimmodel_RecordID { 0 };
    std::string project_Name { "" };
    std::string project_Type { "" };
    std::string project_BimCodeTable { "" };
    std::string bimmodel_ModelDBName { "" };
    std::string bimmodel_VersionName { "" };
    std::string bimmodel_CreateTime { "" };
    std::string bimmodel_CalcRule { "" };
  };

  struct my_biz_tbl_occbool_bimmodel {
    enum {
      COL_IDX_IDINMODEL = 1
      , COL_IDX_CATEGORY
      , COL_IDX_ELEMENTID
      , COL_IDX_NAME
      , COL_IDX_EX_SUMCOST
      , COL_IDX_EX_SELFCOST
      , COL_IDX_EX_EXPRESSION
    };

    std::string id_model { "" };
    std::int32_t category { 0 };
    std::int32_t elementid { 0 };
    std::string name { "" };
    double ex_sum_cost { 0.00 };
    double ex_self_cost { 0.00 };
    std::string ex_expression { "" };
  };

  struct my_biz_tbl_result_occbool_calc_digest {
    enum {
      COL_IDX_STATE = 1
      , COL_IDX_DIGEST
    };

    std::int32_t error { 0 };
    std::string digest { "" };
  };

  struct my_mq_occbool_calc {
    std::string request_message { "" };
    std::string response_message { "" };
  };

} // end namespace config

my_namespace_end


#endif // INCLUDE_MY_CONFIG_H
