'use strict';

/**
 * Created by zhouxing on 5/6/17.
 */

var projects = require('../../app/controllers/project.server.controller');

module.exports = function (app) {
    app.route('/api/projects').get(projects.list).post(projects.create);
    app.route('/api/projects/:projectId').get(projects.read)
    // .put(projects.update)
    .delete(projects.delete);
    app.param('projectId', projects.projectByID);
};

//# sourceMappingURL=project.server.routes-compiled.js.map