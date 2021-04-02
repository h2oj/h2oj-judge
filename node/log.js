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

const fs = require('fs');

class Logger {
    constructor() {
        const fileName = `log/client_${this.getTime()}.log`;
        this.fd = fs.openSync(fileName, 'w');
    }

    getTime() {
        return Number(new Date()).toString().replace(/(\d*)(\d{3})/, '$1.$2');
    }

    write(text) {
        console.log(text);
        return fs.writeSync(this.fd, text + '\n');
    }

    log(...params) {
        this.write(`${this.getTime()} [log] ${params.join('')}`)
    }

    warn(...params) {
        this.write(`${this.getTime()} [warning] ${params.join('')}`)
    }

    err(...params) {
        this.write(`${this.getTime()} [error] ${params.join('')}`)
    }
}

module.exports = Logger;
