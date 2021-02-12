#include "run.h"

#include <cctype>
#include <chrono>
#include <filesystem>
#include <map>
#include <string>
#include <thread>
#include <vector>

#include <fcntl.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "result.h"
#include "util.h"

namespace fs = std::filesystem;

int hoj::compile(hoj::resource_usage &usage, const std::string &command, const fs::path &output, unsigned int time_limit, unsigned int space_limit) {
    auto split_result = hoj::parse_command(command);
    char **argv = new char *[split_result.size() + 1];
    for (int i = 0; i < split_result.size(); ++i) {
        argv[i] = const_cast<char *>(split_result[i].c_str());
    }
    argv[split_result.size()] = nullptr;
    
    auto clock_start = std::chrono::steady_clock::now();

    int pid = fork();
    int status = 0;
    if (pid < 0) {
        return -1;
    }
    else if (pid == 0) {
        rlimit time_rlimit, space_rlimit;
        time_rlimit.rlim_cur = time_rlimit.rlim_max = time_limit + 200;
        space_rlimit.rlim_cur = space_rlimit.rlim_max = space_limit + 200;
        setrlimit(RLIMIT_CPU, &time_rlimit);
        setrlimit(RLIMIT_AS, &space_rlimit);

        int output_file = open(output.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dup2(output_file, STDOUT_FILENO);
        dup2(output_file, STDERR_FILENO);
        close(output_file);

        execvp(argv[0], argv); // require error handler
    }
    else {
        std::thread monitor([pid, time_limit]() -> void {
            std::this_thread::sleep_for(std::chrono::milliseconds(time_limit + 200));
            kill(pid, SIGKILL);
        });
        monitor.detach();

        rusage res_usage;
        if (wait4(pid, &status, WSTOPPED, &res_usage) == -1) {
            return -2;
        }
        
        auto clock_end = std::chrono::steady_clock::now();

        usage.cpu_time = res_usage.ru_utime.tv_sec * 1000 + res_usage.ru_utime.tv_usec / 1000;
        usage.real_time = std::chrono::duration_cast<std::chrono::milliseconds>(clock_end - clock_start).count();
        usage.memory = res_usage.ru_maxrss;
    }

    return status;
}

int hoj::run(hoj::resource_usage &usage, const std::string &command, const fs::path &input, const fs::path &output, unsigned int time_limit, unsigned int space_limit) {
    auto split_result = hoj::parse_command(command);
    char **argv = new char *[split_result.size() + 1];
    for (int i = 0; i < split_result.size(); ++i) {
        argv[i] = const_cast<char *>(split_result[i].c_str());
    }
    argv[split_result.size()] = nullptr;

    auto clock_start = std::chrono::steady_clock::now();

    int pid = fork();
    int status = 0;
    if (pid < 0) {
        return -1;
    }
    else if (pid == 0) {
        rlimit time_rlimit, space_rlimit;
        time_rlimit.rlim_cur = time_rlimit.rlim_max = time_limit + 200;
        space_rlimit.rlim_cur = space_rlimit.rlim_max = space_limit + 200;
        setrlimit(RLIMIT_CPU, &time_rlimit);
        setrlimit(RLIMIT_AS, &space_rlimit);

        int output_file = open(output.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        int input_file = open(input.c_str(), O_RDONLY);
        dup2(output_file, STDOUT_FILENO);
        dup2(input_file, STDIN_FILENO);
        close(output_file);
        close(input_file);

        execvp(argv[0], argv); // require error handler
    }
    else {
        std::thread monitor([pid, time_limit]() -> void {
            std::this_thread::sleep_for(std::chrono::milliseconds(time_limit + 200));
            kill(pid, SIGKILL);
        });
        monitor.detach();

        rusage res_usage;
        if (wait4(pid, &status, WSTOPPED, &res_usage) == -1) {
            return -2;
        }
        
        auto clock_end = std::chrono::steady_clock::now();

        usage.cpu_time = res_usage.ru_utime.tv_sec * 1000 + res_usage.ru_utime.tv_usec / 1000;
        usage.real_time = std::chrono::duration_cast<std::chrono::milliseconds>(clock_end - clock_start).count();
        usage.memory = res_usage.ru_maxrss;
    }

    return status;
}

int hoj::check(hoj::resource_usage &usage, const fs::path &checker, const fs::path &input, const fs::path &output, const fs::path &answer, unsigned int time_limit, unsigned int space_limit) {
    char **argv = new char *[5];
    argv[0] = const_cast<char *>(checker.c_str());
    argv[1] = const_cast<char *>(input.c_str());
    argv[2] = const_cast<char *>(output.c_str());
    argv[3] = const_cast<char *>(answer.c_str());
    argv[4] = nullptr;

    auto clock_start = std::chrono::steady_clock::now();

    int pid = fork();
    int status = 0;
    if (pid < 0) {
        return -1;
    }
    else if (pid == 0) {
        rlimit time_rlimit, space_rlimit;
        time_rlimit.rlim_cur = time_rlimit.rlim_max = time_limit + 200;
        space_rlimit.rlim_cur = space_rlimit.rlim_max = space_limit + 200;
        setrlimit(RLIMIT_CPU, &time_rlimit);
        setrlimit(RLIMIT_AS, &space_rlimit);

        execvp(argv[0], argv); // require error handler
    }
    else {
        std::thread monitor([pid, &time_limit]() -> void {
            std::this_thread::sleep_for(std::chrono::milliseconds(time_limit + 200));
            kill(pid, SIGKILL);
        });
        monitor.detach();

        rusage res_usage;
        if (wait4(pid, &status, WSTOPPED, &res_usage) == -1) {
            return -2;
        }
        
        auto clock_end = std::chrono::steady_clock::now();

        usage.cpu_time = res_usage.ru_utime.tv_sec * 1000 + res_usage.ru_utime.tv_usec / 1000;
        usage.real_time = std::chrono::duration_cast<std::chrono::milliseconds>(clock_end - clock_start).count();
        usage.memory = res_usage.ru_maxrss;
    }

    return status;
}
