

const path = require("path");
const fs = require('fs');
const htmlGenerator = require(__dirname + '/../../build/Release/cppToHTML.node');
const metricAnalyzer = require(__dirname + '/../../build/Release/metricAnalyzer.node');
const templateHTMLPath = "./Service/CppService/CppToHTML/HTMLgenerator/template.htm";
const multer  = require('multer');

const uploadCppPath = "userUploadFiles/cppFiles";
const upload = multer({ dest: __dirname}).any();


exports.render = function(req, res) {


    updateSession(req);

    var indexFilePath = path.resolve('public/views/upload.html');
    // console.log(indexFilePath);
    res.sendFile(indexFilePath);

};

exports.receiveFiles = function (req, res, next) {
    upload(req,res, function(err) {
        if (err) {
            console.log("error happened when multer is processing");
            console.log(err);
            return ;
        }

        var userIP = req.hostname;
        makeUserSpecDir(userIP);
        var newFilePaths = [];
        for (let i = 0; i < req.files.length; i++) {

            // files will be put into path: uploadCppPath with a uuid name and without extension
            // need to rename file using fs module
            renameFile(req.files[i], userIP, newFilePaths);
        }


        var destHTMLPath =  uploadCppPath + "/" + userIP;
        var searchPath =  destHTMLPath;
        let result  = generateHTML(templateHTMLPath, destHTMLPath, searchPath);
        if (result) {
            analyzeCodeMetrics( ".");
        }

        // delete all the cpp files in the end
        deleteFiles(newFilePaths);

        // send success status code back
        res.status(200).end();

    })


};

const analyzeCodeMetrics = function (path) {
    var isFinished = metricAnalyzer(path);
    console.log("analyze code: ${isFinished}");
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
            }
            console.log(`mkdir for /${userIP} success!`);
        })
    }
}

// fs.rename(oldPath, newPath,callback);
const renameFile = function (file, userIP, newFilePaths) {
    var newPath =  __dirname + `/../../${uploadCppPath}/${userIP}/`  + file.originalname;
    fs.renameSync(file.path, newPath);
    newFilePaths.push(newPath);
}

const deleteFiles = function(newFilePaths) {
    for (let path of newFilePaths) {
        fs.unlinkSync(path);
    }

}

const generateHTML = function(templateHTMLPath, destHTMLPath, searchPath) {

    let isFinished = htmlGenerator(templateHTMLPath, destHTMLPath, searchPath);
    return isFinished;
}

function updateSession(req) {
    if (req.session.lastVisit) {
        console.log(req.session.lastVisit);
    }
    req.session.lastVisit = new Date();
}