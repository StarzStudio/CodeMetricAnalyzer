"use strict";

var uploadFiles = function uploadFiles() {
    console.log("button click");
    var files = document.querySelector("#file_input").files;
    var formData = new FormData();
    var _iteratorNormalCompletion = true;
    var _didIteratorError = false;
    var _iteratorError = undefined;

    try {
        for (var _iterator = files[Symbol.iterator](), _step; !(_iteratorNormalCompletion = (_step = _iterator.next()).done); _iteratorNormalCompletion = true) {
            var file = _step.value;

            var name = file.name;
            if (fileFilter(name)) {
                formData.append(name, file);
            }
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

    if (files.length === 0) {
        console.log("no file input, return");
        return;
    }

    var request = new XMLHttpRequest();
    request.onload = function () {

        //alert("upload seccuess!");
    };

    request.onreadystatechange = function () {
        // state change, callback
        if (request.readyState === 4) {
            // state: done
            if (request.status === 200) {
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

//# sourceMappingURL=upload.client-compiled.js.map