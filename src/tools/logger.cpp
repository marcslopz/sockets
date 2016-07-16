#include "logger.h"

namespace mjlst = mjl::sockets::tools;

mjlst::logger::logger(log_level l)
    :_log_level_threshold(l) {}

void
mjlst::logger::to_ostream(
    std::ostream& my_ostream,
    const std::string& filename,
    const unsigned int line_number,
    const std::string& msg,
    log_level level) const {
    if (level >= _log_level_threshold)
        my_ostream << filename << "::"
                   << line_number << " --> "
                   << msg
                   << std::endl;
}
