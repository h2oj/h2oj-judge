#include <iostream>

#include "../../src/sandbox.h"
#include "../../src/util.h"

using namespace std;
using namespace hoj;

auto compile() {
    auto split_result = parse_command("g++ -o a.out mle.cpp -DTEST");
    
    sandbox_param param {
        .cgroup_name = "hoj-test-sandbox-0",
        .memory_limit = 1024 * 1024 * 1024,
        .time_limit = 5000,
        .process_limit = -1,
        .chroot_path = "sandbox/rootfs",
        .work_path = "/sandbox",
        .redirect_stdin = false,
        .redirect_stdout = true,
        .redirect_stderr = true,
        .stdout_path = "compile.txt",
        .stderr_path = "compile.txt",
        .mount = {
            { "src_dir", "sandbox", true }
        },
        .command = split_result[0],
        .argv = split_result
    };

    return start_sandbox(param);
}

auto run() {
    auto split_result = parse_command("./a.out");
    
    sandbox_param param {
        .cgroup_name = "hoj-test-sandbox-1",
        .memory_limit = 1024 * 1024 * 128,
        .time_limit = 1000,
        .process_limit = 1,
        .chroot_path = "sandbox/rootfs",
        .work_path = "/sandbox",
        .redirect_stdin = true,
        .redirect_stdout = true,
        .redirect_stderr = false,
        .stdin_path = "stdin.txt",
        .stdout_path = "stdout.txt",
        .mount = {
            { "src_dir", "sandbox", true }
        },
        .command = split_result[0],
        .argv = split_result
    };

    return start_sandbox(param);
}

void print_result(const sandbox_result &result) {
    cout << "status: " << result.status << endl
        << "exit_code: " << result.exit_code << endl
        << "signal: " << result.signal << endl
        << "cpu_time: " << result.cpu_time << endl
        << "real_time: " << result.real_time << endl
        << "memory: " << result.memory << endl;
}

int main() {
    cout << "Compile" << endl;
    print_result(compile());
    cout << "Run" << endl;
    print_result(run());
    return 0;
}
