'use strict';

module.exports = function (app) {
    var uploadController = require('../controllers/upload.server.controller');
    //app.get('/', uploadFileController.render);


    app.get('/upload', uploadController.render);

    app.post('/upload', uploadController.receiveFiles);
};

//# sourceMappingURL=upload.server.routes-compiled.js.map