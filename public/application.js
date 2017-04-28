/**
 * Created by xing on 4/27/17.
 */

const mainApplicationModuleName = 'codeAnalyzer';
const mainApplicationModule = angular.module(mainApplicationModuleName,
    ['upload']);

angular.element($document).ready(function () {
   angular.bootstrap(document,[mainApplicationModuleName]);
});