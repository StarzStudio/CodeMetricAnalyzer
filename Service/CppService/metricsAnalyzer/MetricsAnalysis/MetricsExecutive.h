#ifndef METRICSEXECUTIVE_H
#define METRICSEXECUTIVE_H
////////////////////////////////////////////////////////////////////////
//  MetricExecutive.h - declares new parsing rules and actions        //
//  ver 1.3                                                           //
//  Platform:      Mac Book Pro, OS X EI Captain 10.11                //
//  Application:   Code Analysis, 687 OOD Project2                    //
//  Author:        Xing Zhou, Syracuse University                     //
//                 Software Engineering                               //
//                 xzhou118@syr.edu                                   //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University        //
//                 (315) 443-3948, jfawcett@twcny.rr.com              //
////////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module accepts a path parameter on the command line provided by user
and will go through that directionary, find any files in that directionary 
which meets the pattern and then analyze them.

Public Interface:
================= 
build()                                    // initialize Parser to generate AST, provide metric analyzer
analyzeSingle(std::string filename)        // print out a particular package's metric
AnalyzeDirectory(path, pattern)            // analyze all the files meeting the pattern in a given directory

Build Process:
==============
Required files
- ActionsAndRules.h, ActionsAndRules.cpp, SemiExpression.h, 
SemiExpression.cpp, tokenizer.h, tokenizer.cpp��ConfigureParser.h, 
ConfigureParser.cpp, Parser.h, Parser.cpp, ASTNode.h, ASTNode.cpp,
MetricsAnalysis.h, MetricsAnalysis.cpp.

Build commands
- devenv Project2.sln

Maintenance History:
====================
ver 1.4 : 12 Mar 16
- provide facility to turn on/off show AST structure infomation when analyzing
ver 1.3 : 06 Mar 16
- remove Configurer and Parser as private variables
move building process of Configure.build() into AnalysisDirectory
i.e. establish new configure for each file.
ver 1.2 : 06 Mar 16
- add fileSearch function to support search for all the files meeting 
the pattern in a given directory
- add function analysisDirectory to support analyze all the files meeting
the pattern in a given directory
ver 1.1 : 05 Mar 16
- add build function to initialize Configure, pParser, analyzer
- analyzeFunc(filename) wrap the analyzeFunc() in MetricAnalysis
ver 1.0 : 04 Mar 16
- first release

*/
//

#include "../FileSystem/FileSystem.h"
#include "../ASTNode/ASTNode.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/Parser.h"
#include "../Parser/ConfigureParser.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "MetricsAnalysis.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include "sys/types.h"

#ifdef NODE_ENV

#include <node.h>
#include <v8.h>
using namespace v8;

#endif


class MetricExecutive {
    using Path = std::string;
    using File = std::string;
    using Files = std::vector<std::string>;
    using Pattern = std::string;
    using Patterns = std::vector<std::string>;
public:
    MetricExecutive(Path in_path, Patterns in_patterns) : _path(in_path), _patterns(in_patterns) {}

    bool build();

    ASTTree *generateTree(const std::string &fileName);

    void fileSearch(const std::string &path);

    void start();

    void addPattern(std::string _pattern) {
        patterns.push_back(_pattern);
    }

    void outStream(std::string in_content, bool stdcout_on = true) {
        if (stdcout_on) {
            std::cout << in_content.c_str() << std::endl;
        }
        // metricsResultBuf << in_content << "\n";
    }

//    void store(bool into_file = true) {
//      if (into_file) {
//        std::ofstream metricsResult("./userUploadFiles/cppFiles/localhost/metricsResult.cpp");
//      }
//        // std::cout << "buf is:" << analyzer.analyzer.metricsResultBuf.str() << std::endl;
//        std::string bufferContent = analyzer.analyzer.metricsResultBuf.str();
//        metricsResult << bufferContent;
//        metricsResult.close();
//    }
    std::vector <std::string> metricInfoCollection() {
        return _metricInfoCollection;
    }

private:
    Files _fileCollection;
    std::ostringstream metricsResultBuf;
    MetricAnalysis analyzer;
    Path _path;
    Patterns _patterns;
    ConfigParseToConsole *configure;
    Parser *pParser;
    Patterns patterns;
    std::vector <std::string> _metricInfoCollection;
};


#ifdef NODE_ENV


void analyzeMatrics(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    HandleScope scope(isolate);
    // start path
    String::Utf8Value _path(args[0]->ToString()); //convert to std::string
    std::string path  = std::string(*_path);
// 	std::cout << "searching path: " << FileSystem::Directory::getCurrentDirectory().c_str() << std::endl;
    std::cout << "metrics analyzer searching path: " << path.c_str() << std::endl;
     std::cout << "====================================================" << std::endl;

    // provide patterns for file manager
    std::vector<std::string> patterns = {"*.h", "*.cpp", "*.cc", "*.c", "*.C"};

   MetricExecutive exe(path, patterns);


   exe.start();
   std::vector<std::string> metrics =  exe.metricInfoCollection();

    Local<Array> array = Array::New(isolate,metrics.size());

    for (int i = 0; i < metrics.size(); i++) {
        array->Set(i,String::NewFromUtf8(isolate, metrics[i].c_str()));
    }


    args.GetReturnValue().Set(array);
}


void Init(Local<Object> exports, Local<Object> module) {
  NODE_SET_METHOD(module, "exports", analyzeMatrics);
}

NODE_MODULE(analyzeMatrics, Init)
#endif

#endif