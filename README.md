# CodeMetricAnalyzer
MEAN stack website using C++ as backend service.

## Author
Xing Zhou, a graduate student majoring in Computer Engineering at Syracuse Univeristy.

[website](http://www.xingzhou.us)

Email: work.xingzhou@gmail.com


## Structure


![overview](/READMESource/images/CodeAnalyzerOverviewStructure.js)

This project consists of two parts:


#### 1. MEAN stack web project  
- Server part tech stack: Nodejs, Express, MongoDB  
  
- Client part tech stack: AngularJS, BootStrap, JQuery 
 
#### 2. C++ project 
 
   1. #####  Cpp To HTML: generate html file for C++ source files
   
   2. ##### Metrics Analyzer:  analyze source code and display code metrics
 

## Storage
1. Server code is stored in /app
2. All static web files are stored in /public 
3. All the files uploaded by user will be stored into /userUploadedFiles
4. C++ projects are stored in /Service/CppService

## Test Platform
Ubuntu Desktop 16.04 LTS

## Build

#### Build node server
In commond line, `cd  CodeMetricAnalyzer/`  , then type:
```
$ node server.js
```
This will run the node server at port 3000

#### For individual C++ project:
1. ##### cppToHTML:
    ```
    $cd /CodeMetricAnalyzer/Service/cppService/cppToHTML/
    $cd compile.sh
    $cd run.sh 
    ```
    (change parameters in run.sh file)
2. ##### metricsAnalyzer:
	```
    $ cd CodeMetricAnalyzer/Service/cppService/metricsAnalyzer/
   	$ sh compile.sh
    $ sh run.sh 
    ```
    (change parameters in run.sh file)


## Output:

#### Code Metrics:

![Code Metrics](/READMESource/images/MetricsOutput.js)

#### Dependency Metrics:

![Code Metrics](/READMESource/images/DependencyOutput.js)