#include "util.h"

#include <cctype>
#include <cstdlib>
#include <map>
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
