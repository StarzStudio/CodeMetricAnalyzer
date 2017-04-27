
module.exports = function(app) {
    var uploadController = require('../controllers/upload.server.controller');
    //app.get('/', uploadFileController.render);

    app.get('/', uploadController.render);


    app.post('/', uploadController.receiveFiles) ;





};




