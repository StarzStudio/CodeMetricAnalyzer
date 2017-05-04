'use strict';

/**
 * Created by zhouxing on 5/4/17.
 */
var mongoose = require('mongoose'),
    Schema = mongoose.Schema;

var ProjectsSchema = new Schema({
    userID: {
        type: String,
        required: true
    },
    overLineFunctons: {
        type: [String]
    },
    overComplexityFunctons: {
        type: [String]
    },
    globalFunctons: {
        type: [String]
    },
    globalLambdas: {
        type: [String]
    }
});

var MetricsSchema = new Schema({
    userID: {
        type: String,
        required: true
    },
    overLineFunctons: {
        type: [String]
    },
    overComplexityFunctons: {
        type: [String]
    },
    globalFunctons: {
        type: [String]
    },
    globalLambdas: {
        type: [String]
    }
});

mongoose.model('Metrics', MetricsSchema);

var DependencySchema = new Schema({
    title: {
        type: String,
        required: true
    },
    content: {
        type: String,
        required: true
    },
    author: {
        type: Schema.ObjectId,
        ref: 'User'
    }
});

mongoose.model('Post', PostSchema);

//# sourceMappingURL=metrics.server.model-compiled.js.map