const HojJudger = require('./index');

HojJudger.judge({
    code_path: './test/aplusb_problem/src.cpp',
    problem_path: './test/aplusb_problem/data',
    output_path: './build/test1/',
    language: 'cpp98'
}).then(result => {
    console.log(result);
});
HojJudger.judge({
    code_path: './test/aplusb_problem/src_ac.cpp',
    problem_path: './test/aplusb_problem/data',
    output_path: './build/test2/',
    language: 'cpp98'
}).then(result => {
    console.log(result);
});
