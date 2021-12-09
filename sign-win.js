var signtool = require("signtool");
const path = require('path')

let productName = process.env.npm_package_productName

function sign(file) {
    signtool.sign(path.resolve(file),
        {
            certificate: path.resolve('./cert/excellent_fusion_sdn_bhd.pfx'),
            password: "t4WSjuY3kVbxrbGy",
            algorithm: "sha256",
            timestamp: "http://timestamp.digicert.com",
            description: `${productName}`
        }).then(result => {
            if (result.code == 0) {
                console.log(result.stdout) // The signtool stdout content.
            } else {
                console.error(result.stderr) // The signtool stderr content.
            }
        });
}

sign(path.join(__dirname, 'bin/win32/Screenshot.exe'))