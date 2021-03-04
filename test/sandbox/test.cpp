#include <iostream>

#include "../../src/sandbox.h"

using namespace std;
using namespace hoj::sandbox;

int main() {
    sandbox_param param;
    param.memory_limit = 1024 * 1024 * 1024;
    param.process_limit = 1;
    param.chroot_path = "./sandbox";
    param.work_path = "./sandbox";
    param.redirect_stdin = false;
    param.redirect_stdout = false;
    param.stdout_path = "stdout.txt";
    param.redirect_stderr = false;
    param.stderr_path = "stdout.txt";
    param.command = "ls";
    param.argv.push_back("ls");
    cout << start_sandbox(param) << endl;
    return 0;
}
