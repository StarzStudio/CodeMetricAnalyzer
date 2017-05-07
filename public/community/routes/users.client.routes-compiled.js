'use strict';

/**
 * Created by zhouxing on 5/7/17.
 */

angular.module('users').config(['$routeProvider', function ($routeProvider) {
    $routeProvider.when('/users', {
        templateUrl: '/community/views/list-users.client.view.html'
    }).when('/users/:userId', {
        templateUrl: '/community/views/view-user.client.view.html'
    }).otherwise({
        redirectTo: '/'
    });
}]);

//# sourceMappingURL=users.client.routes-compiled.js.map