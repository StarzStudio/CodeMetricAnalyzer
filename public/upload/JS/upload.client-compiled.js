"use strict";

var formData = new FormData();

var uploadFiles = function uploadFiles() {

    var projectName = document.querySelector('input[name="projectName"]').value;
    console.log(projectName);
    if (projectName === "") {
        alert("Please specify your project name");
        return;
    }
    formData.append('projectName', projectName);
    var request = new XMLHttpRequest();
    request.onload = function () {

        //alert("upload seccuess!");
    };

    request.onreadystatechange = function () {
        // state change, callback
        if (request.readyState === 4) {
            // state: done
            if (request.status === 200) {
                if (confirm('go to project page？')) {
                    location.href = '#!/projects';
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
    };

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

function fileFilter(filename) {

    if (filename.endsWith('.h') || +filename.endsWith('.cc') || +filename.endsWith('.cpp') || +filename.endsWith('.C') || +filename.endsWith('.c')) {

        return true;
    } else {
        alert(' File: ${filename} is not accepted.\n ' + 'Can only upload files with ex:\n *.h\n *.cc\n *.cpp\n *.C\n *.c\n');
        return false;
    }
}

$("button[id=submitButton]").click(function () {
    uploadFiles();
});

$('input[id=file_input]').change(function () {
    // $('#photoCover').val($(this).val());
    // $('#photoCover').val("Now, Please click submit button");
    appendFilesToFormData();
});

var appendFilesToFormData = function appendFilesToFormData() {
    var files = document.querySelector("#file_input").files;
    var fileNameTextarea = document.getElementById("fileNameTextarea");
    var _iteratorNormalCompletion = true;
    var _didIteratorError = false;
    var _iteratorError = undefined;

    try {
        for (var _iterator = files[Symbol.iterator](), _step; !(_iteratorNormalCompletion = (_step = _iterator.next()).done); _iteratorNormalCompletion = true) {
            var f = _step.value;

            if (fileFilter(f.name)) {

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

// $(document).ready(function() {
//
//
// });


function ResizeTextarea() {
    var minHeight = 100;
    var maxHeight = 200;
    var t = document.getElementById('fileNameTextarea');
    var h = t.scrollHeight;
    h = h > minHeight ? h : minHeight;
    h = h > maxHeight ? maxHeight : h;
    t.style.height = h + "px";
}

// $("textarea[id=fileNameTextarea]").scroll(function(){ $(this).css("height",$(this)[0].scrollHeight); });

//# sourceMappingURL=upload.client-compiled.js.map