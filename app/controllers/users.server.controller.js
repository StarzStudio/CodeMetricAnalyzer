var User = require('mongoose').model('User');


exports.create = function(req, res, next) {
    var user = new User(req.body);
    user.save(function(err) {
        if (err) {
            return next(err);
        } else {
            res.json(user);
        }
    });
};

exports.alongSave = function(req, res) {
    let userName = req.ip;
    User.findOne({'name':userName}, function(err, user) {
        if (err) {
            console.log(err);
        }

        if(user) {
            console.log('this is an old user');
            res.status(200).json(user);
        } else {
            var newuser = new User();
            newuser.name = userName;
            newuser.save(function(err) {
                if (err) {
                    console.log(err);
                }
                else {
                    console.log('this is an new user');
                    console.log(newuser);
                    res.status(200).json(newuser);
                }
            });
        }
    });


};

exports.list = function(req, res, next) {
    User.find({}, function(err, users) {
        if (err) {
            return next(err);
        } else {
            res.json(users);
        }
    });
};

exports.read = function(req, res) {
    res.json(req.user);
};


exports.userByID = function(req, res, next, id) {
    User.findOne({
        _id: id
    }, function(err, user) {
        if (err) {
            return next(err);
        } else {
            req.user = user;
            next();
        }
    });
};

exports.update = function(req, res, next) {
    User.findByIdAndUpdate(req.user.id, req.body, function(err, user) {
        if (err) {
            return next(err);
        } else {
            res.json(user);
        }
    });
};

exports.delete = function(req, res, next) {
    req.user.remove(function(err) {
        if (err) {
            return next(err);
        } else {
            res.json(req.user);
        }
    })
};