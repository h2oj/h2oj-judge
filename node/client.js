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
const axios = require('axios').default;
const Logger = require('./log');
const Judge = require('./index');

class JudgeClient {
    constructor() {
        this.ws = null;
        this.token = '';
        this.logger = new Logger();
        this.taskId = 0;

        const configPath = 'h2oj-judge.yml';
        this.config = yaml.parse(fs.readFileSync(configPath, { encoding: 'utf-8' }));

        this.axios = axios.create({
            baseURL: this.config.server_url,
            timeout: 15000,
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json'
            }
        });
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
            this.logger.log('on open');
            this.ws.send(JSON.stringify({
                token: this.config.client_token
            }));
        });
        this.ws.on('message', msg => {
            this.logger.log('Data Received: ', msg.toString());
            const data = JSON.stringify(msg);
            if (data.event === 'judge') {
                this.judge(data);
            }
        });
        this.ws.on('close', code => {
            this.logger.log('Websocket disconneted: ', code);
        });
        this.ws.on('error', error => {
            this.logger.err('Websocket error: ', error);
        });
        /*await new Promise((resolve) => {
            this.ws.once('open', async () => {
                if (!this.config.noStatus) {
                    const info = await sysinfo.get();
                    this.ws.send(JSON.stringify({ key: 'status', info }));
                    setInterval(async () => {
                        const [mid, inf] = await sysinfo.update();
                        this.ws.send(JSON.stringify({ key: 'status', info: { mid, ...inf } }));
                    }, 1200000);
                }
                resolve(null);
            });
        });*/
        this.logger.log('Connected.');
    }

    async judge(data) {

    }

    async fetchData(problemId) {
        this.axios.get('judge/get_data');
    }

    async checkDataCache(problemId) {
        const cacheDir = path.join(this.config.cache_dir, problemId);
        if (!fs.existsSync(cacheDir)) {
            fs.mkdirSync(cacheDir);
            fs.writeFileSync(path.join(cacheDir, 'last_sync'), '0');
        }
        else {
            const lastSync = Number(fs.readFileSync(path.join(cacheDir, 'last_sync'), { encoding: 'utf-8' }));
            const lastUpdate = (await this.axios.get('judge/check_data', {
                params: { problem_id: problemId }
            })).data.data.last_update;
            if (lastUpdate > lastSync) {
                await this.fetchData(problemId);
            }
        }
    }
}

let client = new JudgeClient();
client.connectServer().then(() => {
    client.startWebSocket();
});
