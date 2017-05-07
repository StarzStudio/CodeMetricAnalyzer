/**
 * Created by zhouxing on 5/6/17.
 */

// Invoke 'strict' JavaScript mode
'use strict';

// Create the 'articles' controller

angular.module('projects').controller('ProjectsController', ['$scope', '$routeParams', '$location', 'Projects', function ($scope, $routeParams, $location, Projects) {
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
    $scope.find = function () {
        // Use the article 'query' method to send an appropriate GET request
        $scope.projects = Projects.query();
    };

    // Create a new controller method for retrieving a single article
    $scope.findOne = function () {
        // Use the project 'get' method to send an appropriate GET request
        $scope.project = Projects.get({
            projectId: $routeParams.projectId
        });

        $scope.projectMetrics = {
            "overLinedFunc": [],
            "overComplexityFunc": []
        };

        var _iteratorNormalCompletion = true;
        var _didIteratorError = false;
        var _iteratorError = undefined;

        try {
            for (var _iterator = $scope.project.files[Symbol.iterator](), _step; !(_iteratorNormalCompletion = (_step = _iterator.next()).done); _iteratorNormalCompletion = true) {
                var f = _step.value;

                $scope.projectMetrics.overLinedFunc = $scope.projectMetrics.overLinedFunc.concat(f.metrics.overlinedFunctions);
                $scope.projectMetrics.overComplexityFunc = $scope.projectMetrics.overComplexityFunc.concat(f.metrics.overComplexFunctions);
            }
        } catch (err) {
            _didIteratorError = true;
            _iteratorError = err;
        } finally {
            try {
                if (!_iteratorNormalCompletion && _iterator.return) {
                    _iterator.return();
                }
            } finally {
                if (_didIteratorError) {
                    throw _iteratorError;
                }
            }
        }
    };

    $scope.clickFile = function (file) {
        var currentFile = file;
        $scope.currentFileMetric = JSON.parse(currentFile.metrics);
        $scope.currentFileURL = currentFile.fileURL;
        console.log($scope.currentFileMetric);
        $scope.show = 0;
    };
    // // Create a new controller method for updating a single article
    // $scope.update = function() {
    //     // Use the article '$update' method to send an appropriate PUT request
    //     $scope.article.$update(function() {
    //         // If an article was updated successfully, redirect the user to the article's page
    //         $location.path('articles/' + $scope.article._id);
    //     }, function(errorResponse) {
    //         // Otherwise, present the user with the error message
    //         $scope.error = errorResponse.data.message;
    //     });
    // };
    //
    // // Create a new controller method for deleting a single article
    // $scope.delete = function(article) {
    //     // If an article was sent to the method, delete it
    //     if (article) {
    //         // Use the article '$remove' method to delete the article
    //         article.$remove(function() {
    //             // Remove the article from the articles list
    //             for (var i in $scope.articles) {
    //                 if ($scope.articles[i] === article) {
    //                     $scope.articles.splice(i, 1);
    //                 }
    //             }
    //         });
    //     } else {
    //         // Otherwise, use the article '$remove' method to delete the article
    //         $scope.article.$remove(function() {
    //             $location.path('articles');
    //         });
    //     }
    // };

    // $scope.showMetrics = function() {
    //     $scope.projectMetrics =
    // }
}]);

//# sourceMappingURL=projects.client.controller-compiled.js.map