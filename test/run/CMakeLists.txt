set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(PROJECT_SRC_FILES
    main.cpp
    ../../src/sandbox.cpp
    ../../src/cgroup.cpp
    ../../src/util.cpp
    ../../src/run.cpp
)

add_executable(test-run ${PROJECT_SRC_FILES})
target_link_libraries(test-run pthread)
