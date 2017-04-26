var path = require("path");


exports.render = function(req, res) {


    updateSession(req);

    var indexFilePath = path.resolve('public/views/upload.html');
    // console.log(indexFilePath);
    res.sendFile(indexFilePath);

};


function updateSession(req) {
    if (req.session.lastVisit) {
        console.log(req.session.lastVisit);
    }
    req.session.lastVisit = new Date();
}