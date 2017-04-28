#ifndef METRICSANALYSIS_H
#define METRICSANALYSIS_H
////////////////////////////////////////////////////////////////////////
//  MetricAnalysis.h - declares new parsing rules and actions         //
//  ver 1.2                                                           //
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
This module provides evaluating and displaying the size and complexity
of all global functions, static member functions, and non-static member
functions in a package.

Public Interface:
=================
void analyzeFunc();             // analyze the size and complexity of functions in AST

Build Process:
==============
Required files
- ActionsAndRules.h, ActionsAndRules.cpp, SemiExpression.h,
SemiExpression.cpp, tokenizer.h, tokenizer.cpp��ConfigureParser.h,
ConfigureParser.cpp, Parser.h, Parser.cpp, ASTNode.h, ASTNode.cpp


Build commands
- devenv Project2.sln

Maintenance History:
====================
ver 1.3 : 05 Mar 16
- provide setTree function 
ver 1.2 : 04 Mar 16
- Move build function to the MetricsExecutive package
and modify analysisFunc() to accept ASTNode* as parameter.
ver 1.1 : 03 Mar 16
- Provide build function to let this package open
Parser, take in file, and then do analysis
ver 1.0 : 02 Mar 16
- first release

*/
//

#include "../ASTNode/ASTNode.h"
#include <iostream>
#include <sstream>


struct MetricInfo {

    std::vector<std::string> overlinedFunctions;
    std::vector<std::string> overComplexityFunctions;
    
};
// provide DFS trasversal of AST to get node information
class MetricAnalysis
{
public:
    void setTree(ASTTree* _tree) { tree = _tree; }
    void analyzeFunc();
    MetricInfo metricInfo() {
        return _metricInfo;
    }
    std::ostringstream metricsResultBuf;
    void clean() {
        _metricInfo = MetricInfo();
    }
private:
    ASTTree* tree;
   
    std::string TreeWalk(ASTNode* root);
    MetricInfo _metricInfo;
};

#endif