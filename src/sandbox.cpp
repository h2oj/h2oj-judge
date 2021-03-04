/*
    This file is part of the Hydrogen OJ Judger.

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

#include <filesystem>

#include <fcntl.h>
#include <sched.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#include <iostream>
#include <cerrno>

namespace fs = std::filesystem;

int child_process(void *arg) {
    hoj::sandbox::sandbox_param &param = *reinterpret_cast<hoj::sandbox::sandbox_param *>(arg);

    // TODO: check if param.chroot_path exsits

    std::clog << chroot(param.chroot_path.c_str()) << std::endl;
    //chdir(param.work_path.c_str());

    // Redirect stdin, stdout, stderr
    int null_fd = open("/dev/null", O_RDWR);
    int stdin_fd, stdout_fd, stderr_fd;

    if (param.redirect_stdin) {
        stdin_fd = open(param.stdin_path.c_str(), O_RDONLY);
    }
    else {
        stdin_fd = null_fd;
    }
    //dup2(stdin_fd, STDIN_FILENO);

    if (param.redirect_stdout) {
        stdout_fd = open(param.stdout_path.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    }
    else {
        stdout_fd = null_fd;
    }
    //dup2(stdout_fd, STDOUT_FILENO);

    if (param.redirect_stderr) {
        if (param.stderr_path == param.stdout_path) {
            stderr_fd = stdout_fd;
        }
        else {
            stderr_fd = open(param.stderr_path.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
        }
    }
    else {
        stderr_fd = null_fd;
    }
    //dup2(stderr_fd, STDERR_FILENO);

    // Set resource limit
    rlimit rlim_core;
    rlim_core.rlim_max = rlim_core.rlim_cur = 0;
    setrlimit(RLIMIT_CORE, &rlim_core);

    // Run command
    char **argv = new char *[param.argv.size() + 1];
    for (int i = 0; i < param.argv.size(); ++i) {
        argv[i] = const_cast<char *>(param.argv[i].c_str());
    }
    argv[param.argv.size()] = nullptr;

    std::clog << param.command.c_str() << std::endl;
    for (int i = 0; i < param.argv.size(); ++i) {
        std::clog << argv[i] << " ";
    }
    std::clog << (argv[param.argv.size()] == nullptr) << std::endl;

    execvp(argv[0], argv);
    std::clog << "ERR! execvp " << errno << std::endl;
    // exec failed
    return -1;
}

int hoj::sandbox::start_sandbox(const hoj::sandbox::sandbox_param &param) {
    constexpr unsigned int CHILD_STACK_SIZE = 1024 * 1024; // 1 MiB

    unsigned char *child_stack = new unsigned char[CHILD_STACK_SIZE];

    pid_t child_pid = clone(child_process, child_stack + CHILD_STACK_SIZE,
        //CLONE_NEWNET | CLONE_NEWNS | CLONE_NEWPID | CLONE_NEWUTS | SIGCHLD,
        CLONE_NEWNS | SIGCHLD,
        const_cast<void *>(reinterpret_cast<const void *>(&param)));
    if (child_pid == -1) std::clog << "LOG! " << child_pid << " " << errno << std::endl;
    int status;
    rusage res_usage;
    if (wait4(child_pid, &status, WSTOPPED, &res_usage) == -1) {
        return -2;
    }

    return status;
}
