const fs = require('fs');
const path = require('path');
const childProcess = require('child_process');

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
    const test_config_data = Object.keys(test_config).map(key => {
        return `${key}=${test_config[key]}`;
    }).join('\n');
    fs.writeFileSync(path.join(work_path, './test.conf'), test_config_data);
    
    childProcess.execFileSync(path.join(judger_path, 'hoj-judger'), [work_path]);
};
