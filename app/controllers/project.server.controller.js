/**
 * Created by zhouxing on 5/5/17.
 */
const mongoose = require('mongoose');
const Project = mongoose.model('Project');
const File = mongoose.model('File');

const getErrorMessage = function (err) {
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
    for (let fileInfo of req.fileInfos) {
        // let fileName = fileInfo.name;
        // let fileURL = fileInfo.fileURL;
        // let fileMetrics = fileInfo.metrics;
        let file = new File(fileInfo);
        project.files.push(file);
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
        if (!project) return next(new Error('Failed to load project '
            + id));
        req.project = project;

        next();
    });
};

exports.read = function(req, res) {
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