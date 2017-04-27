'use strict';

var multer = require('multer');
var fs = require('fs');

var uploadCppPath = "userUploadFiles/cppFiles";
var upload = multer({ dest: '' + uploadCppPath });

module.exports = function (app) {
    var uploadController = require('../controllers/upload.server.controller');
    //app.get('/', uploadFileController.render);

    app.get('/', uploadController.render);

    app.post('/', upload.any(), function (req, res, next) {

        var userIP = req.hostname;
        console.log(userIP);
        makeUserSpecDir(userIP);
        for (var i = 0; i < req.files.length; i++) {
            // 图片会放在uploads目录并且没有后缀，需要自己转存，用到fs模块
            // 转存，fs.rename(oldPath, newPath,callback);
            //console.log(req.files[i]);
            var newPath = uploadCppPath + '/' + userIP + '/' + req.files[i].originalname;
            //console.log(newPath);
            fs.rename(req.files[i].path, newPath, function (err) {
                if (err) {
                    throw err;
                }
                console.log('rename file done!');
            });
        }
    });
};

var makeUserSpecDir = function makeUserSpecDir(userIP) {
    if (!fs.existsSync(uploadCppPath + '/' + userIP)) {
        fs.mkdir(uploadCppPath + '/' + userIP, function (err) {
            if (err) {
                throw err;
            }
            console.log('mkdir for /' + userIP + ' success!');
        });
    }
};
//
// var initMulter(req) {
//     var instance = multer({ dest: 'cppFiles/' })
//     return instance
// }

//# sourceMappingURL=upload.server.routes-compiled.js.map