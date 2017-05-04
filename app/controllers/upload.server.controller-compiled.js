'use strict';

var path = require("path");
var fs = require('fs');
var htmlGenerator = require(__dirname + '/../../build/Release/cppToHTML.node');
var metricAnalyzer = require(__dirname + '/../../build/Release/metricAnalyzer.node');
var templateHTMLPath = "./Service/CppService/CppToHTML/HTMLgenerator/template.htm";
var multer = require('multer');

var uploadCppPath = "userUploadFiles/cppFiles";
var upload = multer({ dest: __dirname }).any();

exports.render = function (req, res) {

    res.render('upload'); // send .ejs file to the client

};

exports.receiveFiles = function (req, res, next) {
    console.log("hello");
    // multer process in the below block
    upload(req, res, function (err) {
        if (err) {
            console.log("error happened when multer is processing");
            console.log(err);
            res.status(500).end();
            return;
        }

        var userIP = req.hostname;
        makeUserSpecDir(userIP); // generate a dir for each user
        var newFilePaths = [];
        for (var i = 0; i < req.files.length; i++) {

            // files will be put into path: uploadCppPath with a uuid name and without suffix
            // need to rename file using fs module
            renameFile(req.files[i], userIP, newFilePaths);
        }

        var destHTMLPath = uploadCppPath + "/" + userIP;
        var searchPath = destHTMLPath;

        // call c++ services
        analyzeCodeMetrics(searchPath);
        generateHTML(templateHTMLPath, destHTMLPath, searchPath);

        // delete all the cpp files eventually
        deleteFiles(newFilePaths);

        // send success status code back
        exports.render(req, res);
    });
};

var analyzeCodeMetrics = function analyzeCodeMetrics(path) {
    metricAnalyzer(path);
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
var renameFile = function renameFile(file, userIP, newFilePaths) {
    var newPath = __dirname + ('/../../' + uploadCppPath + '/' + userIP + '/') + file.originalname;
    fs.renameSync(file.path, newPath);
    newFilePaths.push(newPath);
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

    var metricsResultFile = __dirname + ('/../../' + uploadCppPath + '/localhost/') + 'metricsResult';
    fs.renameSync(metricsResultFile + ".cpp.html", metricsResultFile + ".html");
    fs.unlinkSync(metricsResultFile + ".cpp");
};

var generateHTML = function generateHTML(templateHTMLPath, destHTMLPath, searchPath) {

    var isFinished = htmlGenerator(templateHTMLPath, destHTMLPath, searchPath);
    return isFinished;
};

//# sourceMappingURL=upload.server.controller-compiled.js.map