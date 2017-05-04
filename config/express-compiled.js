'use strict';

var config = require('./config');
var express = require('express'),
    morgan = require('morgan'),
    compress = require('compression'),
    // gzip compression
bodyParser = require('body-parser'),
    methodOverride = require('method-override'),
    session = require('express-session'),
    passport = require('passport'),
    fs = require('fs'),
    path = require('path');

module.exports = function () {
    var app = express();

    if (process.env.NODE_ENV === 'development') {
        app.use(morgan('dev')); //:method :url :status :response-time ms - :res[content-length]
    } else if (process.env.NODE_ENV === 'production') {
        app.use(morgan('common')); // remote-addr - :remote-user [:date[clf]] ":method :url HTTP/:http-version" :status :res[content-length]
        app.use(compress());
    }

    app.use(bodyParser.urlencoded({
        extended: true // req will contain key-value pairs, where the value can be a string or array (when extended is false), or any type (when extended is true).
    }));

    app.use(bodyParser.json());
    app.use(methodOverride());

    // session data is stored in the server, only session ID is stored in the cookie
    app.use(session({
        saveUninitialized: true,
        resave: true,
        secret: config.sessionSecret
    }));

    app.set('views', './app/views');
    app.set('view engine', 'ejs');

    app.use(passport.initialize());
    app.use(passport.session());

    require('../app/routes/upload.server.routes.js')(app);
    require('../app/routes/users.server.routes.js')(app);
    require('../app/routes/index.server.routes.js')(app);

    app.use(express.static('./public'));
    app.use(express.static('./userUploadFiles/cppFiles'));
    return app;
};

//# sourceMappingURL=express-compiled.js.map