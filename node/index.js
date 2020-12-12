const fs = require('fs');
const path = require('path');
const childProcess = require('child_process');
const ConfigLoader = require('./config_loader');

module.exports = function (config) {
    const judger_path = "./node/bin";
    const work_path = config.work_path;

    // test config
    const test_config = {
        mode: 'compile',
        spj: 'false',
        checker: '',
        language: 'cpp',
        exe_file: './exe',
        code_file: './src.cpp',
        input_file: './test.in',
        output_file: './test.out',
        answer_file: './test.ans'
    };

    const conf_test = new ConfigLoader;
    conf_test.load(test_config);
    conf_test.write(path.join(work_path, './test.conf'));

    childProcess.execFileSync(path.join(judger_path, 'hoj-judger'), [work_path]);
};
