/**
 * Created by zhouxing on 5/7/17.
 */

angular.module('users').config(['$routeProvider',
    function($routeProvider) {
        $routeProvider
            .when('/community', {
                templateUrl: '/community/views/list-users.client.view.html'
            })
            .when('/community/:userId', {
                templateUrl: '/community/views/view-user.client.view.html'
            })
            .otherwise({
                redirectTo: '/'
            });
    }
]);
