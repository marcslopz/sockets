#include "thread_safe_map.h"

namespace mjlsm = mjl::sockets::model;

/// template<class Key, class Value> FIXME
///bool
///mjlsm::thread_safe_map<Key,Value>::emplace(std::pair<Key,Value>&& new_pair) {
bool
mjlsm::thread_safe_map::emplace(std::pair<int,connection_data>&& new_pair) {
    std::lock_guard<std::mutex> guard(_mutex);
    auto emplace_return = _internal_map.emplace(new_pair);
    return emplace_return.second;
}
