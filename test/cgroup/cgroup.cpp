#include <iostream>

#include "../../src/cgroup.h"

using namespace std;
using namespace hoj;

int main() {
    cgroup_info info {
        .name = "hoj-test-cgroup",
        .controller = { "memory", "pids", "cpuacct" }
    };

    init_cgroup();
    if (check_cgroup(info)) remove_cgroup(info);
    create_cgroup(info);
    write_cgroup_property(info, "memory", "memory.limit_in_bytes", 128 * 1024 * 1024);
    write_cgroup_property(info, "memory", "memory.memsw.limit_in_bytes", 128 * 1024 * 1024);
    write_cgroup_property(info, "pids", "pids.max", 10);

    return 0;
}
