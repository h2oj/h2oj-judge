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

#include "cgroup.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include <mntent.h>

namespace fs = std::filesystem;

static std::map<std::string, std::vector<fs::path>> mount_table;
static bool initialized = false;

int hoj::init_cgroup() {
    mount_table.clear();
    
    std::ifstream proc_cgroup("/proc/cgroups");
    if (!proc_cgroup.is_open()) {
        // TODO: error
    }

    std::string subsys_name;
    int hierarchy, num_cgroups, enabled;
    std::vector<std::string> controller;

    proc_cgroup.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (!proc_cgroup.eof()) {
        proc_cgroup >> subsys_name >> hierarchy >> num_cgroups >> enabled;
        controller.push_back(subsys_name);
    }

    FILE *proc_mount = fopen("/proc/mounts", "re");
    if (proc_mount == nullptr) {
        // TODO: error
    }

    mntent *ent = nullptr;
    mntent *temp_ent = new mntent;
    char mntent_buffer[4 * FILENAME_MAX];
    while (
        (ent = getmntent_r(
            proc_mount, temp_ent, mntent_buffer, sizeof(mntent_buffer))
        ) != nullptr
    ) {
        if (strcmp(ent->mnt_type, "cgroup")) {
            continue;
        }

        for (const auto &name : controller) {
            char *mntopt = hasmntopt(ent, name.c_str());
            if (!mntopt) {
                continue;
            }

            mount_table[name].push_back(ent->mnt_dir);
        }
    }

    delete temp_ent;
    fclose(proc_mount);
    proc_cgroup.close();

    initialized = true;

    return 0;
}

bool hoj::cgroup_initialized() {
    return initialized;
}

static fs::path get_cgroup_path(const std::string &controller) {
    auto iter = mount_table.find(controller);
    if (iter == mount_table.end()) {
        // TODO: error
        return "";
    }

    return iter->second[0];
}

int hoj::create_cgroup(const hoj::cgroup_info &info) {
    for (const auto &controller : info.controller) {
        fs::create_directories(get_cgroup_path(controller) / info.name);
    }
    return 0;
}

int hoj::remove_cgroup(const cgroup_info &info) {
    for (const auto &controller : info.controller) {
        auto target = get_cgroup_path(controller) / info.name;
        if (fs::exists(target)) {
            fs::remove(target);
        }
    }
    return 0;
}

int hoj::check_cgroup(const cgroup_info &info) {
    for (const auto &controller : info.controller) {
        auto target = get_cgroup_path(controller) / info.name;
        if (fs::exists(target)) {
            return 1;
        }
    }
    return 0;
}

template <typename value_type>
static int write_file(
    const fs::path &path, const value_type &value,
    bool overwrite = false
) {
    std::ofstream file(
        path,
        overwrite ? std::ofstream::trunc : std::ofstream::app
    );
    if (!file.is_open()) {
        // TODO: error
        return 1;
    }
    file << value << std::endl;
    file.close();
    return 0;
}

template <typename value_type>
static value_type read_file(const fs::path &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        // TODO: error
        return value_type();
    }
    value_type value;
    file >> value;
    file.close();
    return value;
}

template <typename value_type>
static std::vector<value_type> read_file_array(const fs::path &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        // TODO: error
        return std::vector<value_type>();
    }
    value_type temp;
    std::vector<value_type> value;
    while (!file.eof()) {
        file >> temp;
        value.push_back(temp);
    }
    return value;
}

template <typename value_type>
int hoj::write_cgroup_property(
    const hoj::cgroup_info &info, const std::string &controller,
    const std::string &property, const value_type &value, bool overwrite
) {
    return write_file(
        get_cgroup_path(controller) / info.name / property,
        value, overwrite
    );
}

template int hoj::write_cgroup_property<std::int32_t>(
    const hoj::cgroup_info &info, const std::string &controller,
    const std::string &property, const std::int32_t &value, bool overwrite
);

template int hoj::write_cgroup_property<std::int64_t>(
    const hoj::cgroup_info &info, const std::string &controller,
    const std::string &property, const std::int64_t &value, bool overwrite
);

template int hoj::write_cgroup_property<std::string>(
    const hoj::cgroup_info &info, const std::string &controller,
    const std::string &property, const std::string &value, bool overwrite
);

template <typename value_type>
value_type hoj::read_cgroup_property(
    const hoj::cgroup_info &info, const std::string &controller,
    const std::string &property
) {
    return read_file<value_type>(
        get_cgroup_path(controller) / info.name / property
    );
}

template std::int32_t hoj::read_cgroup_property<std::int32_t>(
    const hoj::cgroup_info &info, const std::string &controller,
    const std::string &property
);

template std::int64_t hoj::read_cgroup_property<std::int64_t>(
    const hoj::cgroup_info &info, const std::string &controller,
    const std::string &property
);
