'use strict';

/**
 * Created by zhouxing on 5/3/17.
 */

angular.module('index').config(['$routeProvider', function ($routeProvider) {
    $routeProvider.when('/', {
        templateUrl: '/upload/views/uploadPage.client.view.html'
    }).when('/playground', {
        templateUrl: '/playground/views/playground.client.view.html'
    }).when('/community', {
        templateUrl: '/community/views/community.client.view.html'
    }).otherwise({
        redirectTo: '/'
    });
}]);

//# sourceMappingURL=index.client.routes-compiled.js.map