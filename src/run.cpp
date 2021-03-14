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

#include "cgroup.h"
#include "result.h"
#include "sandbox.h"
#include "util.h"

namespace fs = std::filesystem;

hoj::sandbox_result hoj::compile(
    const fs::path &sandbox_path, const fs::path &work_path,
    const std::string &command,
    const std::string &source_file, const std::string &exec_file,
    int time_limit, int space_limit
) {
    std::map<std::string, std::string> format_data;
    format_data["source"] = fs::path("source") / source_file;
    format_data["executable"] = fs::path("binary") / exec_file;

    auto parsed_argv = hoj::parse_command(hoj::format(command, format_data));
    
    hoj::sandbox_param param;
    param.cgroup_name = "hoj-judger-" + hoj::random_string(8);
    param.memory_limit = space_limit;
    param.time_limit = time_limit;
    param.process_limit = -1;
    param.chroot_path = sandbox_path;
    param.work_path = "/sandbox";
    param.redirect_stdin = false;
    param.redirect_stdout = true;
    param.redirect_stderr = true;
    param.stdout_path = "binary/compile.txt";
    param.stderr_path = "binary/compile.txt";
    param.mount = {
        { work_path / "source", "sandbox/source", true },
        { work_path / "binary", "sandbox/binary", false }
    };
    param.command = parsed_argv[0];
    param.argv = parsed_argv;

    return start_sandbox(param);
}

hoj::sandbox_result hoj::compile_checker(
    const fs::path &sandbox_directory,
    const fs::path &source_directory, const std::string &source_name,
    const fs::path &binary_directory, const std::string &binary_name,
    const std::string &command,
    int time_limit, int space_limit
) {
    std::map<std::string, std::string> format_var;
    format_var["source"] = fs::path("source") / source_name;
    format_var["executable"] = fs::path("binary") / binary_name;

    auto parsed_cmd = hoj::parse_command(hoj::format(command, format_var));
    
    hoj::sandbox_param param;
    param.cgroup_name = "hoj-judger-" + hoj::random_string(8);
    param.memory_limit = space_limit;
    param.time_limit = time_limit;
    param.process_limit = -1;
    param.chroot_path = sandbox_directory;
    param.work_path = "/sandbox";
    param.redirect_stdin = false;
    param.redirect_stdout = false;
    param.redirect_stderr = false;
    //param.stdout_path = "binary/compile.txt";
    //param.stderr_path = "binary/compile.txt";
    param.mount = {
        { source_directory, "sandbox/source", false },
        { binary_directory, "sandbox/binary", false }
    };
    param.command = parsed_cmd[0];
    param.argv = parsed_cmd;

    return start_sandbox(param);
}

hoj::sandbox_result hoj::run(
    const fs::path &sandbox_path, const fs::path &work_path,
    const std::string &command,
    const std::string &source_file, const std::string &exec_file,
    const fs::path &input, const fs::path &output,
    int time_limit, int space_limit
) {
    std::map<std::string, std::string> format_data;
    format_data["source"] = fs::path("source") / source_file;
    format_data["executable"] = fs::path("binary") / exec_file;

    auto parsed_argv = hoj::parse_command(hoj::format(command, format_data));

    hoj::sandbox_param param;
    param.cgroup_name = "hoj-judger-" + hoj::random_string(8);
    param.memory_limit = space_limit;
    param.time_limit = time_limit;
    param.process_limit = 1;
    param.chroot_path = sandbox_path;
    param.work_path = "/sandbox";
    param.redirect_stdin = true;
    param.redirect_stdout = true;
    param.redirect_stderr = false;
    param.stdin_path = "working/input.in";
    param.stdout_path = "working/output.out";
    param.mount = {
        { work_path / "source", "sandbox/source", true },
        { work_path / "binary", "sandbox/binary", false },
        { work_path / "working", "sandbox/working", false }
    };
    param.command = parsed_argv[0];
    param.argv = parsed_argv;

    // Copy input file
    fs::copy_file(
        input, work_path / "working" / "input.in",
        fs::copy_options::overwrite_existing
    );

    return start_sandbox(param);
}

hoj::sandbox_result hoj::run_checker(
    const fs::path &sandbox_directory,
    const fs::path &checker_directory, const std::string &checker_name,
    const fs::path &input_path,
    const fs::path &output_path,
    const fs::path &answer_path
    const std::string &command,
    int time_limit, int space_limit
) {

}

hoj::sandbox_result hoj::check(
    const fs::path &sandbox_path, const fs::path &work_path,
    const fs::path &checker,
    const fs::path &input, const fs::path &output, const fs::path &answer,
    int time_limit, int space_limit
) {
    hoj::sandbox_param param;
    param.cgroup_name = "hoj-judger-" + hoj::random_string(8);
    param.memory_limit = space_limit;
    param.time_limit = time_limit;
    param.process_limit = 1;
    param.chroot_path = sandbox_path;
    param.work_path = "/sandbox";
    param.redirect_stdin = false;
    param.redirect_stdout = true;
    param.redirect_stderr = false;
    param.stdout_path = "working/checker.txt";
    param.mount = {
        { work_path / "source", "sandbox/source", true },
        { work_path / "binary", "sandbox/binary", false },
        { work_path / "working", "sandbox/working", false }
    };
    param.command = checker;
    param.argv = {
        checker,
        "working/input.in", "working/output.out", "working/answer.ans"
    };
    
    // Copy input and answer file
    fs::copy_file(
        input, work_path / "working" / "input.in",
        fs::copy_options::overwrite_existing
    );
    fs::copy_file(
        answer, work_path / "working" / "answer.ans",
        fs::copy_options::overwrite_existing
    );

    return start_sandbox(param);
}
