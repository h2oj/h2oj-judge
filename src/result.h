#pragma once

#include <string>

namespace hoj {
    enum class test_case_status {
        NO_STATUS = 0,
        ACCEPTED = 1,
        COMPILE_ERROR = 2,
        COMPILE_MEMORY_LIMIT_EXCEEDED = 3,
        COMPILE_TIME_LIMIT_EXCEEDED = 4,
        MEMORY_LIMIT_EXCEEDED = 5,
        OUTPUT_LIMIT_EXCEEDED = 6,
        PRESENTATION_ERROR = 7,
        RUNTIME_ERROR = 8,
        SYSTEM_ERROR = 9,
        TIME_LIMIT_EXCEEDED = 10,
        UNKNOWN_ERROR = 11,
        WRONG_ANSWER = 12
    };

    enum class judge_status {
        NO_STATUS = 0,
        ACCEPTED = 1,
        COMPILE_ERROR = 2,
        SYSTEM_ERROR = 3,
        UNACCEPTED = 4,
        UNKNOWN_ERROR = 5
    };

    struct resource_usage {
        int cpu_time;
        int real_time;
        int memory;
    };
}

