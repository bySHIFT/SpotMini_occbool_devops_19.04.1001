# SpotMini
# From 波士顿动力
TARGET = SpotMini_occbool_devops
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

zerTarget = $${TARGET}
zerVersion = '19.04.1001'    # major.minor.patch

# zer 应用程序 文件名规则
#   $(zerTarget)_$(zerVersion)
#   eg: SpotMini_occbool_devops_19.04.1001
TARGET = $$join(TARGET,,,_$${zerVersion})

# 关闭GCC引入的ABI特性
# 因为从GCC 5.1开始, std::string 和 std::list 使用了新的C++11的ABI
DEFINES += _GLIBCXX_USE_CXX11_ABI=0

# MySQL Connector/C++ 8.0
# When compiling code which is linked with the connector library statically,
# define STATIC_CONCPP macro before including Connector/C++ public headers.
# This macro adjusts API declarations in the headers for
# usage with the static library.
win32 {
  # DEFINES += STATIC_CONCPP
  # for static build
  DEFINES += CPPCONN_LIB_BUILD
} else: {
  DEFINES += STATIC_CONCPP
}
# Location of MySQL Connector/C++ 8.0
# Platform
win32 { # Windows
MYSQL_CPPCONN_DIR = \
  $$PWD/../../__thirdparty/MySQL_Connector_C++/mysql-connector-c++-8.0.15-winx64
} else: { # Ubuntu
MYSQL_CPPCONN_DIR = \
  $$PWD/../../__thirdparty/MySQL_Connector_C++/mysql-connector-c++-8.0.15-linux-glibc2.12-x86-64bit
}


# Location of Boost
win32 {
  BOOST_DIR = \
    C:/Boost/include/boost-1_67
} else: {
  # 此平台是直接安装Boost库, 不需要指定Boost相关位置
}

# Location of RabbitMQ C AMQP client library
DEFINES += AMQP_STATIC
RABBITMQ_C_AMQP_DIR = \
  $$PWD/../../__thirdparty/RabbitMQ_C/rabbitmq-c-0.9.0

INCLUDEPATH += $${BOOST_DIR}
INCLUDEPATH += $${MYSQL_CPPCONN_DIR}/include
INCLUDEPATH += $${RABBITMQ_C_AMQP_DIR}/librabbitmq
INCLUDEPATH += $$PWD/include

# MySQL Connector/C++ 8.0
win32 {
  # MySQL Connector/C++ 8.0
  # LIBS += $${MYSQL_CPPCONN_DIR}/lib64/vs14/mysqlcppconn.lib

  # for static build
  LIBS += $${MYSQL_CPPCONN_DIR}/lib64/vs14/mysqlcppconn-static.lib
  LIBS += -lAdvAPI32

  # RabbitMQ C AMQP
  LIBS += $${RABBITMQ_C_AMQP_DIR}/build/librabbitmq/Release/librabbitmq.4.lib
  LIBS += -lws2_32
} else: {
  # Using the static library
  # 生产环境的数据库版本不能满足 MySQL Connector/C++ 8.0 要求
  # Applications that use the JDBC API can use a server from MySQL 5.5 or higher.

  LIBPATH += $${MYSQL_CPPCONN_DIR}/lib64 \     # MySQL Connector/C++ 8.0
    $${RABBITMQ_C_AMQP_DIR}/build/librabbitmq  # RabbitMQ C AMQP

  LIBS += -lrabbitmq
  LIBS += -lmysqlcppconn-static -lssl -lcrypto

  LIBS += -lpthread
  LIBS += -ldl
  LIBS += -lstdc++
}

HEADERS += \
  include/my_namespace.hxx \
  include/my_time.hxx \
  include/my_cout.hxx \
  include/my_message.hxx \
  include/my_config.h \
  include/my_string_to_array.hxx \
  include/my_connection_params_parse.hxx \
  include/my_hello.hxx \
  include/my_amqp.hxx \
  include/my_application.hxx \
  include/my_application_define.h \
  include/main.h \

SOURCES += \
  src/connector/connector_get_version.cpp \
  src/connector/connector_get_connection.cpp \
  src/connector/connector_try_check.cpp \
  src/connector/connector_check_result_1st.cpp \
  src/connector/connector_init_connection_information.cpp \
  src/connector/connector_init_biz_options.cpp \
  src/connector/connector_init_occbool_projects.cpp \
  src/connector/connector_init_occbool_project_bimmodels.cpp \
  src/connector/connector_execute.cpp \
  src/connector/connector_write_calc_result.cpp \
  src/connector/connector_init_result_occbool_calc_digest.cpp \
  src/mq/mq_init_information.cpp \
  src/contents/contents_get_biz_options.cpp \
  src/contents/contents_bim_model.cpp \
  src/init_theapp.cpp \
  src/theapp/theapp_init_occbool_project_bimmodels_calc.cpp \
  src/theapp/theapp_init_biz_options.cpp \
  src/theapp/theapp_mq_send_request_calc.cpp \
  src/theapp/theapp_mq_send_response_calc.cpp \
  src/theapp/theapp_calc_analyzer.cpp \
  main.cpp \

# 输出 Release 的目录
# release
if(!debug_and_release | build_pass):CONFIG(release, debug | release) {
  DESTDIR = $$PWD/../../bin
}
