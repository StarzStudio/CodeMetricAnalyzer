'use strict';

/**
 * Created by zhouxing on 5/3/17.
 */

module.exports = function (app) {
  var indexController = require('../controllers/index.server.controller');
  var uploadController = require('../controllers/upload.server.controller');
  //app.get('/', uploadFileController.render);


  app.get('/', indexController.render);
  app.post('/', uploadController.receiveFiles);
  // app.get('/signIn', {
};

//# sourceMappingURL=index.server.routes-compiled.js.map