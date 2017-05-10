"use strict";

var formData = new FormData();


var uploadFiles = function () {

    var projectName = document.querySelector('input[name="projectName"]').value;
    console.log(projectName);
    if (projectName === "") {
        alert("Please specify your project name");
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
                if(confirm('go to project page？')) {
                    location.href='#!/projects';
                }

                // success, get the return text
                return success(request.responseText);
            } else {
                // 失败，根据响应码判断失败原因:
                return fail(request.status);
            }
        } else {
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

$( "button[id=submitButton]" ).click(function() {
    uploadFiles();
});

$('input[id=file_input]').change(function() {
    // $('#photoCover').val($(this).val());
    // $('#photoCover').val("Now, Please click submit button");
    appendFilesToFormData();

});


const appendFilesToFormData = function() {
    var files = document.querySelector("#file_input").files;
    var fileNameTextarea = document.getElementById("fileNameTextarea");
    for (let f of files) {
        if (fileFilter(f.name)){

            // elimate duplicated files
            if (!formData.has(f.name)) {
                formData.append(f.name, f);
                fileNameTextarea.value += f.name;
                fileNameTextarea.value += '\n';
            }

            ResizeTextarea();
        } else {
            // if one of files doesn't meet file suffix, then just break the loop and let alert show just once.
            break;
        }
    }

    // for (let key of formData.keys()) {
    //     console.log(key);
    // }
}



// $(document).ready(function() {
//
//
// });


function ResizeTextarea(){
    let minHeight = 100;
    let maxHeight = 200;
    let t = document.getElementById('fileNameTextarea');
    let h = t.scrollHeight;
    h = h > minHeight ? h : minHeight;
    h = h > maxHeight ? maxHeight : h ;
    t.style.height = h + "px";
}

// $("textarea[id=fileNameTextarea]").scroll(function(){ $(this).css("height",$(this)[0].scrollHeight); });