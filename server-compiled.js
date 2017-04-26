'use strict';

process.env.NODE_ENV = process.env.NODE_ENV || 'development';
var v = require("./build/Release/palindrome.node");

var config = require('./config/config');
var mongoose = require('./config/mongoose'),
    express = require('./config/express');
var db = mongoose();
var app = express();
var port = config.port;

var isPal = v("wow");
console.log(isPal);
app.listen(port);

module.exports = app;

console.log('Server running at http://localhost:' + port + '/');

//# sourceMappingURL=server-compiled.js.map