const fs = require('fs');
const path = require('path');
const yaml = require('yaml');
const childProcess = require('child_process');

const TestCaseStatus = {
    NO_STATUS: 0,
    ACCEPTED: 1,
    COMPILE_ERROR: 2,
    COMPILE_MEMORY_LIMIT_EXCEEDED: 3,
    COMPILE_TIME_LIMIT_EXCEEDED: 4,
    MEMORY_LIMIT_EXCEEDED: 5,
    OUTPUT_LIMIT_EXCEEDED: 6,
    PRESENTATION_ERROR: 7,
    RUNTIME_ERROR: 8,
    SYSTEM_ERROR: 9,
    TIME_LIMIT_EXCEEDED: 10,
    UNKNOWN_ERROR: 11,
    WRONG_ANSWER: 12
};

const JudgeStatus = {
    NO_STATUS: 0,
    ACCEPTED: 1,
    COMPILE_ERROR: 2,
    SYSTEM_ERROR: 3,
    UNACCEPTED: 4,
    UNKNOWN_ERROR: 5
};

async function judge(config) {
    const { code_path, problem_path, output_path, language } = config;
    const judger_path = path.join(__dirname, 'bin/hoj-judger');
    const checker_path = path.join(__dirname, 'bin/hoj-checker');
    childProcess.execFileSync(judger_path, [
        '--source', code_path,
        '--problem', problem_path,
        '--language', language,
        '--checker', checker_path,
        '--output', output_path
    ]);

    const result = yaml.parse(fs.readFileSync(path.join(output_path, 'result.yml'), { encoding: 'utf-8' }));
    return result;
}

module.exports = {
    TestCaseStatus,
    JudgeStatus,
    judge,
};
