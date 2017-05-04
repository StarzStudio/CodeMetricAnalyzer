'use strict';

/**
 * Created by xing on 4/27/17.
 */

var mainApplicationModuleName = 'codeAnalyzer';
var mainApplicationModule = angular.module(mainApplicationModuleName, ['ngRoute', 'index']);

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
      templateUrl: '/upload/views/FileList.client.view.html'
   }).when('/projects', {
      templateUrl: './projects/projects.client.html'
   }).when('/projects/projectDetail', {
      templateUrl: './projectDetail/projectDetail.client.html'
   }).otherwise({
      redirectTo: '/'
   });
}]);

angular.element(document).ready(function () {
   angular.bootstrap(document, [mainApplicationModuleName]);
});

//# sourceMappingURL=application-compiled.js.map