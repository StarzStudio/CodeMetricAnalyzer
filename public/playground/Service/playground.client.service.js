/**
 * Created by zhouxing on 5/6/17.
 */
angular.module('playground').factory('Playground', ['$resource', function($resource) {
    // Use the '$resource' service to return an article '$resource' object
    return $resource('api/playground');
}]);