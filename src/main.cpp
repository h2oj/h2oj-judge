#include <iostream>

#include <unistd.h>

#include "run.h"
#include "config_loader.h"

int main(int argc, char *argv[]) {
    if (argc > 1) {
        chdir(argv[1]);
    }

    hoj::config_loader config_test;
    config_test.read("./test.conf");

    hoj::config_loader config_judger;
    config_judger.read(config_test.get("judger_config"));

    hoj::config_loader config_result;

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
    config_result.set("cwd", cwd);
    
    config_result.set("mode", config_test.get("mode"));
    config_result.set("result", std::to_string(static_cast<int>(result.result)));
    config_result.set("cpu_time", std::to_string(result.cpu_time));
    config_result.set("real_time", std::to_string(result.real_time));
    config_result.set("memory", std::to_string(result.memory));
    config_result.set("exit_code", std::to_string(result.exit_code));
    config_result.write("./result.conf");

    return 0;
}
