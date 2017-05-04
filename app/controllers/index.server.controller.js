/**
 * Created by Xing Zhou on 5/3/17.
 */



exports.render = function(req, res) {
    updateSession(req);
    res.render('index'); // send .ejs file to the client
};

function updateSession(req) {
    if (req.session.lastVisit) {
        console.log(req.session.lastVisit);
    }
    req.session.lastVisit = new Date();
}