/**
 * Created by zhouxing on 5/6/17.
 */


// Invoke 'strict' JavaScript mode
'use strict';

// Create the 'articles' controller
angular.module('playground').controller('PlaygroundController', ['$scope', '$routeParams', 'Playground',
    function($scope, $routeParams, Playground) {


        // Create a new controller method for creating new articles
        $scope.create = function() {
            let editor = ace.edit("code");

            var cppContent =  editor.getValue();
            var playground = new Playground({
                cppContent: cppContent,
            });

            console.log(playground);
            // Use '$save' method to send an appropriate POST request
            playground.$save(function(response) {
                    console.log(`playground post success:\n`);
                    console.log(response);
                    if(response.complieStderr === "") {
                        $scope.complieOutput = response.complieStdout;
                        if(response.runStderr === "") {
                            $scope.runOutput = response.runStdout;
                        } else {
                            $scope.runOutput = response.runStderr;
                        }
                    } else {
                        $scope.complieOutput = response.complieStderr;

                    }
                }, function(errorResponse) {
                // Otherwise, present the user with the error message
                $scope.error = errorResponse.data.message;
            });
        };

    }
]);
