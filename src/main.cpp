#include <iostream>

#include <unistd.h>

#include "run.h"
#include "config_loader.h"

int main(int argc, char *argv[]) {
    hoj::config_loader config_judger;
    hoj::config_loader config_test;
    if (argc > 3) {
        config_judger.read(argv[1]);
        config_test.read(argv[2]);
    }
    else {
        return -1;
    }

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

    char cwd[1024];
    getcwd(cwd, 1024);
    hoj::config_loader config_result;
    config_result.set("cwd", cwd);
    
    config_result.set("mode", config_test.get("mode"));
    config_result.set("result", std::to_string(static_cast<int>(result.result)));
    config_result.set("cpu_time", std::to_string(result.cpu_time));
    config_result.set("real_time", std::to_string(result.real_time));
    config_result.set("memory", std::to_string(result.memory));
    config_result.set("exit_code", std::to_string(result.exit_code));
    config_result.write(argv[3]);

    return 0;
}
