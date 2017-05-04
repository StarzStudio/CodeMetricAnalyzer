

const path = require("path");
const fs = require('fs');
const htmlGenerator = require(__dirname + '/../../build/Release/cppToHTML.node');
const metricAnalyzer = require(__dirname + '/../../build/Release/metricAnalyzer.node');
const templateHTMLPath = "./Service/CppService/CppToHTML/HTMLgenerator/template.htm";
const multer  = require('multer');

const uploadCppPath = "userUploadFiles/cppFiles";
const upload = multer({ dest: __dirname}).any();


exports.render = function(req, res) {

    res.render('upload'); // send .ejs file to the client


};

exports.receiveFiles = function (req, res, next) {
    console.log("hello");
    // multer process in the below block
    upload(req,res, function(err) {
        if (err) {
            console.log("error happened when multer is processing");
            console.log(err);
            res.status(500).end();
            return ;
        }

        var userIP = req.hostname;
        makeUserSpecDir(userIP);               // generate a dir for each user
        var newFilePaths = [];
        for (let i = 0; i < req.files.length; i++) {

            // files will be put into path: uploadCppPath with a uuid name and without suffix
            // need to rename file using fs module
            renameFile(req.files[i], userIP, newFilePaths);
        }

        var destHTMLPath =  uploadCppPath + "/" + userIP;
        var searchPath =  destHTMLPath;

        // call c++ services
        analyzeCodeMetrics( searchPath);
        generateHTML(templateHTMLPath, destHTMLPath, searchPath);



        // delete all the cpp files eventually
        deleteFiles(newFilePaths);

        // send success status code back
        exports.render(req, res);

    })


};

const analyzeCodeMetrics = function (path) {
    metricAnalyzer(path);
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
const renameFile = function (file, userIP, newFilePaths) {
    var newPath =  __dirname + `/../../${uploadCppPath}/${userIP}/`  + file.originalname;
    fs.renameSync(file.path, newPath);
    newFilePaths.push(newPath);
}

const deleteFiles = function(newFilePaths) {
    for (let path of newFilePaths) {
        fs.unlinkSync(path);
    }
    let metricsResultFile = __dirname + `/../../${uploadCppPath}/localhost/`  + 'metricsResult'
    fs.renameSync(metricsResultFile + ".cpp.html" , metricsResultFile + ".html");
    fs.unlinkSync(metricsResultFile + ".cpp");

}

const generateHTML = function(templateHTMLPath, destHTMLPath, searchPath) {

    let isFinished = htmlGenerator(templateHTMLPath, destHTMLPath, searchPath);
    return isFinished;
}

