#pragma once

#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>

#include <vector>
#include <mutex>

namespace mjl {
namespace sockets {
namespace model {

template<class T>
class thread_safe_vector {
public:

  /// not mutable methods
  bool
  empty() const;

  int
  get_select_info(fd_set&) const;

  T
  get_first_selected_socket(const fd_set&) const;

  /// mutable operations
  void
  push_back(const T&);

private:
  std::vector<T> _internal_vector;
  mutable std::mutex _mutex;
};

} /// namespace model
} /// namespace sockets
} /// namespace mjl
