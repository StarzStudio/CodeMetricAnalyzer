var path = require("path");


exports.render = function(req, res) {

    if (req.session.lastVisit) {
        console.log(req.session.lastVisit);
    }
    req.session.lastVisit = new Date();


    var indexFilePath = path.resolve('public/views/index.html');
    // console.log(indexFilePath);
    res.sendFile(indexFilePath);

};