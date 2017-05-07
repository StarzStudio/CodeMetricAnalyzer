const process = require('child_process')
console.log(__dirname);

		process.exec('g++-4.9 -std=c++14 -o tempExe temp.cpp', function (error, stdout, stderr) {
			if (error !== null) {
			  console.log('exec error: ' + error);
			}
			 console.log('complie stdout: ' + stdout);
			console.log('complie stderr: ' + stderr);

		});

			process.execFile('./tempExe',null, function (error,stdout,stderr) {
				if (error !== null) {
				  console.log('exec error: ' + error);
				}
				 console.log('run stdout: ' + stdout);
				console.log('run stderr: ' + stderr);
			});
