

const path = require("path");
const fs = require('fs');
const htmlGenerator = require('../../build/Release/cppToHTML.node')
const templateHTMLPath = "./Service/CppService/CppToHTML/HTMLgenerator/template.htm";
const multer  = require('multer');

const uploadCppPath = "userUploadFiles/cppFiles";
const upload = multer({ dest: `${uploadCppPath}`}).any();


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
            return ;
        }
    })

    var userIP = req.hostname;
    console.log(userIP);
    makeUserSpecDir(userIP);
    for (let i = 0; i < req.files.length; i++) {
        // files will be put into path: uploadCppPath with a uuid name and without extension
        // need to rename file using fs module
        renameFile(req.files[i], userIP);
    }

    var destHTMLPath = "./" + uploadCppPath + "/" + userIP;
    var searchPath = "./" + destHTMLPath;
    generateHTML(templateHTMLPath, destHTMLPath, searchPath);

    // send success status code back
    res.send(200);
};


const makeUserSpecDir = function(userIP) {
    if (!fs.existsSync(`${uploadCppPath}/${userIP}`)) {
        fs.mkdir(`${uploadCppPath}/${userIP}`, function(err) {
            if (err) {
                console.log("error in mkdir")
                throw err;
            }
            console.log(`mkdir for /${userIP} success!`);
        })
    }
}

// fs.rename(oldPath, newPath,callback);
const renameFile = function (file, userIP) {
    var newPath = `${uploadCppPath}/${userIP}/`  + file.originalname;

    fs.rename(file.path, newPath, function(err) {
        if (err) {
            throw err;
        }
        console.log(`rename file done!`);
    })
}

const generateHTML = function(templateHTMLPath, destHTMLPath, searchPath) {

    var isFinished = htmlGenerator(templateHTMLPath, destHTMLPath, searchPath);
    console.log(isFinished);
}

function updateSession(req) {
    if (req.session.lastVisit) {
        console.log(req.session.lastVisit);
    }
    req.session.lastVisit = new Date();
}