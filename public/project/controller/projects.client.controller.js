/**
 * Created by zhouxing on 5/6/17.
 */


// Invoke 'strict' JavaScript mode
'use strict';

// Create the 'articles' controller
angular.module('projects').controller('ProjectsController', ['$scope',  '$routeParams', '$location', 'Projects','$compile',
    function($scope, $routeParams, $location, Projects, $compile) {
        // Expose the Authentication service
        // $scope.authentication = Authentication;

        // Create a new controller method for creating new articles
        // $scope.create = function() {
        //     // Use the form fields to create a new article $resource object
        //     var project = new Projects({
        //         name: this.name,
        //     });
        //
        //     // Use the article '$save' method to send an appropriate POST request
        //     project.$save(function(response) {
        //         // If an article was created successfully, redirect the user to the article's page
        //         $location.path('articles/' + response._id);
        //     }, function(errorResponse) {
        //         // Otherwise, present the user with the error message
        //         $scope.error = errorResponse.data.message;
        //     });
        // };

        // Create a new controller method for retrieving a list of articles
        $scope.find = function() {
            // Use the article 'query' method to send an appropriate GET request
            $scope.projects = Projects.query();
        };

        // Create a new controller method for retrieving a single article
        $scope.findOne = function() {
            // Use the project 'get' method to send an appropriate GET request
            $scope.project = Projects.get({
                projectId: $routeParams.projectId
            });






        };

        $scope.clickFile = function(file){

            let currentFile = file;
            $scope.currentFileMetric = JSON.parse(currentFile.metrics);
            console.log($scope.currentFileMetric.overComplexFunctions.length);
            $scope.currentFileContent = currentFile.htmlContent;
            let sourceContainer = angular.element( document.querySelector('#fileSource'));
            sourceContainer.empty();
            var node = $compile('<div><br>' + $scope.currentFileContent + '</div>' )($scope);
            sourceContainer.append(node);
            $scope.show = 0;
        };

        $scope.calculateTotalBadFunc = function () {
            console.log($scope.project.files);

            $scope.overLinedFunc = [];
            $scope.overComplexityFunc = [];
            let files = $scope.project.files;
            for (let i = 0 ; i < files.length; i++) {
                $scope.overLinedFunc = $scope.overLinedFunc.concat(JSON.parse(files[i].metrics).overlinedFunctions);
                $scope.overComplexityFunc = $scope.overComplexityFunc.concat(JSON.parse(files[i].metrics).overComplexFunctions);
            }
        };


        // // Create a new controller method for updating a single article
        // $scope.update = function() {
        //     // Use the article '$update' method to send an appropriate PUT request
        //     $scope.article.$update(function() {
        //         // If an article was updated successfully, redirect the user to the article's page
        //         $location.path('articles/' + $scope.article._id);ar
        //     }, function(errorResponse) {
        //         // Otherwise, present the user with the error message
        //         $scope.error = errorResponse.data.message;
        //     });
        // };
        //
        // Create a new controller method for deleting a single article
        $scope.delete = function(project) {
            if(confirm('Delete this project?')) {
                // If an project was sent to the method, delete it
                if (project) {
                    // Use the project '$remove' method to delete the project
                    project.$remove(function() {
                        // Remove the project from the projects list
                        for (var i in $scope.projects) {
                            if ($scope.projects[i] === project) {
                                $scope.projects.splice(i, 1);
                            }
                        }
                    });
                } else {
                    $scope.project.$remove(function() {
                        $location.path('#!/projects');
                    });
                }
                location.href='#!/projects';
            }


        };

        $scope.showMetrics = function() {
            console.log("in showMetrics() function");

            $scope.show = 1;
            // $scope.calculateTotalBadFunc()
        }
        $scope.showDependencies = function() {
            // console.log("in showMetrics() function");

            $scope.show = 2;
            // $scope.calculateTotalBadFunc()
        }

    }
]);
