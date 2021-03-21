/*
    This file is part of the Hydrogen OJ Judger.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/* Copyright (C) 2020-2021 Alex Cui */

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "../third_party/argparse/argparse.h"
#include "../third_party/yaml-cpp/include/yaml-cpp/yaml.h"

#include "def.h"
#include "result.h"
#include "run.h"
#include "sandbox.h"
#include "util.h"

namespace fs = std::filesystem;

static void fill_testcase_status(
    YAML::Node &result, int testcase_count,
    int memory, int time, int score, hoj::test_case_status status
) {
    result["case_count"] = testcase_count;
    for (int i = 0; i < testcase_count; ++i) {
        result["case"][i]["memory"] = memory;
        result["case"][i]["time"] = time;
        result["case"][i]["score"] = score;
        result["case"][i]["status"] = status;
    }
}

static std::string read_file(std::ifstream &fin) {
    std::string result, temp;
    while (!fin.eof()) {
        getline(fin, temp);
        temp.push_back('\n');
        result += temp;
    }
    return result;
}

static inline void save_result(const YAML::Node &result, std::ofstream &fout) {
    fout << result;
    fout.close();
}

static inline void print_error(const std::string &str) {
    std::cerr << str << std::endl;
}

template<>
class YAML::convert<hoj::test_case_status> {
public:
    static inline Node encode(const hoj::test_case_status &status) {
        return YAML::convert<int>::encode(static_cast<int>(status));
    }
};

template<>
class YAML::convert<hoj::judge_status> {
public:
    static inline Node encode(const hoj::judge_status &status) {
        return YAML::convert<int>::encode(static_cast<int>(status));
    }
};

int judge_program(argparse::ArgumentParser &parser) {
    if (!parser.exists("problem")) {
        print_error("Missing argument(s): problem");
        return -1;
    }
    if (!parser.exists("checker")) {
        print_error("Missing argument(s): checker");
        return -1;
    }

    const fs::path judger_config_path = "./hoj-judger-config.yml";
    const fs::path problem_path = parser.get<std::string>("problem");
    const fs::path problem_config_path = problem_path / "config.yml";
    const fs::path work_path = parser.get<std::string>("workdir");
    const fs::path source_directory = work_path / "source";
    const fs::path binary_directory = work_path / "binary";
    const fs::path working_directory = work_path / "working";
    const fs::path result_path = working_directory / "result.yml";
    const fs::path sandbox_directory = parser.get<std::string>("sandbox");
    const std::string language = parser.get<std::string>("type");
    const std::string source_name = parser.get<std::string>("source");
    const fs::path checker_path = parser.get<std::string>("checker");
    const fs::path checker_directory = checker_path.parent_path();
    const std::string checker_name = checker_path.filename();

    YAML::Node judger_config = YAML::LoadFile(judger_config_path);
    YAML::Node problem_config = YAML::LoadFile(problem_config_path);
    YAML::Node judge_result;

    std::ofstream result_file(result_path);
    if (!result_file.is_open()) {
        return -1;
    }

    YAML::Node language_config = judger_config["language"][language];
    if (!language_config) {
        // TODO: error
        std::cerr << "No such language in config." << std::endl
            << "Language: " << language << std::endl;
        return -1;
    }

    YAML::Node compile_config = language_config["compile"];
    YAML::Node run_config = language_config["run"];

    if (compile_config) {
        constexpr int max_compile_time = 5000;
        constexpr int max_compile_memory = 1024 * 1024 * 1024;

        std::string compile_command = compile_config.as<std::string>();

        hoj::sandbox_result result = hoj::compile(
            sandbox_directory,
            work_path,
            compile_command,
            source_name,
            "a.out",
            max_compile_time,
            max_compile_memory
        );

        judge_result["compile_time"] = result.real_time;
        judge_result["compile_memory"] = result.memory;

        // has signal
        if (result.signal) { // UKE
            judge_result["detail"] = "Signal " + std::to_string(result.signal);
            judge_result["status"] = hoj::judge_status::UNKNOWN_ERROR;
            int case_count = problem_config["case_count"].as<int>();
            fill_testcase_status(
                judge_result, case_count, 0, 0, 0,
                hoj::test_case_status::UNKNOWN_ERROR
            );

            save_result(judge_result, result_file);
            return 0;
        }
        // CTLE
        else if (result.real_time > max_compile_time) {
            judge_result["status"] = hoj::judge_status::COMPILE_ERROR;

            int case_count = problem_config["case_count"].as<int>();
            fill_testcase_status(
                judge_result, case_count, 0, 0, 0,
                hoj::test_case_status::COMPILE_TIME_LIMIT_EXCEEDED
            );

            save_result(judge_result, result_file);
            return 0;
        }
        // CMLE
        else if (result.memory > max_compile_memory) {
            judge_result["status"] = hoj::judge_status::COMPILE_ERROR;

            int case_count = problem_config["case_count"].as<int>();
            fill_testcase_status(
                judge_result, case_count, 0, 0, 0,
                hoj::test_case_status::COMPILE_MEMORY_LIMIT_EXCEEDED
            );

            save_result(judge_result, result_file);
            return 0;
        }
        // CE
        else if (result.exit_code != 0) {
            std::ifstream compile_log(binary_directory / "compile.txt");

            if (!compile_log.is_open()) { // SE
                judge_result["status"] = hoj::judge_status::SYSTEM_ERROR;
                judge_result["detail"] = "Failed to open compile log.";
                int case_count = problem_config["case_count"].as<int>();
                fill_testcase_status(
                    judge_result, case_count, 0, 0, 0,
                    hoj::test_case_status::SYSTEM_ERROR
                );

                save_result(judge_result, result_file);
                return 0;
            }
            
            std::string compile_detail = read_file(compile_log);

            judge_result["detail"] = compile_detail;
            judge_result["status"] = hoj::judge_status::COMPILE_ERROR;
            int case_count = problem_config["case_count"].as<int>();
            fill_testcase_status(
                judge_result, case_count, 0, 0, 0,
                hoj::test_case_status::COMPILE_ERROR
            );

            save_result(judge_result, result_file);
            return 0;
        }
    }

    if (run_config) {
        std::string run_command = run_config.as<std::string>();
        
        std::string file_prefix = problem_config["file"].as<std::string>();
        std::string judge_mode = problem_config["mode"].as<std::string>();
        std::string checker_type = problem_config["type"].as<std::string>();

        int testcase_count = problem_config["case_count"].as<int>();
        YAML::Node testcase_list = problem_config["case"];
        judge_result["case_count"] = testcase_count;

        int total_score = 0;
        int total_time = 0;
        int total_memory = 0;
        hoj::judge_status final_status = hoj::judge_status::ACCEPTED;

        for (int i = 0; i < testcase_count; ++i) {
            YAML::Node testcase = testcase_list[i];
            int max_time = testcase["time"].as<int>();
            int max_memory = testcase["memory"].as<int>();
            int score = testcase["score"].as<int>();

            std::string filename = file_prefix + std::to_string(i + 1);
            fs::path input_file = problem_path / (filename + ".in");
            fs::path output_file = working_directory / "output.out";
            fs::path answer_file = problem_path / (filename + ".out");

            hoj::sandbox_result result = hoj::run(
                sandbox_directory,
                work_path,
                run_command,
                source_name,
                "a.out",
                input_file,
                output_file,
                max_time,
                max_memory
            );

            judge_result["case"][i]["time"] = result.real_time;
            judge_result["case"][i]["memory"] = result.memory;

            total_memory = std::max(total_memory, result.memory);
            total_time += result.real_time;

            // has signal, UKE
            if (result.signal) {
                judge_result["case"][i]["detail"] = "Signal " + std::to_string(result.signal);
                judge_result["case"][i]["score"] = 0;
                judge_result["case"][i]["status"] = hoj::judge_status::UNKNOWN_ERROR;
                final_status = hoj::judge_status::UNACCEPTED;
            }
            // TLE
            else if (result.real_time > max_time) {
                judge_result["case"][i]["score"] = 0;
                judge_result["case"][i]["status"] = hoj::test_case_status::TIME_LIMIT_EXCEEDED;
                final_status = hoj::judge_status::UNACCEPTED;
            }
            // MLE
            else if (result.memory > max_memory) {
                judge_result["case"][i]["score"] = 0;
                judge_result["case"][i]["status"] = hoj::test_case_status::MEMORY_LIMIT_EXCEEDED;
                final_status = hoj::judge_status::UNACCEPTED;
            }
            // RE
            else if (result.exit_code != 0) {
                judge_result["case"][i]["score"] = 0;
                judge_result["case"][i]["status"] = hoj::test_case_status::RUNTIME_ERROR;
                final_status = hoj::judge_status::UNACCEPTED;
            }
            else {
                // Run checker
                constexpr int max_checker_time = 1000;
                constexpr int max_checker_memory = 1024 * 1024 * 1024;

                hoj::sandbox_result checker_result = hoj::run_checker(
                    sandbox_directory,
                    checker_directory,
                    checker_name,
                    working_directory,
                    input_file,
                    answer_file,
                    "",
                    max_checker_time,
                    max_checker_memory
                );

                if (checker_result.signal) {
                    judge_result["case"][i]["detail"] = "Signal " + std::to_string(result.signal);
                    judge_result["case"][i]["score"] = 0;
                    judge_result["case"][i]["status"] = hoj::judge_status::UNKNOWN_ERROR;
                    final_status = hoj::judge_status::UNACCEPTED;
                }
                else {
                    switch (checker_result.exit_code) {
                        case static_cast<int>(hoj::checker_status::ACCEPTED): { // AC
                            judge_result["case"][i]["score"] = score;
                            judge_result["case"][i]["status"] = hoj::test_case_status::ACCEPTED;
                            total_score += score;
                            break;
                        }
                        case static_cast<int>(hoj::checker_status::WRONG_ANSWER): { // WA
                            judge_result["case"][i]["score"] = 0;
                            judge_result["case"][i]["status"] = hoj::test_case_status::WRONG_ANSWER;
                            final_status = hoj::judge_status::UNACCEPTED;
                            break;
                        }
                        case static_cast<int>(hoj::checker_status::PRESENTATION_ERROR): { // PE
                            judge_result["case"][i]["score"] = 0;
                            judge_result["case"][i]["status"] = hoj::test_case_status::PRESENTATION_ERROR;
                            final_status = hoj::judge_status::UNACCEPTED;
                            break;
                        }
                        default: { // UKE
                            judge_result["case"][i]["score"] = 0;
                            judge_result["case"][i]["status"] = hoj::test_case_status::UNKNOWN_ERROR;
                            break;
                        }
                    }
                }
            }
        }

        judge_result["score"] = total_score;
        judge_result["time"] = total_time;
        judge_result["memory"] = total_memory;
        judge_result["status"] = final_status;
        
        save_result(judge_result, result_file);
    }

    return 0;
}

int compile_checker(argparse::ArgumentParser &parser) {
    const fs::path judger_config_path = "./hoj-judger-config.yml";
    const fs::path work_path = parser.get<std::string>("workdir");
    const fs::path source_directory = work_path / "source";
    const fs::path binary_directory = work_path / "binary";
    const fs::path result_path = work_path / "result.yml";
    const fs::path sandbox_directory = parser.get<std::string>("sandbox");
    const std::string type = parser.get<std::string>("type");
    const std::string source_name = parser.get<std::string>("source");

    YAML::Node judger_config = YAML::LoadFile(judger_config_path);
    YAML::Node compile_result;

    std::ofstream result_file(result_path);
    if (!result_file.is_open()) {
        return -1;
    }

    YAML::Node checker_config = judger_config["checker"][type];
    if (!checker_config) {
        std::cerr << "No such checker type in config." << std::endl
            << "Type: " << type << std::endl;
        return -1;
    }
    
    YAML::Node compile_config = checker_config["compile"];

    if (compile_config) {
        constexpr int max_compile_time = 5000;
        constexpr int max_compile_memory = 1024 * 1024 * 1024;

        std::string compile_command = compile_config.as<std::string>();

        hoj::sandbox_result result = hoj::compile_checker(
            sandbox_directory,
            source_directory,
            source_name,
            binary_directory,
            "checker",
            compile_command,
            max_compile_time,
            max_compile_memory
        );

        compile_result["compile_time"] = result.real_time;
        compile_result["compile_memory"] = result.memory;

        // has signal
        if (result.signal) { // UKE
            compile_result["detail"] = "Signal " + std::to_string(result.signal);
            compile_result["status"] = hoj::test_case_status::UNKNOWN_ERROR;
        }
        // CTLE
        else if (result.real_time > max_compile_time) {
            compile_result["detail"] = "Compile time limit exceeded.";
            compile_result["status"] = hoj::judge_status::COMPILE_ERROR;
        }
        // CMLE
        else if (result.memory > max_compile_memory) {
            compile_result["detail"] = "Compile memory limit exceeded.";
            compile_result["status"] = hoj::judge_status::COMPILE_ERROR;
        }
        // CE
        else if (result.exit_code != 0) {
            std::ifstream compile_log(binary_directory / "compile.txt");

            if (!compile_log.is_open()) { // SE
                compile_result["status"] = hoj::judge_status::SYSTEM_ERROR;
                compile_result["detail"] = "Failed to open compile log.";
            }
            
            std::string compile_detail = read_file(compile_log);

            compile_result["detail"] = compile_detail;
            compile_result["status"] = hoj::judge_status::COMPILE_ERROR;
        }
        else {
            compile_result["status"] = hoj::judge_status::ACCEPTED;
        }

        save_result(compile_result, result_file);
    }

    return 0;
}

int main(int argc, char *argv[]) {
    argparse::ArgumentParser parser("hoj-judger", "Judger for hydrogen oj");
    parser.add_argument("-v", "--version", "show version", false);
    parser.add_argument("-S", "--sandbox", "sandbox root path", true);
    parser.add_argument("-W", "--workdir", "working directory", true);
    parser.add_argument("-t", "--type", "type/language", true);
    parser.add_argument("-s", "--source", "source file", true);
    // For judger
    parser.add_argument("-P", "--problem", "problem directory", false);
    parser.add_argument("-C", "--checker", "checker path", false);
    // For compiling checker
    parser.add_argument("-x", "--compilechecker", "compile checker", false);
    
    parser.enable_help();
    
    auto err = parser.parse(argc, const_cast<const char **>(argv));

    if (parser.exists("version")) {
        std::cout << "v" HOJ_VERSION_STR << std::endl;
        return 0;
    }

    if (parser.exists("help")) {
        parser.print_help();
        return 0;
    }

    if (err) {
        std::cerr << err << std::endl;
        return -1;
    }

    if (parser.exists("compilechecker")) {
        return compile_checker(parser);
    }
    else {
        return judge_program(parser);
    }

    return 0;
}
