#ifndef INCLUDE_MY_APPLICATION_DEFINE_H
#define INCLUDE_MY_APPLICATION_DEFINE_H

#define unknown_error(msg) zer::config::last_error(9999, msg);
#define theApp zer::application::get_my_application_contents()
#define theMutableApp zer::application::get_mutable_my_application_contents()

#define my_exception_unknown()                                      \
{                                                                   \
  std::stringstream ss;                                             \
  ss << ">>>> ERR: Exception in " << __FILE__                       \
     << "(" << __func__ << ") on line " << __LINE__ << std::endl;   \
                                                                    \
  theMutableApp.info_last_error = unknown_error(ss.str());          \
  zer::common::my_cout_text_red();                                  \
  my_cout(theApp.info_last_error.error_message);                    \
  zer::common::my_cout_text_default();                              \
}

#define my_exception_std(e)                                         \
{                                                                   \
  std::stringstream ss;                                             \
  ss << ">>>> ERR: Exception in " << __FILE__                       \
     << "(" << __func__ << ") on line " << __LINE__ << std::endl;   \
                                                                    \
  ss << ">>>> ERR: " << e.what() << std::endl;                      \
                                                                    \
  theMutableApp.info_last_error = unknown_error(ss.str());          \
  zer::common::my_cout_text_red();                                  \
  my_cout(theApp.info_last_error.error_message);                    \
  zer::common::my_cout_text_default();                              \
}

// The JDBC API throws three different exceptions:
// - sql::MethodNotImplementedException (derived from sql::SQLException)
// - sql::InvalidArgumentException (derived from sql::SQLException)
// - sql::SQLException (derived from std::runtime_error)
#define my_exception_MySQL(e)                                       \
{                                                                   \
  std::stringstream ss;                                             \
  ss << ">>>> ERR: SQLException in " << __FILE__                    \
     << "(" << __func__ << ") on line " << __LINE__ << std::endl;   \
                                                                    \
  ss << ">>>> ERR: " << e.what()                                    \
     << " (MySQL error code: " << e.getErrorCode()                  \
     << ", SQLState: " << e.getSQLState() << " )" << std::endl;     \
                                                                    \
  theMutableApp.info_last_error = config::last_error{               \
      e.getErrorCode(), ss.str() };                                 \
  zer::common::my_cout_text_red();                                  \
  my_cout(theApp.info_last_error.error_message);                    \
  zer::common::my_cout_text_default();                              \
}

#define my_waiting_for(fut) {                                       \
  while(fut.wait_for(std::chrono::milliseconds(980))                \
    == std::future_status::timeout) {                               \
    zer::common::my_cout_text_green();                              \
    my_cout("... ");                                                \
    zer::common::my_cout_text_default();                            \
  }                                                                 \
}

#endif // INCLUDE_MY_APPLICATION_DEFINE_H
