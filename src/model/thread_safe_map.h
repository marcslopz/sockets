#pragma once

#include <map>
#include <mutex>

#include "connection_data.h"

namespace mjl {
namespace sockets {
namespace model {
    /// template<class Key,class Value> FIXME
    class thread_safe_map {
    public:
        /// bool emplace(std::pair<Key,Value>&&); FIXME
        bool emplace(std::pair<int,connection_data>&&);
    private:
        ///std::map<Key,Value> _internal_map; FIXME
        std::map<int,connection_data> _internal_map;
        std::mutex _mutex;
    };
} /// namespace model
} /// namespace sockets
} /// namespace mjl
