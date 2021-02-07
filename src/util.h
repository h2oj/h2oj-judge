#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace hoj {
    std::string format(const std::string &src, const std::map<std::string, std::string> &data);
    char transfer_char(const char &key);
    std::vector<std::string> parse_command(const std::string &command);
}
