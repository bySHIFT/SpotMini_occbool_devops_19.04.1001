#ifndef INCLUDE_MY_AMQP_HXX
#define INCLUDE_MY_AMQP_HXX
#include "my_namespace.hxx"
#include "my_config.h"

#include <amqp.h>
#include <amqp_tcp_socket.h>

#include <string>
#include <sstream>
#include <list>

my_namespace_start

namespace my_amqp {
namespace internal {
namespace { // unnamed

static int die_on_amqp_error(const amqp_rpc_reply_t x, const char *context
  , std::string &message) {
  const char prefix_erro[] = "RabbitMQ: ";
  std::stringstream ss;
  switch (x.reply_type) {
    case AMQP_RESPONSE_NORMAL:
      return zer::config::state::ok;

    case AMQP_RESPONSE_NONE:
      ss << prefix_erro
         << context
         << "(missing RPC reply type!)"
         << std::endl;
      break;

    case AMQP_RESPONSE_LIBRARY_EXCEPTION:
      ss << prefix_erro
         << context
         << "("
         << amqp_error_string2(x.library_error)
         << ")"
         << std::endl;
      break;

    case AMQP_RESPONSE_SERVER_EXCEPTION:
      switch (x.reply.id) {
        case AMQP_CONNECTION_CLOSE_METHOD: {
          amqp_connection_close_t *m =
              (amqp_connection_close_t *)x.reply.decoded;
          ss << prefix_erro
             << context
             << "(server connection error "
             << m->reply_code
             << ", message: "
             << m->reply_text.bytes
             << ")"
             << std::endl;
          break;
        }
        case AMQP_CHANNEL_CLOSE_METHOD: {
          amqp_channel_close_t *m = (amqp_channel_close_t *)x.reply.decoded;
          ss << prefix_erro
             << context
             << "(server channel error "
             << m->reply_code
             << ", message: "
             << m->reply_text.bytes
             << ")"
             << std::endl;
          break;
        }
        default:
          ss << prefix_erro
             << context
             << "(unknown server error, method id "
             << x.reply.id
             << ")"
             << std::endl;
          break;
      }
      break;
  }

  message = ss.str();

  return zer::config::state::error;
}

amqp_connection_state_t setup_connection_and_channel(
  const zer::config::my_calc_rabbit_mq &info_mq
  , std::string &error_msg)  try {
  auto conn = amqp_new_connection();
  do {
    if (!conn) {
      error_msg = "创建 RabbitMQ Connection 错误";
      break;
    }

    auto socket = amqp_tcp_socket_new(conn);
    if (!socket) {
      error_msg = "TCP 套接字关联 RabbitMQ Connection 错误";
      break;
    }

    const auto hostname = info_mq.host.data();
    const auto port = static_cast<int>(info_mq.port);
    const auto user = info_mq.user.data();
    const auto pswd = info_mq.pswd.data();
    const auto status = amqp_socket_open(socket, hostname, port);
    if (status) {
      error_msg = "打开 RabbitMQ Connection 错误";
      break;
    }

    if (die_on_amqp_error(
      amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN,
        user, pswd), "用户验证失败", error_msg)) {
      break;
    }

    amqp_channel_open(conn, zer::config::mq_amqp_occbool_devops_channel);
    if (die_on_amqp_error(amqp_get_rpc_reply(conn)
      , "打开 Channel 错误", error_msg)) {
      break;
    }

    return conn;
  } while(false);

  return nullptr;
} catch(...) {
  return nullptr;
}

void close_channel_and_connection(
  const amqp_connection_state_t conn) try {
  std::string error_msg { "" };
  if (die_on_amqp_error(amqp_channel_close(conn
    , 1, AMQP_REPLY_SUCCESS)
    , "关闭 Channel 错误", error_msg)) {
  }

  if (die_on_amqp_error(amqp_connection_close(conn
    , AMQP_REPLY_SUCCESS)
    , "关闭 Connection 错误", error_msg)) {
  }

  if (amqp_destroy_connection(conn) < 0) {
    error_msg = "销毁 TCP 套接字错误";
  }
} catch(...) {
}

class amqp_connection_guard {
public:
  explicit amqp_connection_guard(amqp_connection_state_t &conn) : conn__(conn) {
  }
  ~amqp_connection_guard() {
    close_channel_and_connection(conn__);
  }

  amqp_connection_guard(const amqp_connection_guard &) = delete;
  amqp_connection_guard &operator= (const amqp_connection_guard &) = delete;

private:
  amqp_connection_state_t &conn__;
};

} // end namespace unnamed
} // end namespace internal

  struct send_data {
    std::string message { "" };
    int state { zer::config::state::error };
    std::string bundle_data { "" };
    std::string bundle_order_key{ "" };
  };

#ifndef _MSC_VER
  __attribute__((unused))
#endif
  static int amqp_exchange_declare(const zer::config::my_calc_rabbit_mq &info_mq
    , std::string &error_msg) try {
    do {
      auto conn = internal::setup_connection_and_channel(info_mq, error_msg);
      if (!conn) {
        break;
      }

      internal::amqp_connection_guard guard(conn);

      const auto exchange = \
          zer::config::mq_amqp_occbool_devops_exchange.data();
      const auto exchangetype = \
          zer::config::mq_amqp_occbool_devops_exchange_type.data();
      amqp_exchange_declare(conn, 1, amqp_cstring_bytes(exchange)
        , amqp_cstring_bytes(exchangetype)
        , 0 // passive
        , 0 // durable
        , 0 // auto delete
        , 0 // internal
        , amqp_empty_table);
      if (internal::die_on_amqp_error(amqp_get_rpc_reply(conn)
        , "定义 Exchange 错误", error_msg)) {
        break;
      }

      return zer::config::state::ok;
    } while(false);

    return zer::config::state::error;
  } catch(...) {
    return zer::config::state::error;
  }

#ifndef _MSC_VER
  __attribute__((unused))
#endif
  static int amqp_sendstring(const zer::config::my_calc_rabbit_mq &info_mq
    , std::list<send_data> &send_contents
    , std::string &error_msg) try {
    do {
      auto conn = internal::setup_connection_and_channel(info_mq, error_msg);
      if (!conn) {
        break;
      }

      internal::amqp_connection_guard guard(conn);

      const auto exchange = \
          zer::config::mq_amqp_occbool_devops_exchange.data();
      const auto queue = \
          info_mq.queue_name.data();
      const auto bindingkey = \
          zer::config::mq_amqp_occbool_devops_routing.data();

      { // create queue
        amqp_queue_declare(conn, zer::config::mq_amqp_occbool_devops_channel
          , amqp_cstring_bytes(queue)
          , 0 // passive
          , 1 // durable
          , 0 // exclusive
          , 0 // auto delete
          , amqp_empty_table);
        if (internal::die_on_amqp_error(amqp_get_rpc_reply(conn)
          , "声明 Queue 错误", error_msg)) {
          break;
        }
      }

      amqp_queue_bind(conn
        , zer::config::mq_amqp_occbool_devops_channel
        , amqp_cstring_bytes(queue)
        , amqp_cstring_bytes(exchange), amqp_cstring_bytes(bindingkey),
        amqp_empty_table);
      if (internal::die_on_amqp_error(amqp_get_rpc_reply(conn)
        , "绑定 Queue 错误", error_msg)) {
        break;
      }

      { // send the message
        amqp_basic_properties_t props;
        props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
        props.content_type = amqp_cstring_bytes("text/plain");
        props.delivery_mode = 2; /* persistent delivery mode */
        for (auto &content : send_contents) {
          content.state = zer::config::state::ok;
          if (amqp_basic_publish(conn
              , zer::config::mq_amqp_occbool_devops_channel
              , amqp_cstring_bytes(exchange),
              amqp_cstring_bytes(bindingkey), 0, 0,
              &props, amqp_cstring_bytes(content.message.data())) < 0) {
            content.state = zer::config::state::error;
            error_msg = "发布信息失败";
            continue;
          }
        }
      }

      return zer::config::state::ok;
    } while(false);

    return zer::config::state::error;
  } catch(...) {
    return zer::config::state::error;
  }

#ifndef _MSC_VER
  __attribute__((unused))
#endif
  static int amqp_queue_messages_count(const zer::config::my_calc_rabbit_mq &info_mq
    , std::uint32_t &message_count
    , std::string &error_msg) try {
    do {
      auto conn = internal::setup_connection_and_channel(info_mq, error_msg);
      if (!conn) {
        break;
      }

      internal::amqp_connection_guard guard(conn);

      const auto queue = \
          info_mq.queue_name.data();

      { // create queue
        amqp_queue_declare_ok_t *res = amqp_queue_declare(conn
          , zer::config::mq_amqp_occbool_devops_channel
          , amqp_cstring_bytes(queue)
          , 0 // passive
          , 1 // durable
          , 0 // exclusive
          , (info_mq.auto_delete ? 1 : 0) // auto delete
          , amqp_empty_table);
        if (internal::die_on_amqp_error(amqp_get_rpc_reply(conn)
          , "声明 Queue 错误", error_msg)) {
          break;
        }

        if (res) {
          message_count = res->message_count;
        }
      }

      return zer::config::state::ok;
    } while(false);

    return zer::config::state::error;
  } catch(...) {
    return zer::config::state::error;
  }

#ifndef _MSC_VER
  __attribute__((unused))
#endif
  static int amqp_queue_purge(const zer::config::my_calc_rabbit_mq &info_mq
    , std::string &error_msg) try {
    do {
      auto conn = internal::setup_connection_and_channel(info_mq, error_msg);
      if (!conn) {
        break;
      }

      internal::amqp_connection_guard guard(conn);

      const auto queue = \
          info_mq.queue_name.data();

      { // create queue
        amqp_queue_purge(conn
          , zer::config::mq_amqp_occbool_devops_channel
          , amqp_cstring_bytes(queue));
        if (internal::die_on_amqp_error(amqp_get_rpc_reply(conn)
          , "清除 Queue 错误", error_msg)) {
          break;
        }
      }

      return zer::config::state::ok;
    } while(false);

    return zer::config::state::error;
  } catch(...) {
    return zer::config::state::error;
  }

#ifndef _MSC_VER
  __attribute__((unused))
#endif
  static int amqp_queue_get_messages(const zer::config::my_calc_rabbit_mq &info_mq
    , std::list<std::string> &messages
    , std::string &error_msg) try {
    do {
      auto conn = internal::setup_connection_and_channel(info_mq, error_msg);
      if (!conn) {
        break;
      }

      internal::amqp_connection_guard guard(conn);

      const auto queue = \
          info_mq.queue_name.data();

      amqp_rpc_reply_t rpc_reply;

      do { // 获取所有信息
        rpc_reply = amqp_basic_get(conn
          , zer::config::mq_amqp_occbool_devops_channel
          , amqp_cstring_bytes(queue)
          , 1/*no_ack*/);

        if (rpc_reply.reply_type != AMQP_RESPONSE_NORMAL) {
          break;
        }
        if (rpc_reply.reply.id == AMQP_BASIC_GET_EMPTY_METHOD) {
          break;
        }
        if (rpc_reply.reply.id != AMQP_BASIC_GET_OK_METHOD) {
          break;
        }

        amqp_message_t message;
        rpc_reply = amqp_read_message(conn
          , zer::config::mq_amqp_occbool_devops_channel
          , &message, 0);
        if (rpc_reply.reply_type != AMQP_RESPONSE_NORMAL) {
          break;
        }

        std::string data { "" };
        data.assign((char *)message.body.bytes, message.body.len);
        messages.push_back(std::move(data));
        amqp_destroy_message(&message);
      } while(true);

      return zer::config::state::ok;
    } while(false);

    return zer::config::state::error;
  } catch(...) {
    return zer::config::state::error;
  }

} // end namespace my_amqp

my_namespace_end

#endif // INCLUDE_MY_AMQP_HXX
