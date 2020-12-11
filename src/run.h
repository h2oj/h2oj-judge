#include <cstdint>
#include <string>

namespace hoj {

enum class judge_status {
    SUCCESS = 0,
    FORK_FAILED = -1
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
    std::string program_path;
    std::string input_path;
    std::string output_path;
    std::string answer_path;
    std::string log_path;
};

struct result {
    int cpu_time;
    int real_time;
    int memory;
    int exit_code;
    judge_result result;
};

void compile(const std::string& code_path);
result run(const config& config);

}
