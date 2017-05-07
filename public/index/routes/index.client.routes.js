/**
 * Created by zhouxing on 5/3/17.
 */

angular.module('index').config(['$routeProvider',
    function($routeProvider) {
        $routeProvider
            .when('/', {
                templateUrl: '/upload/views/uploadPage.client.view.html'
            })
            .when('/playground', {
               templateUrl: '/playground/views/playground.client.html'
            })
            .otherwise({
                redirectTo: '/'
            });
    }
]);
