set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(PROJECT_SRC_FILES
    test.cpp
    ../../src/sandbox.cpp
    ../../src/cgroup.cpp
    ../../src/util.cpp
)

add_executable(test-sandbox ${PROJECT_SRC_FILES})
target_link_libraries(test-sandbox pthread)
