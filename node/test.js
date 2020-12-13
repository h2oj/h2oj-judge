const hojJudger = require('./index');

hojJudger.judge({
    work_path: './test/aplusb_problem'
}).then(result => {
    console.log(result);
});

