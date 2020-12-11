#include <iostream>

#include "run.h"

int main(int argc, char *argv[]) {
    std::cout << "====================" << std::endl;
    std::cout << "     HOJ Judger     " << std::endl;
    std::cout << "====================" << std::endl;

    hoj::config conf;
    conf.max_memory = 512 * 1024 * 1024;
    conf.max_cpu_time = 1000;
    conf.max_real_time = 1000;
    conf.log_path = "./log.txt";
    conf.input_path = "./test.in";
    conf.output_path = "./test.out";
    conf.answer_path = "./test.ans";
    conf.program_path = "./exe";

    hoj::compile("src.cpp");
    hoj::result result = hoj::run(conf);

    std::cout << "====================" << std::endl;
    std::cout << "Result: " << static_cast<int>(result.result) << std::endl;
    std::cout << "CPU Time: " << result.cpu_time << "ms" << std::endl;
    std::cout << "Real Time: " << result.real_time << "ms" << std::endl;
    std::cout << "Memory Usage: " << result.memory << "KiB" << std::endl;
    std::cout << "Exit Code: " << result.exit_code << std::endl;
    std::cout << "====================" << std::endl;
    return 0;
}
