const { execFile } = require('child_process');
const path = require('path');

const isElectron = 'electron' in process.versions;
let isUsingAsar = isElectron && process.mainModule && process.mainModule.filename.includes('app.asar');
let fixPathForAsarUnpack = path => isUsingAsar ? path.replace('app.asar', 'app.asar.unpacked') : path;

let captrue = (callback) => {
    let exec
    if (process.platform == 'win32')
        exec = path.join(__dirname, `/bin/win32/Screenshot.exe`)
    else if (process.platform == 'darwin')
        exec = path.join(__dirname, `/bin/darwin/Screenshot.app`)
    else {
        console.warn('platform not support')
        return;
    }
    let proc = execFile(fixPathForAsarUnpack(exec));
    proc.on('exit', function (code) {
        // 执行成功返回 1，返回 0 没有截图
        if (callback) {
            callback(code)
        }
    })
}
module.exports = captrue;