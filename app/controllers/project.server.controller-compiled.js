'use strict';

/**
 * Created by zhouxing on 5/5/17.
 */
var mongoose = require('mongoose');
var Project = mongoose.model('Project');
var File = mongoose.model('File');

var getErrorMessage = function getErrorMessage(err) {
    if (err.errors) {
        for (var errName in err.errors) {
            if (err.errors[errName].message) return err.errors[errName].message;
        }
    } else {
        return 'Unknown server error';
    }
};

exports.create = function (req, res) {
    var project = new Project();
    var _iteratorNormalCompletion = true;
    var _didIteratorError = false;
    var _iteratorError = undefined;

    try {
        for (var _iterator = req.fileInfos[Symbol.iterator](), _step; !(_iteratorNormalCompletion = (_step = _iterator.next()).done); _iteratorNormalCompletion = true) {
            var fileInfo = _step.value;

            // let fileName = fileInfo.name;
            // let fileURL = fileInfo.fileURL;
            // let fileMetrics = fileInfo.metrics;
            var file = new File(fileInfo);
            project.files.push(file);
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

    project.name = req.body.projectName;
    //project.creator = req.user;
    project.save(function (err) {
        if (err) {
            return res.status(400).send({
                message: getErrorMessage(err)
            });
        } else {
            res.json(project);
        }
    });
};

exports.list = function (req, res) {
    console.log("in list");
    Project.find().sort('-created').exec(function (err, projects) {
        if (err) {
            return res.status(400).send({
                message: getErrorMessage(err)
            });
        } else {
            res.json(projects);
        }
    });
};

exports.projectByID = function (req, res, next, id) {
    Project.findById(id).exec(function (err, project) {
        if (err) return next(err);
        if (!project) return next(new Error('Failed to load project ' + id));
        req.project = project;

        next();
    });
};

exports.read = function (req, res) {
    res.json(req.project);
};

// exports.update = function(req, res, next) {
//     var article = req.article;
//     article.title = req.body.title;
//     article.content = req.body.content;
//     article.save(function(err) {
//         if (err) {
//             return res.status(400).send({
//                 message: getErrorMessage(err)
//             });
//         } else {
//             res.json(article);
//         }
//     });
//     next();
// };
//
// exports.delete = function(req, res) {
//     var article = req.article;
//     article.remove(function(err) {
//         if (err) {
//             return res.status(400).send({
//                 message: getErrorMessage(err)
//             });
//         } else {
//             res.json(article);
//         }
//     });
// };

//# sourceMappingURL=project.server.controller-compiled.js.map