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

#ifndef _HOJ_JUDGER_CGROUP_H_
#define _HOJ_JUDGER_CGROUP_H_

#include <cstdint>
#include <string>
#include <vector>

namespace hoj {
    struct cgroup_info {
        std::string name;
        std::vector<std::string> controller;
    };

    int init_cgroup();
    bool cgroup_initialized();
    int create_cgroup(const cgroup_info &info);
    int remove_cgroup(const cgroup_info &info);
    int check_cgroup(const cgroup_info &info);

    // Only for std::int32_t, std::int64_t, std::string
    template <typename value_type>
    int write_cgroup_property(
        const cgroup_info &info, const std::string &controller,
        const std::string &property, const value_type &value,
        bool overwrite = true
    );

    // Only for std::int32_t, std::int64_t, std::string
    template <typename value_type>
    value_type read_cgroup_property(
        const cgroup_info &info, const std::string &controller,
        const std::string &property
    );
}

#endif // _HOJ_JUDGER_CGROUP_H_
