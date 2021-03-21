const Judger = require('../../node/index');

Judger.compileChecker({
    sandboxDirectory: 'test/sandbox/sandbox/rootfs',
    workingDirectory: 'test/aplusb_problem/checker_workdir',
    sourceName: 'checker.cpp',
    type: 'testlib'
}).then(compileResult => {
    console.log(compileResult);

    Judger.judge({
        sandboxDirectory: 'test/sandbox/sandbox/rootfs',
        workingDirectory: 'test/aplusb_problem/judge_workdir',
        problemDirectory: 'test/aplusb_problem/data',
        checkerPath: 'test/aplusb_problem/checker_workdir/binary/checker',
        sourceName: 'src.cpp',
        type: 'cpp98'
    }).then(judgeResult => {
        console.log(judgeResult);
    });
});
