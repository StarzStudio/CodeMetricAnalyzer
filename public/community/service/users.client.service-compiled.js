'use strict';

/**
 * Created by zhouxing on 5/7/17.
 */
angular.module('users').factory('Users', ['$resource', function ($resource) {
    // Use the '$resource' service to return an article '$resource' object
    return $resource('users/:userID', {
        userID: '@_id'
    }, {
        update: {
            method: 'PUT'
        }
    });
}]);

//# sourceMappingURL=users.client.service-compiled.js.map