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

#ifndef _HOJ_JUDGER_UTIL_H_
#define _HOJ_JUDGER_UTIL_H_

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace hoj {
    std::string format(const std::string &src, const std::map<std::string, std::string> &data);
    char transfer_char(const char &key);
    std::vector<std::string> parse_command(const std::string &command);
    std::string random_string(int length);
}

#endif // _HOJ_JUDGER_UTIL_H_
