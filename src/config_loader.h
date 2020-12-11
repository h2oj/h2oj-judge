#pragma once

#include <string>
#include <fstream>
#include <map>

namespace hoj {

class config_loader {
    std::fstream file;
    std::map<std::string, std::string> conf;
public:
    config_loader();
    ~config_loader();
    int read(std::string file_path);
    int write(std::string file_path);
    std::string get(std::string key);
    void set(std::string key, std::string value);
private:
    int parse();
};

}
