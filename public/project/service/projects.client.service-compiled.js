'use strict';

/**
 * Created by zhouxing on 5/6/17.
 */
angular.module('projects').factory('Projects', ['$resource', function ($resource) {
    // Use the '$resource' service to return an article '$resource' object
    return $resource('api/projects/:projectId', {
        articleId: '@_id'
    }, {
        update: {
            method: 'PUT'
        }
    });
}]);

//# sourceMappingURL=projects.client.service-compiled.js.map