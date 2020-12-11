#pragma once

#include <cstdint>
#include <string>

#include "config_loader.h"

namespace hoj {

enum class judge_status {
    SUCCESS = 0,
    FORK_FAILED = -2
};

enum class judge_result {
    ACCEPTED = 1,
    WRONG_ANSWER = 2,
    TIME_LIMIT_EXCEEDED = 3,
    MEMORY_LIMIT_EXCEEDED = 4,
    RUNTIME_ERROR = 5,
    SYSTEM_ERROR = 6,
    UNKNOWN_ERROR = -1
};

struct config {
    int max_cpu_time;
    int max_real_time;
    int max_memory;
    config_loader *judger_config;
    config_loader *test_config;
};

struct result {
    int cpu_time;
    int real_time;
    int memory;
    int exit_code;
    judge_result result;
};

result compile(const config& config);
result run(const config& config);

}
