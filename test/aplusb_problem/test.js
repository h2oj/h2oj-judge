const HojJudger = require('../../node/index');

HojJudger.judge({
    code_path: './test/aplusb_problem/src.cpp',
    problem_path: './test/aplusb_problem/data',
    output_path: './test/aplusb_problem/',
    language: 'cpp98'
}).then(result => {
    console.log(result);
});
