#pragma once

#include <sys/select.h>

#include <condition_variable>
#include <map>
#include <mutex>

namespace mjl {
namespace sockets {
namespace model {

template<class T>
class thread_safe_map {
public:

  /// not mutable methods
  bool
  empty() const;

  int
  get_select_info(fd_set&) const;

  T*
  get_first_selected_socket(const fd_set&) const;

  /// mutable operations
  void
  insert(const std::pair<int,T>&);

  size_t
  erase(int);

private:
  std::map<int,T> _internal_map;
  mutable std::mutex _mutex;
  mutable std::condition_variable _condition;
};

} /* namespace model */
} /* namespace sockets */
} /* namespace mjl */

/// template implementation
namespace mjlsm = mjl::sockets::model;

template<class T>
bool
mjlsm::thread_safe_map<T>::empty() const {
  std::lock_guard<std::mutex> my_lock(_mutex);
  return _internal_map.empty();
}

template<class T>
int
mjlsm::thread_safe_map<T>::get_select_info(fd_set& my_set) const {
  std::unique_lock<std::mutex> my_lock(_mutex);
  _condition.wait(my_lock,[this]{return !_internal_map.empty();});
  int max_fd = 0;
  FD_ZERO(&my_set);
  for (auto& kv: _internal_map) {
    FD_SET(kv.first, &my_set);
    if (kv.first > max_fd)
      max_fd = kv.first;
  }
  return max_fd;
}

template<class T>
T*
mjlsm::thread_safe_map<T>::get_first_selected_socket(const fd_set& my_set) const {
  std::lock_guard<std::mutex> my_lock(_mutex);
  const int max_fd = 0;
  for (auto& kv: _internal_map) {
    if (FD_ISSET(kv.first, &my_set))
      return new T(kv.second);
  }
  return nullptr;
}

template<class T>
void
mjlsm::thread_safe_map<T>::insert(const std::pair<int,T>& p) {
  std::lock_guard<std::mutex> my_lock(_mutex);
  _internal_map.insert(p);
  _condition.notify_all();
}

template<class T>
size_t
mjlsm::thread_safe_map<T>::erase(int k) {
  std::lock_guard<std::mutex> my_lock(_mutex);
  return _internal_map.erase(k);
}
