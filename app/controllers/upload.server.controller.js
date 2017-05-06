

const path = require("path");
const fs = require('fs');
const htmlGenerator = require(__dirname + '/../../build/Release/cppToHTML.node');
const metricAnalyzer = require(__dirname + '/../../build/Release/metricAnalyzer.node');
const templateHTMLPath = "./Service/CppService/CppToHTML/HTMLgenerator/template.htm";
const multer  = require('multer');

const uploadCppPath = "userUploadFiles/cppFiles";
const upload = multer({ dest: __dirname}).any();

var projectController = require('../controllers/project.server.controller');

//const projectController = require(__dirname + '/project.server.controller.js');

exports.render = function(req, res) {

    res.sendfile('./public/upload/views/upload.html');


};

exports.receiveFiles = function (req, res, next) {
    // multer process in the below block
    upload(req,res, function(err) {
        if (err) {
            console.log("error happened when multer is processing");
            console.log(err);
            res.status(500).end();
            return ;
        }

        //var userIP = req.ip || 'anonymous';
        var userIP = req.hostname;
        makeUserSpecDir(userIP);               // generate a dir for each user
        var cppFilePaths = [];
        var htmlFilePaths = [];
        for (let i = 0; i < req.files.length; i++) {

            // files will be put into path: uploadCppPath with a uuid name and without suffix
            // need to rename file using fs module
            renameFile(req.files[i], userIP, cppFilePaths, htmlFilePaths);
        }

        var destHTMLPath =  uploadCppPath + "/" + userIP;
        var searchPath =  destHTMLPath;

        // call c++ services

        // store all the files metrics in JSON format in an array
        var fileMetricsJSON = analyzeCodeMetrics( searchPath);
        let fileInfos = [];
        for (let i = 0; i < fileMetricsJSON.length; i++) {
            let fileInfo = {};
            fileInfo.name = req.files[i].originalname;
            fileInfo.fileURL = htmlFilePaths[i];
            fileInfo.metrics = fileMetricsJSON[i];
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

    })


};

const analyzeCodeMetrics = function (path) {
    let result = metricAnalyzer(path);
    return result;
}
const makeUserSpecDir = function(userIP) {
    let path =__dirname + `/../../${uploadCppPath}/${userIP}`;
    console.log(`user's storage path: ${path}`);
    if (!fs.existsSync(path)) {
        console.log("user's storage path didn't exist before");
        fs.mkdir( path, function(err) {
            if (err) {
                console.log("error in mkdir")
                throw err;
                res.status(500).end();
            }
            console.log(`mkdir for /${userIP} success!`);
        })
    }
}

// fs.rename(oldPath, newPath,callback);
const renameFile = function (file, userIP, cppFilePaths, htmlFilePaths) {
    let cppPath =  __dirname + `/../../${uploadCppPath}/${userIP}/`  + file.originalname;
    let htmlPath =  __dirname + `/../../${uploadCppPath}/${userIP}/`  + file.originalname + '.html';
    fs.renameSync(file.path, cppPath);
    cppFilePaths.push(cppPath);
    htmlFilePaths.push(htmlPath);
}

const deleteFiles = function(newFilePaths) {
    for (let path of newFilePaths) {
        fs.unlinkSync(path);
    }
    // let metricsResultFile = __dirname + `/../../${uploadCppPath}/localhost/`  + 'metricsResult'
    // fs.renameSync(metricsResultFile + ".cpp.html" , metricsResultFile + ".html");
    // fs.unlinkSync(metricsResultFile + ".cpp");

}

const generateHTML = function(templateHTMLPath, destHTMLPath, searchPath) {

    let isFinished = htmlGenerator(templateHTMLPath, destHTMLPath, searchPath);
    return isFinished;
}

