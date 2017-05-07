/**
 * Created by zhouxing on 5/6/17.
 */
/**
 * Created by zhouxing on 5/6/17.
 */

var playground = require('../../app/controllers/playground.server.controller');

module.exports = function(app) {
    app.route('/api/playground')
        .post(playground.run);
};
