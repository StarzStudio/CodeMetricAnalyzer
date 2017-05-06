'use strict';

/**
 * Created by zhouxing on 5/6/17.
 */

// Configure the 'articles' module routes
angular.module('projects').config(['$routeProvider', function ($routeProvider) {
    $routeProvider.when('/projects', {
        templateUrl: 'project/views/list-projects.client.view.html'
    }).
    // when('/project/create', {
    //     templateUrl: 'articles/views/create-article.client.view.html'
    // }).
    when('/projects/:projectId', {
        templateUrl: 'project/views/view-project.client.view.html'
    });
    // when('/project/:projectId/edit', {
    //     templateUrl: 'articles/views/edit-article.client.view.html'
    // });
}]);

//# sourceMappingURL=projects.routes.client-compiled.js.map