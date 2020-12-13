// For typescript

declare enum JudgeStatus {
    WAITING = 0,
    ACCEPTED = 1,
    WRONG_ANSWER = 2,
    TIME_LIMIT_EXCEEDED = 3,
    MEMORY_LIMIT_EXCEEDED = 4,
    RUNTIME_ERROR = 5,
    SYSTEM_ERROR = 6,
    COMPILE_ERROR = 7,
    COMPILE_TIME_LIMIT_EXCEEDED = 8,
    COMPILE_MEMORY_LIMIT_EXCEEDED = 9,
    UNKNOWN_ERROR = -1
};

declare class Config {
    work_path: string
};

declare class TestPointResult {
    time: number;
    memory: number;
    status: JudgeStatus;
    message: string;
};

declare class JudgeResult {
    language: string;
    mode: string;
    spj: boolean;
    test_point: TestPointResult[];
    total_time: number;
    total_memory: number;
    status: JudgeStatus;
    message: string;
};

declare async function judge(config: Config): JudgeResult;

declare module 'hoj-judger' {
    export { Config, TestPointResult, JudgeResult, JudgeStatus, judge };
};
