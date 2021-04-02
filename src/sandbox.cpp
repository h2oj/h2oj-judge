/*
    This file is part of the h2oj-judge.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/* Copyright (C) 2020-2021 Alex Cui */

#include "sandbox.h"

#include <cerrno>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <thread>

#include <fcntl.h>
#include <sched.h>
#include <signal.h>
#include <sys/mount.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "cgroup.h"

namespace fs = std::filesystem;

int child_process(void *arg) {
    hoj::sandbox_param &param = *reinterpret_cast<hoj::sandbox_param *>(arg);

    // Make root private
    mount(nullptr, "/", nullptr, MS_PRIVATE | MS_REC, nullptr);

    // Mount file
    for (const auto &info : param.mount) {
        auto target_path = param.chroot_path / info.target.relative_path();

        if (!fs::exists(target_path)) {
            fs::create_directories(target_path);
        }

        if (info.read_only) {
            mount(
                info.source.c_str(), target_path.c_str(),
                nullptr, MS_BIND | MS_REC | MS_RDONLY, nullptr
            );
        }
        else {
            mount(
                info.source.c_str(), target_path.c_str(),
                nullptr, MS_BIND | MS_REC, nullptr
            );
        }
    }

    // TODO: check if param.chroot_path exsits

    chroot(param.chroot_path.c_str());
    chdir(param.work_path.c_str());

    // Redirect stdin, stdout, stderr
    int null_fd = open("/dev/null", O_RDWR);
    int stdin_fd, stdout_fd, stderr_fd;

    if (param.redirect_stdin) {
        stdin_fd = open(param.stdin_path.c_str(), O_RDONLY);
    }
    else {
        stdin_fd = null_fd;
    }
    if (!param.stdin_path.empty()) {
        dup2(stdin_fd, STDIN_FILENO);
    }

    if (param.redirect_stdout) {
        stdout_fd = open(
            param.stdout_path.c_str(),
            O_WRONLY | O_TRUNC | O_CREAT,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
        );
    }
    else {
        stdout_fd = null_fd;
    }
    if (!param.stdout_path.empty()) {
        dup2(stdout_fd, STDOUT_FILENO);
    }

    if (param.redirect_stderr) {
        if (param.stderr_path == param.stdout_path) {
            stderr_fd = stdout_fd;
        }
        else {
            stderr_fd = open(
                param.stderr_path.c_str(),
                O_WRONLY | O_TRUNC | O_CREAT,
                S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
            );
        }
    }
    else {
        stderr_fd = null_fd;
    }
    if (!param.stderr_path.empty()) {
        dup2(stderr_fd, STDERR_FILENO);
    }

    // Set resource limit
    //rlimit rlim_core, rlim_time, rlim_memory, rlim_proc;
    //rlim_core.rlim_max = rlim_core.rlim_cur = 0;
    //rlim_time.rlim_max = rlim_time.rlim_cur = param.time_limit;
    //rlim_memory.rlim_max = rlim_memory.rlim_cur = param.memory_limit;
    //rlim_proc.rlim_max = rlim_proc.rlim_cur = param.process_limit;
    //setrlimit(RLIMIT_CORE, &rlim_core);
    //setrlimit(RLIMIT_CPU, &rlim_time);
    //setrlimit(RLIMIT_AS, &rlim_memory);
    //setrlimit(RLIMIT_STACK, &rlim_memory);
    //setrlimit(RLIMIT_NPROC, &rlim_proc);

    // Run command
    char **argv = new char *[param.argv.size() + 1];
    for (int i = 0; i < param.argv.size(); ++i) {
        argv[i] = const_cast<char *>(param.argv[i].c_str());
    }
    argv[param.argv.size()] = nullptr;

    execvp(argv[0], argv);
    std::clog << "ERR! execvp " << errno << std::endl;
    // exec failed
    return -1;
}

hoj::sandbox_result hoj::start_sandbox(const hoj::sandbox_param &param) {
    constexpr unsigned int CHILD_STACK_SIZE = 1024 * 1024; // 1 MiB

    unsigned char *child_stack = new unsigned char[CHILD_STACK_SIZE];

    pid_t child_pid = clone(
        child_process, child_stack + CHILD_STACK_SIZE,
        CLONE_NEWNET | CLONE_NEWNS | CLONE_NEWPID | CLONE_NEWUTS | SIGCHLD,
        const_cast<void *>(reinterpret_cast<const void *>(&param))
    );

    cgroup_info cgroup;
    cgroup.name = param.cgroup_name;
    cgroup.controller = { "cpuacct", "memory", "pids" };

    if (!cgroup_initialized()) {
        init_cgroup();
    }

    if (check_cgroup(cgroup)) {
        remove_cgroup(cgroup);
    }

    create_cgroup(cgroup);
    write_cgroup_property(cgroup, "memory", "memory.limit_in_bytes",
        param.memory_limit);
    write_cgroup_property(cgroup, "memory", "memory.memsw.limit_in_bytes",
        param.memory_limit);
    write_cgroup_property(cgroup, "pids", "pids.max", param.process_limit);
    write_cgroup_property(cgroup, "cpuacct", "tasks", child_pid);
    write_cgroup_property(cgroup, "memory", "tasks", child_pid);
    write_cgroup_property(cgroup, "pids", "tasks", child_pid);

    if (child_pid == -1) {
        // TODO: error
        std::clog << "LOG! " << child_pid << " " << errno << std::endl;
    }

    auto clock_start = std::chrono::steady_clock::now();

    std::thread monitor([child_pid, &param]() -> void {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(param.time_limit + 200)
        );
        kill(child_pid, SIGKILL);
    });
    monitor.detach();

    int status = 0xffff;
    rusage res_usage;
    if (wait4(child_pid, &status, WSTOPPED, &res_usage) == -1) {
        // TODO: error
    }

    auto clock_end = std::chrono::steady_clock::now();

    delete[] child_stack;

    // TODO: Failure handler

    hoj::sandbox_result result;
    result.status = status;
    if (WIFEXITED(status)) {
        result.exit_code = WEXITSTATUS(status);
        result.signal = 0;
    }
    else if (WIFSIGNALED(status)) {
        result.exit_code = -1;
        result.signal = WTERMSIG(status);
    }
    result.cpu_time = read_cgroup_property<std::int64_t>(cgroup,
        "cpuacct", "cpuacct.usage") / 1'000'000;
    result.real_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        clock_end - clock_start).count();
    result.memory = read_cgroup_property<std::int32_t>(cgroup,
        "memory", "memory.memsw.max_usage_in_bytes");

    remove_cgroup(cgroup);

    return result;
}
