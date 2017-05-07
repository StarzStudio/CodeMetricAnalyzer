"use strict";

var uploadFiles = function () {
    console.log("button click");
    var files = document.querySelector("#file_input").files;
    var projectName = document.querySelector('input[name="projectName"]').value;
    var formData = new FormData();
    for (var file of files) {
        var name = file.name;
        if (fileFilter(name)) {
            formData.append(name, file);
        }
    }
        
    if (files.length === 0) {
        console.log("no file input, return")
        return;
    }
    formData.append('projectName', projectName);

    var request = new XMLHttpRequest();
    request.onload = function() {

        //alert("upload seccuess!");
    }



    request.onreadystatechange = function () { // state change, callback
        if (request.readyState === 4) { // state: done
            if (request.status === 200) {

                // success, get the return text
                return success(request.responseText);
            } else {
                // 失败，根据响应码判断失败原因:
                return fail(request.status);
            }
        } else {
            if(confirm('go to project page？')) {
                location.href='#!/projects';
            }
            console.log("http request is still ongoing");
    }
    }

    // make a request
    request.open("POST", "", true);
    request.send(formData);
};

function success(text) {
   console.log(text);
}

function fail(code) {
    console.log(code);
}


function fileFilter(filename){

    if (filename.endsWith('.h') || +
            filename.endsWith('.cc') || +
            filename.endsWith('.cpp') || +
            filename.endsWith('.C') || +
            filename.endsWith('.c') ) {

        return true;
    } else {
        alert(' File: ${filename} is not accepted.\n ' +
            'Can only upload files with ex:\n *.h\n *.cc\n *.cpp\n *.C\n *.c\n');
        return false;
    }
}



