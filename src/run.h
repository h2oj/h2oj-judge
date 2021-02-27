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

#ifndef _HOJ_JUDGER_RUN_H_
#define _HOJ_JUDGER_RUN_H_

#include <filesystem>
#include <string>

#include "result.h"

namespace fs = std::filesystem;

namespace hoj {
    int compile(resource_usage &usage, const std::string &command, const fs::path &output, unsigned int time_limit, unsigned int space_limit);
    int run(resource_usage &usage, const std::string &command, const fs::path &input, const fs::path &output, unsigned int time_limit, unsigned int space_limit);
    int check(resource_usage &usage, const fs::path &checker, const fs::path &input, const fs::path &output, const fs::path &answer, unsigned int time_limit, unsigned int space_limit);
}

#endif // _HOJ_JUDGER_RUN_H_
