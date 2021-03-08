# H<sub>2</sub> OJ Judger

The project is a tool written in C++ to judge a program for Hydrogen OJ.

## Install in Node.js Project
Requirement: Yarn
```bash
yarn add hoj-judger
```

## Build by Yourself
Requirement: CMake 3.4 or higher, g++ 9 or higher
```bash
./build.sh
```

## How to Use
### Use in Terminal
```bash
hoj-judger --source=<source-file> --problem=<data-dir> --language=<language> --checker=<checker-path> --output=<output-dir>
```

Here is the description for each argument.
|Argument|Description|Example|
|:--|:--|:--|
|source|Path to source file.|test/aplusb_problem/src.cpp|
|problem|Path to problem directory which contains \*.in/\*.out and config.yml.|test/aplusb_problem/data|
|language|Language identifier defined in the config.|cpp98|
|checker|Path to checker.|node/bin/hoj-checker|
|output|Path to output directory.|test/aplusb_problem|

In the output directory, hoj-judger will create `exe`, `output.out`, `compile.log` and `result.yml` while running. If chosen language doesn't need to be compiled, it will only create `output.out` and `result.yaml`. For details of `result.yaml`, see below.

### Use in Node.js Project
```javascript
const HojJudger = require('../node/index');

HojJudger.judge({
    code_path: './test/aplusb_problem/src.cpp',
    problem_path: './test/aplusb_problem/data',
    output_path: './test/aplusb_problem/',
    language: 'cpp98'
}).then(result => {
    console.log(result);
});
```

Also support TypeScript.

## Config
### Judger Config
Path: /path/to/work/directory/hoj-judger-config.yml
```yaml
language:
  cpp98: # This is an identifier for a language.
    compile: "g++ \"%source\" -o \"%executable\" -DONLINE_JUDGE -DHOJ_JUDGER" # This is a command for COMPILE event.
    run: "\"%executable\"" # This is a command for RUN event.
  nodejs:
    run: "node \"%executable\""
```
Here is the description for each variable.
|Variable|Description|Example|
|:--|:--|:--|
|source|Path to source file.|./test/aplusb_problem/src.cpp|
|executable|Path to executable file.|./test/aplusb_problem/exe|

If any language doesn't need to be compiled, `compile` can be ignored. But `run` is always required.

### Problem Config
Path: /path/to/problem/data/config.yml
```yaml
file: aplusb # Prefix of each input and output file.
mode: oi # It can only be filled with "oi" now.
type: default # It can only be filled with "default" now.
case_count: 2 # Count of test cases.
case:
  - space: 131072 # Maximum space limit (KiB).
    time: 1000 # Maximum time limit (ms).
    score: 50 # Score of the test case.
  - space: 131072
    time: 1000
    score: 50
```
Each data file should be named like `aplusb1.in`, `aplusb1.out`. If `file` is ignored, the data file name should be named like `1.in`, `1.out`.

## Judge Result 
Path: /path/to/output/directory/result.yml
```yaml
score: 10 # Total score.
time: 299 # Total time (ms).
space: 1612 # Maximum used space (KiB).
status: 4 # Judge status.
case_count: 2 # Count of test cases.
case:
  - space: 1612 # Used space (KiB).
    time: 27 # Used time (ms).
    score: 0 # Score.
    status: 12 # Test case status.
  - space: 1604
    time: 26
    score: 50
    status: 1 
```

Here is the description for judge status.
|Status|Abbr.|Value|Description|
|:--|:--|:--|:--|
|NO_STATUS|/|0|Unused status.|
|ACCEPTED|AC|1|Accepted.|
|COMPILE_ERROR|CE|2|Compile error.|
|SYSTEM_ERROR|SE|3|System error.
|UNACCEPTED|WA|4|Unaccepted.|
|UNKNOWN_ERROR|UKE|5|Unknown error.|

Here is the description for test case status.
|Status|Abbr.|Value|Description|
|:--|:--|:--|:--|
|NO_STATUS|/|0|Unused status.|
|ACCEPTED|AC|1|Accepted.|
|COMPILE_ERROR|CE|2|Compile error.|
|COMPILE_MEMORY_LIMIT_EXCEEDED|CMLE|3|Compile memory limit exceeded.|
|COMPILE_TIME_LIMIT_EXCEEDED|CTLE|4|Compile time limit exceeded.|
|MEMORY_LIMIT_EXCEEDED|MLE|5|Memory limit exceeded.|
|OUTPUT_LIMIT_EXCEEDED|OLE|6|Output limit exceeded.|
|PRESENTATION_ERROR|PE|7|Presentation error.|
|RUNTIME_ERROR|RE|8|Runtime error.|
|SYSTEM_ERROR|SE|9|System error.|
|TIME_LIMIT_EXCEEDED|TLE|10|Time limit exceeded.|
|UNKNOWN_ERROR|UKE|11|Unknown error.|
|WRONG_ANSWER|WA|12|Wrong answer.|

