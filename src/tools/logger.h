#pragma once

#include <iostream>
#include <string>

namespace mjl {
namespace sockets {
namespace tools {
    enum log_level {
        debug,
        info,
        warning,
        error,
        alert
    };

    class logger {
    public:
        logger(log_level);

        void to_ostream(
            std::ostream&,
            const std::string&,
            const unsigned int,
            const std::string&,
            log_level) const;
    private:
        log_level _log_level_threshold;
    };
}
}
}
