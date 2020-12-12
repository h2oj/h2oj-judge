const fs = require('fs');

module.exports = class ConfigLoader {
    constructor() {
        this.conf = {};
    }

    read(path) {
        const data = fs.readFileSync(path, { encoding: 'utf-8' }).split('\n');
        for (const line of data) {
            if (line.includes('=')) {
                const index = line.indexOf('=');
                this.conf[line.substr(0, index)] = line.substr(index + 1);
            }
        }
    }

    write(path) {
        const data = Object.keys(this.conf).map(key => {
            return `${key}=${this.conf[key]}`;
        }).join('\n');
        fs.writeFileSync(path, data);
    }

    load(data) {
        this.conf = Object.assign({}, this.conf, data);
    }

    get(key) {
        return this.conf[key];
    }

    set(key, value) {
        this.conf[key] = value;
    }
};

