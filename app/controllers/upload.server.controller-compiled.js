'use strict';

var path = require("path");
var fs = require('fs');
var htmlGenerator = require(__dirname + '/../../build/Release/cppToHTML.node');
var metricAnalyzer = require(__dirname + '/../../build/Release/metricAnalyzer.node');
var templateHTMLPath = "./Service/CppService/CppToHTML/HTMLgenerator/template.htm";
var multer = require('multer');

var uploadCppPath = "userUploadFiles/cppFiles";
var upload = multer({ dest: __dirname }).any();

var projectController = require('../controllers/project.server.controller');

//const projectController = require(__dirname + '/project.server.controller.js');

exports.render = function (req, res) {

    res.sendfile('./public/upload/views/upload.html');
};

exports.receiveFiles = function (req, res, next) {
    // multer process in the below block
    upload(req, res, function (err) {
        if (err) {
            console.log("error happened when multer is processing");
            console.log(err);
            res.status(500).end();
            return;
        }

        //var userIP = req.ip || 'anonymous';
        var userIP = req.ip;
        makeUserSpecDir(userIP); // generate a dir for each user
        var cppFilePaths = [];
        var htmlFilePaths = [];
        for (var i = 0; i < req.files.length; i++) {

            // files will be put into path: uploadCppPath with a uuid name and without suffix
            // need to rename file using fs module
            renameFile(req.files[i], userIP, cppFilePaths, htmlFilePaths);
        }

        var destHTMLPath = uploadCppPath + "/" + userIP;
        var searchPath = destHTMLPath;

        // call c++ services

        // store all the files metrics in JSON format in an array
        var fileMetricsJSON = analyzeCodeMetrics(searchPath);
        var fileInfos = [];
        for (var _i = 0; _i < fileMetricsJSON.length; _i++) {
            var fileInfo = {};
            fileInfo.name = req.files[_i].originalname;
            fileInfo.fileURL = htmlFilePaths[_i];
            fileInfo.metrics = fileMetricsJSON[_i];
            fileInfos.push(fileInfo);
        }

        // this step should be after analyzeCodeMetrics(), because it need to wait analyzeCodeMetrics generate
        // metricsResult.cpp, then make it to .html
        generateHTML(templateHTMLPath, destHTMLPath, searchPath);

        // delete all the cpp files eventually
        deleteFiles(cppFilePaths);

        // send success status code back
        // res.status(200).redirect('index/!#/projects');

        req.fileInfos = fileInfos;
        projectController.create(req, res);
    });
};

var analyzeCodeMetrics = function analyzeCodeMetrics(path) {
    var result = metricAnalyzer(path);
    return result;
};
var makeUserSpecDir = function makeUserSpecDir(userIP) {
    var path = __dirname + ('/../../' + uploadCppPath + '/' + userIP);
    console.log('user\'s storage path: ' + path);
    if (!fs.existsSync(path)) {
        console.log("user's storage path didn't exist before");
        fs.mkdir(path, function (err) {
            if (err) {
                console.log("error in mkdir");
                throw err;
                res.status(500).end();
            }
            console.log('mkdir for /' + userIP + ' success!');
        });
    }
};

// fs.rename(oldPath, newPath,callback);
var renameFile = function renameFile(file, userIP, cppFilePaths, htmlFilePaths) {
    var cppPath = __dirname + ('/../../' + uploadCppPath + '/' + userIP + '/') + file.originalname;
    var htmlPath = '/' + userIP + '/' + file.originalname + '.html';
    fs.renameSync(file.path, cppPath);
    cppFilePaths.push(cppPath);
    htmlFilePaths.push(htmlPath);
};

var deleteFiles = function deleteFiles(newFilePaths) {
    var _iteratorNormalCompletion = true;
    var _didIteratorError = false;
    var _iteratorError = undefined;

    try {
        for (var _iterator = newFilePaths[Symbol.iterator](), _step; !(_iteratorNormalCompletion = (_step = _iterator.next()).done); _iteratorNormalCompletion = true) {
            var _path = _step.value;

            fs.unlinkSync(_path);
        }
        // let metricsResultFile = __dirname + `/../../${uploadCppPath}/localhost/`  + 'metricsResult'
        // fs.renameSync(metricsResultFile + ".cpp.html" , metricsResultFile + ".html");
        // fs.unlinkSync(metricsResultFile + ".cpp");
    } catch (err) {
        _didIteratorError = true;
        _iteratorError = err;
    } finally {
        try {
            if (!_iteratorNormalCompletion && _iterator.return) {
                _iterator.return();
            }
        } finally {
            if (_didIteratorError) {
                throw _iteratorError;
            }
        }
    }
};

var generateHTML = function generateHTML(templateHTMLPath, destHTMLPath, searchPath) {

    var isFinished = htmlGenerator(templateHTMLPath, destHTMLPath, searchPath);
    return isFinished;
};

//# sourceMappingURL=upload.server.controller-compiled.js.map