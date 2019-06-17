#ifndef INCLUDE_MY_MESSAGE_HXX
#define INCLUDE_MY_MESSAGE_HXX
#include "my_namespace.hxx"

#include <memory>

my_namespace_start

namespace messaging {
  struct message_base {
    virtual ~message_base() {}
  };

  template<typename MSG>
  struct wrapped_message : message_base {
    MSG contents;
    explicit wrapped_message(const MSG &msg) : contents(msg) {}
  };
} // end namespace messagine

my_namespace_end

#endif // INCLUDE_MY_MESSAGE_HXX
