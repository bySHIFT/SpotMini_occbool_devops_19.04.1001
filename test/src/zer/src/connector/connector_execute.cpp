#include "my_application.hxx"

my_namespace_start

namespace application {

  int contents::connector_execute(const std::string &statement) const {
    info_last_error.clear();

    try {
      connector_connection con(connector_get_connection());
      connector_statement stmt(con->createStatement());

      // To run simple queries, you can use the `sql::Statement::execute()`,
      // `sql::Statement::executeQuery()`, and `sql::Statement::executeUpdate()`
      // methods.
      // Use the method `sql::Statement::execute()` if your query does not return
      // a result set or if your query returns more than one result set
      stmt->execute(statement);

      return config::state::ok;
    } catch (const sql::SQLException &e) {

      my_exception_MySQL(e);
    } catch (const std::exception &e) {
      my_exception_std(e);
    } catch (...) {
      my_exception_unknown();
    }

    return config::state::error;
  }

  int contents::connector_execute(const std::string &prepare_statement
    , std::function<void(connector_prepared_statement &prep_stmt)> _fn_set_prepare_statement) const {
    info_last_error.clear();

    try {
      connector_connection con(connector_get_connection());
      connector_prepared_statement prep_stmt(\
        con->prepareStatement(prepare_statement));

      _fn_set_prepare_statement(prep_stmt);
      // To run simple queries, you can use the `sql::Statement::execute()`,
      // `sql::Statement::executeQuery()`, and `sql::Statement::executeUpdate()`
      // methods.
      // Use the method `sql::Statement::execute()` if your query does not return
      // a result set or if your query returns more than one result set
      prep_stmt->executeUpdate();

      return config::state::ok;
    } catch (const sql::SQLException &e) {

      my_exception_MySQL(e);
    } catch (const std::exception &e) {
      my_exception_std(e);
    } catch (...) {
      my_exception_unknown();
    }

    return config::state::error;
  }
} // end namespace application

my_namespace_end
