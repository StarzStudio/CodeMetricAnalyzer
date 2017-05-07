'use strict';

/**
 * Created by xing on 4/27/17.
 */

var mainApplicationModuleName = 'codeAnalyzer';
var mainApplicationModule = angular.module(mainApplicationModuleName, ['ngResource', 'ngRoute', 'index', 'projects']);

//To mark your application routes as single-page application routes, you will
//need to use a routing scheme called Hashbangs. Hashbangs are implemented by
//adding an exclamation mark right after the hash sig
mainApplicationModule.config(['$locationProvider', function ($locationProvider) {
   $locationProvider.hashPrefix('!');
}]);
// solve Facebook's redirect bug that
// adds a hash part to the application's URL after the OAuth authentication round-trip.
if (window.location.hash === '#_=_') window.location.hash = '#!';

mainApplicationModule.config(['$routeProvider', function ($routeProvider) {
   $routeProvider.when('/', {
      templateUrl: '/upload/views/uploadPage.client.view.html'
   })
   // .when('/project', {
   //    templateUrl: 'project/project.client.html'
   // })
   // .when('/project/projectDetail', {
   //     templateUrl: './projectDetail/projectDetail.client.html'
   // })
   .otherwise({
      redirectTo: '/'
   });
}]);

angular.element(document).ready(function () {
   angular.bootstrap(document, [mainApplicationModuleName]);
});

//# sourceMappingURL=application-compiled.js.map