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

declare enum TestCaseStatus {
    NO_STATUS = 0,
    ACCEPTED = 1,
    COMPILE_ERROR = 2,
    COMPILE_MEMORY_LIMIT_EXCEEDED = 3,
    COMPILE_TIME_LIMIT_EXCEEDED = 4,
    MEMORY_LIMIT_EXCEEDED = 5,
    OUTPUT_LIMIT_EXCEEDED = 6,
    PRESENTATION_ERROR = 7,
    RUNTIME_ERROR = 8,
    SYSTEM_ERROR = 9,
    TIME_LIMIT_EXCEEDED = 10,
    UNKNOWN_ERROR = 11,
    WRONG_ANSWER = 12
}

declare enum JudgeStatus {
    NO_STATUS = 0,
    ACCEPTED = 1,
    COMPILE_ERROR = 2,
    SYSTEM_ERROR = 3,
    UNACCEPTED = 4,
    UNKNOWN_ERROR = 5
}

declare class JudgerConfig {
    code_path: string;
    problem_path: string;
    output_path: string;
    language: string;
}

declare class TestCaseResult {
    time: number;
    space: number;
    score: number;
    status: TestCaseStatus;
}

declare class JudgeResult {
    score: number;
    space: number;
    time: number;
    status: JudgeStatus;
    case: TestCaseResult[];
}

declare function judge(config: JudgerConfig): Promise<JudgeResult>;

declare module 'hoj-judger' {
    export { TestCaseResult, JudgeResult, JudgerConfig, TestCaseStatus, JudgeStatus, judge };
};
