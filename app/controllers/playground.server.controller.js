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
    let result = runCppInConsole(cppFileName);
    res.json(result);
    //delete cpp file
    fs.unlinkSync(cppFileName);
};

const runCppInConsole = function (cppFileName) {

    cppProgram = spawn(`g++ -std=c++14 ${cppFileName}`);

    let result;
// 捕获标准输出并将其打印到控制台
    cppProgram.stdout.on('data', function (data) {
        console.log('standard output:\n' + data);
        result.stdout = data;
    });

// 捕获标准错误输出并将其打印到控制台
    cppProgram.stderr.on('data', function (data) {
        console.log('standard error output:\n' + data);
        result.stderr = data;
    });

// 注册子进程关闭事件
    cppProgram.on('exit', function (code, signal) {
        console.log('child process eixt ,exit:' + code);
    });
    return result;
};


