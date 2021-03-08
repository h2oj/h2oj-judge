cmake_minimum_required(VERSION 3.4 FATAL_ERROR)
project(HOJ_JUDGER VERSION 0.2.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/third_party)

aux_source_directory(${PROJECT_SOURCE_DIR}/src PROJECT_SRC_FILES)

add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/yaml-cpp)

add_executable(hoj-judger ${PROJECT_SRC_FILES})
target_link_libraries(hoj-judger yaml-cpp pthread)

add_subdirectory(test/cgroup)
add_subdirectory(test/sandbox)
enable_testing()
add_test(NAME test-cgroup COMMAND test-cgroup)
add_test(NAME test-sandbox COMMAND test-sandbox)
