set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(PROJECT_SRC_FILES
    cgroup.cpp
    ../../src/cgroup.cpp
)

add_executable(test-cgroup ${PROJECT_SRC_FILES})
