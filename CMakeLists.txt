cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(HOJ_JUDGER VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/third_party)

aux_source_directory(${PROJECT_SOURCE_DIR}/src PROJECT_SRC_FILES)

add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/yaml-cpp)

add_executable(hoj-judger ${PROJECT_SRC_FILES})
target_link_libraries(hoj-judger yaml-cpp pthread)

add_custom_command(
    TARGET hoj-judger
    COMMAND cp ../third_party/hoj-testlib/bin/noip hoj-checker &&
        cp hoj-judger ../node/bin/hoj-judger &&
        cp hoj-checker ../node/bin/hoj-checker
)
