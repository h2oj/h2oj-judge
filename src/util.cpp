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

#include "util.h"

#include <cctype>
#include <cstdlib>
#include <map>
#include <random>
#include <string>
#include <vector>

std::string hoj::format(const std::string &src, const std::map<std::string, std::string> &data) {
    std::string result = "";
    std::string key = "";
    bool is_key = false;
    for (const auto &c : src) {
        if (is_key) {
            if (!isalpha(c)) {
                is_key = false;
                auto iter = data.find(key);
                if (iter != data.end()) result.append(iter->second);
                result.push_back(c);
                key = "";
            }
            else key.push_back(c); 
        }
        else {
            if (c == '%') {
                is_key = true;
            }
            else result.push_back(c);
        }
    }
    if (!key.empty()) {
        auto iter = data.find(key);
        if (iter != data.end()) result.append(iter->second);
    }
    return result;
}

char hoj::transfer_char(const char &key) {
    switch (key) {
        case 'n': return '\n';
        case 'r': return '\r';
        case 't': return '\t';
        case '\"': return '\"';
        case '\'': return '\'';
        default: return ' ';
    }
}

std::vector<std::string> hoj::parse_command(const std::string &command) {
    std::vector<std::string> split_result;
    bool transfer_mode = false;
    bool in_quote = false;
    std::string current_string = "";
    for (const auto &c : command) {
        if (c == '\'') transfer_mode = true;
        if (transfer_mode) {
            transfer_mode = false;
            current_string.push_back(hoj::transfer_char(c));
        }
        else {
            if (c == '\"') in_quote = !in_quote;
            else {
                if (!in_quote && c == ' ') {
                    if (current_string.size() > 0) {
                        split_result.push_back(current_string);
                        current_string.clear();
                    }
                }
                else {
                    current_string.push_back(c);
                }
            }
        }
    }
    if (current_string.size() > 0) split_result.push_back(current_string);
    return split_result;
}

std::string hoj::random_string(int length) {
    std::string result;
    std::default_random_engine random;
    std::uniform_int_distribution<unsigned> dist(1, 10 + 26 * 2);

    for (int i = 0; i < length; ++i) {
        auto temp = dist(random);
        if (temp <= 10) result.push_back(temp - 1 + '0');
        else if (temp <= 36) result.push_back(temp - 11 + 'A');
        else result.push_back(temp - 37 + 'a');
    }

    return result;
}
