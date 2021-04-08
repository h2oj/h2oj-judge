/*
    This file is part of the h2oj-judge.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/* Copyright (C) 2020-2021 Alex Cui */

const WebSocket = require('ws');
const yaml = require('yaml');
const fs = require('fs');
const path = require('path');
const AdmZip = require('adm-zip');
const axios = require('axios').default;
const Logger = require('./log');
const Judge = require('./index');

const languageFileExension = {
    'cpp98': 'cpp', 'cpp03': 'cpp', 'cpp11': 'cpp', 'cpp14': 'cpp', 'cpp17': 'cpp', 'cpp20': 'cpp',
    'c99': 'c', 'c11': 'c', 'c17': 'c',
    'nodejs14': 'js',
    'scratch3': 'sb3', 'clipcc3': 'ccproj'
};

class JudgeClient {
    constructor() {
        this.ws = null;
        this.token = '';
        this.logger = new Logger();
        this.taskId = 0;

        const configPath = 'h2oj-judge.yml';
        this.config = yaml.parse(fs.readFileSync(configPath, { encoding: 'utf-8' }));
        
        this.createDirectoryWithCheck(path.join(this.config.cache_dir), true);
        this.createDirectoryWithCheck(path.join(this.config.cache_dir, 'problem'));
        this.createDirectoryWithCheck(path.join(this.config.cache_dir, 'judge'));
        this.createDirectoryWithCheck(path.join(this.config.cache_dir, 'checker'));

        this.axios = axios.create({
            baseURL: this.config.server_url,
            timeout: 15000,
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json'
            }
        });
    }

    sendJSON(data) {
        this.logger.log('Data sended: ', JSON.stringify(data));
        return this.ws.send(JSON.stringify(data));
    }

    resetAxios() {
        this.axios = axios.create({
            baseURL: this.config.server_url,
            timeout: 15000,
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json',
                'Authorization': this.token,
            },
        });
    }

    getTime() {
        return Math.floor(Number(new Date()) / 1000);
    }

    createDirectoryWithCheck(path, recursive = false) {
        if (!fs.existsSync(path)) {
            fs.mkdirSync(path, { recursive: recursive });
        }
    }

    async connectServer() {
        const res = await this.axios.post('judge/verify', {
            token: this.config.client_token
        });
        this.logger.log('Token: ' + res.data.data.token);
        this.token = res.data.data.token;
        this.resetAxios();
    }

    async startWebSocket() {
        const wsURL = `${this.config.server_url.replace(/^http/i, 'ws')}judge/connect`;
        console.log(wsURL);
        this.ws = new WebSocket(wsURL);
        this.ws.on('open', () => {
            this.logger.log('Websocket opened.');
            this.sendJSON({
                token: this.config.client_token
            });
            setInterval(() => {
                this.sendJSON({
                    event: 'ping',
                    time: this.getTime()
                });
            }, 15000);
        });
        this.ws.on('message', msg => {
            this.logger.log('Data Received: ', msg.toString());
            const data = JSON.parse(msg);
            if (data.event === 'judge') {
                this.judge(data.data, status => {
                    this.sendJSON({
                        event: 'status',
                        data: status
                    });
                }, result => {
                    this.sendJSON({
                        event: 'end',
                        data: result
                    });
                });
            }
        });
        this.ws.on('close', code => {
            this.logger.log('Websocket disconneted: ', code);
        });
        this.ws.on('error', error => {
            this.logger.err('Websocket error: ', error);
        });
        this.logger.log('Connected.');
    }

    setupWorkingDirectory(dir_path, wk = true) {
        this.createDirectoryWithCheck(dir_path);
        if (wk) this.createDirectoryWithCheck(path.join(dir_path, 'working'));
        this.createDirectoryWithCheck(path.join(dir_path, 'binary'));
        this.createDirectoryWithCheck(path.join(dir_path, 'source'));
    }

    async judge(data, callback, finish) {
        const submissionId = data.submission_id.toString();
        const problemId = data.problem_id.toString();
        this.setupWorkingDirectory(path.join(this.config.cache_dir, 'judge', submissionId));
        await this.checkDataCache(problemId);
        const fileName = await this.fetchSource(submissionId);
        const checkerId = await this.checkChecker(problemId);

        Judge.judge({
            sandboxDirectory: this.config.sandbox_dir,
            workingDirectory: path.join(this.config.cache_dir, 'judge', submissionId),
            problemDirectory: path.join(this.config.cache_dir, 'problem', problemId),
            checkerPath: path.join(this.config.cache_dir, 'checker', checkerId, 'binary/checker'),
            sourceName: fileName,
            type: data.language
        }, data => {
            this.logger.log('Judge testcase [', data.id, ']:\n', JSON.stringify(data));
            callback(data);
        }).then(result => {
            finish(result);
        });
    }

    async fetchData(problemId) {
        this.logger.log('Fetching data: ', problemId);
        const filePath = path.join(this.config.cache_dir, 'problem', problemId);
        this.createDirectoryWithCheck(filePath);
        const file = fs.createWriteStream(path.join(filePath, 'data.zip'));
        const res = await this.axios.get('judge/get_data', {
            responseType: 'stream',
            params: {
                problem_id: problemId
            }
        });
        res.data.pipe(file);
        file.on('finish', () => {
            let zip = new AdmZip(path.join(filePath, 'data.zip'));
            zip.extractAllTo(filePath, true);
        });
    }

    async fetchSource(submissionId) {
        this.logger.log('Fetching src: ', submissionId);
        const filePath = path.join(this.config.cache_dir, 'judge', submissionId, 'source');

        const res = await this.axios.get('judge/get_source', {
            responseType: 'stream',
            params: {
                submission_id: submissionId
            }
        });
        let fileName = decodeURI(RegExp("filename=\"([^;]+\\.[^\\.;]+)\";*").exec(res.headers['content-disposition'])[1]);
        console.log(fileName);
        const file = fs.createWriteStream(path.join(filePath, fileName));
        res.data.pipe(file);
        return fileName;
    }

    readIntFromFile(fileName) {
        try {
            return Number(fs.readFileSync(fileName, { encoding: 'utf-8' }));
        }
        catch (err) {
            return 0;
        }
    }

    writeIntToFile(fileName, value) {
        fs.writeFileSync(fileName, value.toString(), { encoding: 'utf-8' });
    }

    async checkDataCache(problemId) {
        const problemCache = path.join(this.config.cache_dir, 'problem', problemId);
        let lastSync = 0;
        const lastUpdate = (await this.axios.get('judge/check_data', {
            params: { problem_id: problemId }
        })).data.data.last_update;
        if (!fs.existsSync(problemCache)) {
            fs.mkdirSync(problemCache);
        }
        else {
            lastSync = this.readIntFromFile(path.join(problemCache, 'last_sync'));
        }
        this.logger.log('Data update time: ', lastUpdate, '/', lastSync);
        if (lastUpdate > lastSync) {
            await this.fetchData(problemId);
            this.writeIntToFile(path.join(problemCache, 'last_sync'), lastUpdate);
        }
    }

    async checkChecker(problemId) {
        const problemCache = path.join(this.config.cache_dir, 'problem', problemId);
        const problemConfig = yaml.parse(fs.readFileSync(path.join(problemCache, 'config.yml'), { encoding: 'utf-8' }));
        let checkerId = problemId;
        if (problemConfig.mode != 'spj') {
            checkerId = problemConfig.mode;
        }

        const checkerDir = path.join(this.config.cache_dir, 'checker', checkerId);
        let lastCompile = 0;
        const lastSync = this.readIntFromFile(path.join(problemCache, 'last_sync'));
        if (!fs.existsSync(checkerDir)) {
            fs.mkdirSync(checkerDir);
        }
        else {
            lastCompile = this.readIntFromFile(path.join(checkerDir, 'last_compile'));
        }
        this.logger.log('Checker update time: ', lastCompile, '/', lastSync);
        if (lastSync > lastCompile) {
            if (problemId === checkerId) {
                await this.compileChecher(checkerId, false);
            }
            else {
                await this.compileChecher(checkerId, true);
            }
            this.writeIntToFile(path.join(checkerDir, 'last_compile'), lastSync);
        }
        return checkerId;
    }

    async compileChecher(checkerId, builtin) {
        let result;
        this.setupWorkingDirectory(path.join(this.config.cache_dir, 'checker', checkerId), false);
        const srcPath = path.join(this.config.cache_dir, 'checker', checkerId, 'source', 'checker.cpp');
        if (builtin) {
            fs.copyFileSync(path.join(this.config.builtin_checker, checkerId + '.cpp'), srcPath);
        }
        else {
            fs.copyFileSync(path.join(this.config.cache_dir, 'problem', checkerId, 'checker.cpp'), srcPath);
        }
        result = await Judge.compileChecker({
            sandboxDirectory: this.config.sandbox_dir,
            workingDirectory: path.join(this.config.cache_dir, 'checker', checkerId),
            sourceName: 'checker.cpp',
            type: 'testlib'
        });
        console.log(result);
        return result;
    }
}

let client = new JudgeClient();
client.connectServer().then(() => {
    client.startWebSocket();
});
