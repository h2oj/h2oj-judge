// For typescript

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
