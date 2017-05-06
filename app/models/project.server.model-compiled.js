'use strict';

/**
 * Created by zhouxing on 5/4/17.
 */
var mongoose = require('mongoose'),
    Schema = mongoose.Schema;
var FileSchema = new Schema({
    created: {
        type: Date,
        default: Date.now
    },
    name: String,
    fileURL: String,
    metrics: String
});

var ProjectSchema = new Schema({
    created: {
        type: Date,
        default: Date.now
    },
    files: [FileSchema],
    dependency: String
});

mongoose.model('File', FileSchema);

mongoose.model('Project', ProjectSchema);

//# sourceMappingURL=project.server.model-compiled.js.map