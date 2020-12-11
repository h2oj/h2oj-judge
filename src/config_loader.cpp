#include "config_loader.h"

namespace hoj {

config_loader::config_loader() {}

config_loader::~config_loader() {}

int config_loader::read(std::string file_path) {
    if (this->file.is_open()) file.close();
    this->file.open(file_path, std::ios::in);
    if (this->file.is_open()) this->parse();
    return this->file.is_open();
}

int config_loader::write(std::string file_path) {
    if (this->file.is_open()) file.close();
    this->file.open(file_path, std::ios::out | std::ios::trunc);
    if (this->file.is_open()) this->parse();
    return this->file.is_open();
}

std::string config_loader::get(std::string key) {
    auto iter = this->conf.find(key);
    if (iter == this->conf.end()) return "";
    return iter->second;
}

void config_loader::set(std::string key, std::string value) {
    this->conf[key] = value;
}

int config_loader::parse() {
    std::string str;
    while (!file.eof()) {
        getline(file, str);
        std::size_t index = str.find_first_of('=');
        std::string key = str.substr(0, index);
        std::string value = str.substr(index + 1);
        if (key.size() == 0) continue;
        this->conf[key] = value;
    }
    return 0;
}

}
