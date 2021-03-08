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

#ifndef _HOJ_JUDGER_SANDBOX_H_
#define _HOJ_JUDGER_SANDBOX_H_

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace hoj {
    struct mount_info {
        std::filesystem::path source;
        std::filesystem::path target;
        bool read_only;
    };

    struct sandbox_param {
        // cgroup name
        std::string cgroup_name;
        // set space limit (Byte)
        int64_t memory_limit;
        // set time limit
        int time_limit;
        // set process limit
        int process_limit;
        // chroot path
        std::filesystem::path chroot_path;
        // working directory
        std::filesystem::path work_path;
        // redirect io
        bool redirect_stdin;
        bool redirect_stdout;
        bool redirect_stderr;
        std::filesystem::path stdin_path;
        std::filesystem::path stdout_path;
        std::filesystem::path stderr_path;
        // mount file
        std::vector<mount_info> mount;
        // run command
        std::string command;
        std::vector<std::string> argv;
    };

    int start_sandbox(const sandbox_param &param);
}

#endif // _HOJ_JUDGER_SANDBOX_H_
