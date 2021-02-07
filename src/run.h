#pragma once

#include <filesystem>
#include <string>

#include "result.h"

namespace fs = std::filesystem;

namespace hoj {
    int compile(resource_usage &usage, const std::string &command, const fs::path &output, unsigned int time_limit, unsigned int space_limit);
    int run(resource_usage &usage, const std::string &command, const fs::path &input, const fs::path &output, unsigned int time_limit, unsigned int space_limit);
    int check(resource_usage &usage, const fs::path &checker, const fs::path &input, const fs::path &output, const fs::path &answer, unsigned int time_limit, unsigned int space_limit);
}
