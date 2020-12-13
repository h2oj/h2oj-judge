const hojJudger = require('./index');

hojJudger.judge({
    work_path: './test/aplusb_problem',
    judger_config: './test/aplusb_problem/judger.conf'
}).then(result => {
    console.log(result);
});

