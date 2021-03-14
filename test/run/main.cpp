#include <iostream>

#include "../../src/run.h"

using namespace std;
using namespace hoj;

int main() {
    cout << "=====COMPILE=====" << endl;
    auto result = compile(
        "sandbox/rootfs",
        "work_dir",
        "g++ \"%source\" -o \"%executable\" -DONLINE_JUDGE -DHOJ_JUDGER",
        "test.cpp",
        "a.out",
        5000,
        1024 * 1024 * 1024
    );
    cout << "Status: " << result.status << endl;
    cout << "Exit Code: " << result.exit_code << endl;
    cout << "Signal: " << result.signal << endl;
    cout << "Cpu Time: " << result.cpu_time << endl;
    cout << "Real Time: " << result.real_time << endl;
    cout << "Memory: " << result.memory << endl;
    cout << "=====COMPILE CHECKER=====" << endl;
    result = compile_checker(
        "sandbox/rootfs",
        "data", "checker.cpp",
        "data/binary", "checker",
        "g++ \"%source\" -o \"%executable\" -DHOJ_CHECKER",
        5000, 1024 * 1024 * 1024
    );
    cout << "Status: " << result.status << endl;
    cout << "Exit Code: " << result.exit_code << endl;
    cout << "Signal: " << result.signal << endl;
    cout << "Cpu Time: " << result.cpu_time << endl;
    cout << "Real Time: " << result.real_time << endl;
    cout << "Memory: " << result.memory << endl;
    cout << "=====RUN=====" << endl;
    result = run(
        "sandbox/rootfs",
        "work_dir",
        "\"%executable\"",
        "test.cpp",
        "a.out",
        "data/test1.in",
        "output.out",
        1000,
        32 * 1024 * 1024
    );
    cout << "Status: " << result.status << endl;
    cout << "Exit Code: " << result.exit_code << endl;
    cout << "Signal: " << result.signal << endl;
    cout << "Cpu Time: " << result.cpu_time << endl;
    cout << "Real Time: " << result.real_time << endl;
    cout << "Memory: " << result.memory << endl;
    cout << "=====CHECK=====" << endl;
    result = check(
        "sandbox/rootfs",
        "data",
        "hoj-checker",
        "data/test1.in",
        "",
        "data/test1.out",
        1000,
        32 * 1024 * 1024
    );
    cout << "Status: " << result.status << endl;
    cout << "Exit Code: " << result.exit_code << endl;
    cout << "Signal: " << result.signal << endl;
    cout << "Cpu Time: " << result.cpu_time << endl;
    cout << "Real Time: " << result.real_time << endl;
    cout << "Memory: " << result.memory << endl;
    return 0;
}
