#include "run.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <chrono>

#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/resource.h>

namespace hoj {

result compile(const config& config) {
    result result;
    result.cpu_time = 0;
    result.memory = 0;
    result.exit_code = 0;

    auto clock_start = std::chrono::steady_clock::now();
    int pid = fork();
    if (pid < 0) {
        exit(static_cast<int>(judge_status::FORK_FAILED));
    }
    else if (pid == 0) {
        const std::string cpp_compiler = config.judger_config->get("cpp_compiler");
        const std::string cpp_flag = config.judger_config->get("cpp_flag");
        const std::string code_file = config.test_config->get("code_file");
        const std::string exe_file = config.test_config->get("exe_file");
        if (cpp_flag.size() > 0) {
            execlp(cpp_compiler.c_str(), cpp_compiler.c_str(), code_file.c_str(), "-o", exe_file.c_str(), cpp_flag.c_str(), NULL);
        }
        else {
            execlp(cpp_compiler.c_str(), cpp_compiler.c_str(), code_file.c_str(), "-o", exe_file.c_str(), NULL);
        }
        exit(0);
    }
    else {
        int status;
        rusage res_usage;
        if (wait4(pid, &status, WSTOPPED, &res_usage) == -1) {
            exit(-2); // TODO: require error code
        }
        
        auto clock_end = std::chrono::steady_clock::now();

        result.cpu_time = res_usage.ru_utime.tv_sec * 1000 + res_usage.ru_utime.tv_usec / 1000;
        result.real_time = std::chrono::duration_cast<std::chrono::milliseconds>(clock_end - clock_start).count();
        result.memory = res_usage.ru_maxrss;
        result.result = judge_result::ACCEPTED;
    }

    return result;
}

result run(const config& config) {
    result result;
    result.cpu_time = 0;
    result.memory = 0;
    result.exit_code = 0;

    auto clock_start = std::chrono::steady_clock::now();

    int pid = fork();
    if (pid < 0) {
        exit(static_cast<int>(judge_status::FORK_FAILED));
    }
    else if (pid == 0) {
        // TODO: setrlimit
        int input = open(config.test_config->get("input_file").c_str(), O_RDONLY);
        dup2(input, STDIN_FILENO);
        int output = open(config.test_config->get("output_file").c_str(), O_RDWR | O_CREAT | O_TRUNC, 0777);
        dup2(output, STDOUT_FILENO);
        
        execl(config.test_config->get("exe_file").c_str(), config.test_config->get("exe_file").c_str(), nullptr);

        close(input);
        close(output);
        exit(0);
    }
    else {
        int status;
        rusage res_usage;
        if (wait4(pid, &status, WSTOPPED, &res_usage) == -1) {
            exit(-1); // TODO: error code
        }

        auto clock_end = std::chrono::steady_clock::now();

        if (status == 0) {
            result.cpu_time = res_usage.ru_utime.tv_sec * 1000 + res_usage.ru_utime.tv_usec / 1000;
            result.real_time = std::chrono::duration_cast<std::chrono::milliseconds>(clock_end - clock_start).count();
            result.memory = res_usage.ru_maxrss;
            result.result = judge_result::ACCEPTED;

            std::ifstream judge_output(config.test_config->get("output_file").c_str());
            std::ifstream judge_answer(config.test_config->get("answer_file").c_str());
            
            if (!judge_output.is_open() || !judge_answer.is_open()) {
                result.result = judge_result::SYSTEM_ERROR;
            }

            std::string out, ans;
            while (!judge_answer.eof() && !judge_output.eof()) {
                getline(judge_output, out);
                getline(judge_answer, ans);

                std::string::iterator iter_out = --out.end();
                std::string::iterator iter_ans = --ans.end();
                while (*iter_out == ' ' || *iter_out == '\n' || *iter_out == '\r') --iter_out;
                while (*iter_ans == ' ' || *iter_ans == '\n' || *iter_ans == '\r') --iter_ans;
                out = out.substr(0, distance(out.begin(), iter_out) + 1);
                ans = ans.substr(0, distance(ans.begin(), iter_ans) + 1);

                if (ans != out) {
                    result.result = judge_result::WRONG_ANSWER;
                    break;
                }
            }

            judge_output.close();
            judge_answer.close();
        }
        else {
            result.result = judge_result::UNKNOWN_ERROR;
            result.exit_code = errno;
        }
    }

    return result;
}

}
