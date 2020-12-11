#include <iostream>

#include "run.h"
#include "config_loader.h"

int main(int argc, char *argv[]) {
    std::cout << "====================" << std::endl;
    std::cout << "     HOJ Judger     " << std::endl;
    std::cout << "====================" << std::endl;

    hoj::config_loader config_judger;
    config_judger.read("./judger.conf");
    hoj::config_loader config_test;
    config_test.read("./test.conf");

    hoj::config conf;
    conf.judger_config = &config_judger;
    conf.test_config = &config_test;

    hoj::result result;
    if (config_test.get("mode") == "compile") {
        result = hoj::compile(conf);
    }
    else {
        result = hoj::run(conf);
    }

    std::cout << "Result: " << static_cast<int>(result.result) << std::endl;
    std::cout << "CPU Time: " << result.cpu_time << "ms" << std::endl;
    std::cout << "Real Time: " << result.real_time << "ms" << std::endl;
    std::cout << "Memory Usage: " << result.memory << "KiB" << std::endl;
    std::cout << "Exit Code: " << result.exit_code << std::endl;
    std::cout << "====================" << std::endl;
    
    return 0;
}
