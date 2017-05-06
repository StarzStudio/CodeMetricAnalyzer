/**
 * Created by zhouxing on 5/4/17.
 */
const mongoose = require('mongoose'),
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
    dependency: String,
    // creator: {
    //     type: Schema.ObjectId,
    //     ref: 'User'
    // }
});


mongoose.model('File', FileSchema);



mongoose.model('Project', ProjectSchema);