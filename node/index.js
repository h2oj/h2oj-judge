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
    const judger_path = path.join(__dirname, 'bin/hoj-judger');
    const work_path = config.work_path || '.';
    const path_judger_config = config.judger_config || './judger.conf';
    const data_path = config.data_path;

    // test config
    const test_config = {
        mode: 'compile',
        spj: false,
        checker: '',
        language: 'cpp',
        exe_file: path.join(work_path, 'exe'),
        code_file: path.join(work_path, 'src.cpp'),
        input_file: 'test.in',
        output_file: 'test.out',
        answer_file: 'test.ans'
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

    const path_test_config = path.join(work_path, 'test.conf');
    const path_result = path.join(work_path, 'result.conf');

    const conf_test = new ConfigLoader;
    conf_test.load(test_config);
    conf_test.write(path_test_config);

    childProcess.execFileSync(judger_path, [path_judger_config, path_test_config, path_result]);
    const conf_result = new ConfigLoader;
    conf_result.read(path_result);
    if (Number(conf_result.get('exit_code')) !== 0 || Number(conf_result.get('result')) !== 1) {
        result.status = status.COMPILE_ERROR;
        result.message = 'COMPILE_ERROR';
        return result;
    }

    const problem_config = yaml.parse(fs.readFileSync(path.join(data_path, 'config.yml'), { encoding: 'utf-8' }));
    conf_test.set('mode', problem_config.mode);
    test_config.spj = problem_config.spj;
    if (problem_config.spj) {
        // SPJ
    }

    for (let i = 1; i <= problem_config.test_points.length; ++i) {
        const test_point = problem_config.test_points[i - 1];
        conf_test.set('input_file', path.join(data_path, `${problem_config.name}${i}.in`));
        conf_test.set('answer_file', path.join(data_path, `${problem_config.name}${i}.out`));
        conf_test.set('output_file', path.join(work_path, `${problem_config.name}.out`));
        conf_test.set('time_limit', test_point.time);
        conf_test.set('memory_limit', test_point.memory);
        conf_test.write(path_test_config);

        childProcess.execFileSync(judger_path, [path_judger_config, path_test_config, path_result]);
        conf_result.read(path_result);
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
