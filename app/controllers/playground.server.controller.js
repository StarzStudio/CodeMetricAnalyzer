/**
 * Created by zhouxing on 5/6/17.
 */

const path = require("path");
const fs = require('fs');
const spawn = require('child_process').spawn;

exports.run = function (req, res, next) {

    let cppContent = req.body.cppContent;
    // store content into a file
    let cppFileName = 'temp.cpp';
    fs.writeFileSync(cppFileName, cppContent);
    // run the that temp cpp file
    try {
        runCppInConsole(cppFileName, res);
    } catch (err) {
        console.error(err);
    }



};
// const contentEscaping(cppContent) {
//
// }

const runCppInConsole = function (cppFileName, res) {

    const process = require('child_process');
    let result = {};
    complieCppFile(process, cppFileName, result, res);


};
const complieCppFile = function(process, cppFileName, result, res) {
    process.exec('g++-4.9 -std=c++14 -o tempExe temp.cpp', function (error, stdout, stderr) {
        if (error !== null) {
            console.log('exec error: ' + error);
        }
        console.log('complie stdout: ' + stdout);
        console.log('complie stderr: ' + stderr);
        result.complieStdout = stdout || "";
        result.complieStderr = stderr || "";
        runCppFile(process, result, res, cppFileName);
    });
}
const runCppFile = function(process, result, res, cppFileName) {
    let exeFileName = './tempExe';
    process.execFile(exeFileName,null, function (error,stdout,stderr) {
        if (error !== null) {
            console.log('exec error: ' + error);
        }
        console.log('run stdout: ' + stdout);
        console.log('run stderr: ' + stderr);
        result.runStdout = stdout || "";
        result.runStderr = stderr || "";
        
        res.json(result);
        //delete cpp file
        deleteFile(cppFileName);
        deleteFile(exeFileName);
    });
}
const deleteFile = function (fileName) {
    if (fs.existsSync(fileName))  {
        fs.unlinkSync(fileName);
    }
}

