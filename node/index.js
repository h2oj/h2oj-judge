const fs = require('fs');
const path = require('path');
const yaml = require('yaml');
const childProcess = require('child_process');
const ConfigLoader = require('./config_loader');

const status = {
    WAITING: 0,
    ACCEPTED: 1,
    WRONG_ANSWER: 2,
    TIME_LIMIT_EXCEEDED: 3,
    MEMORY_LIMIT_EXCEEDED: 4,
    RUNTIME_ERROR: 5,
    SYSTEM_ERROR: 6,
    COMPILE_ERROR: 7,
    COMPILE_TIME_LIMIT_EXCEEDED: 8,
    COMPILE_MEMORY_LIMIT_EXCEEDED: 9,
    UNKNOWN_ERROR: -1
};

const getResultMsg = result => {
    for (const key of Object.keys(status)) {
        if (status[key] == result) {
            return key;
        }
    }
};

async function judge(config) {
    const judger_path = "./node/bin";
    const work_path = config.work_path;

    // test config
    const test_config = {
        mode: 'compile',
        spj: false,
        checker: '',
        language: 'cpp',
        exe_file: './exe',
        code_file: './src.cpp',
        input_file: './test.in',
        output_file: './test.out',
        answer_file: './test.ans'
    };
    const result = {
        language: test_config.language,
        mode: 'oi',
        spj: test_config.spj,
        test_point: [],
        total_time: 0,
        total_memory: 0,
        status: status.WAITING,
        message: 'WAITING'
    };

    const conf_test = new ConfigLoader;
    conf_test.load(test_config);
    conf_test.write(path.join(work_path, './test.conf'));

    await childProcess.execFile(path.join(judger_path, 'hoj-judger'), [work_path]);
    const conf_result = new ConfigLoader;
    conf_result.read(path.join(work_path, './result.conf'));
    if (Number(conf_result.get('exit_code')) !== 0 || Number(conf_result.get('result')) !== 1) {
        result.status = status.COMPILE_ERROR;
        result.message = 'COMPILE_ERROR';
        return result;
    }

    const problem_config = yaml.parse(fs.readFileSync(path.join(work_path, 'data/config.yml'), { encoding: 'utf-8' }));
    console.log(problem_config);
    conf_test.set('mode', problem_config.mode);
    test_config.spj = problem_config.spj;
    if (problem_config.spj) {
        // SPJ
    }

    for (let i = 1; i <= problem_config.test_points.length; ++i) {
        const test_point = problem_config.test_points[i - 1];
        conf_test.set('input_file', `./data/${problem_config.name}${i}.in`);
        conf_test.set('answer_file', `./data/${problem_config.name}${i}.out`);
        conf_test.set('time_limit', test_point.time);
        conf_test.set('memory_limit', test_point.memory);
        conf_test.write(path.join(work_path, './test.conf'));

        childProcess.execFileSync(path.join(judger_path, 'hoj-judger'), [work_path]);
        conf_result.read(path.join(work_path, './result.conf'));
        result.test_point.push({
            time: Number(conf_result.get('real_time')),
            memory: Number(conf_result.get('memory')),
            status: Number(conf_result.get('result')),
            message: getResultMsg(Number(conf_result.get('result')))
        });
        result.total_time += Number(conf_result.get('real_time'));
        result.total_memory = Math.max(result.total_memory, Number(conf_result.get('memory')));

        if (Number(conf_result.get('result')) !== status.ACCEPTED) {
            result.status = status.WRONG_ANSWER;
            result.message = 'UNACCEPTED';
        }
    }

    if (result.status == status.WAITING) {
        result.status = status.ACCEPTED;
        result.message = 'ACCEPTED';
    }

    return result;
};

module.exports = {
    JudgeStatus: status,
    judge,
};
