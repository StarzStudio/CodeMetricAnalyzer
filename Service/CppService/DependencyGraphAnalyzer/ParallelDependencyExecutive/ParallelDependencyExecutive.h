#ifndef PARALLELDEPENDENCYEXECUTIVE_H
#define PARALLELDEPENDENCYEXECUTIVE_H
//////////////////////////////////////////////////////////////////////////////
// parallelDependencyExecutive.h - test executive                           //
// ver 1.0                                                                  //
// Language:      C++, Visual Studio 2015                                   //
// Platform:      Mac Book Pro, OS X EI Captain 10.11                       //
// Application:   dependency Analysis, 687 OOD Project3                     //
// Author:        Xing Zhou, Syracuse University, Software Engineering      //
//                xzhou118@syr.edu                                          //
//////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is for demo purpose
*
*
* Build Process:
* --------------
* Required Files:
*     Parser.h, Parser.cpp,
*     ITable.h, Table.h, Table.cpp,
*     ItokCollection.h, SemiExp.h, SemiExp.cpp,
*     tokenizer.h, tokenizer.cpp
*     RuleActionForDependency.h, RuleActionForDependency.cpp
*     Factory_DependencyAnalysis.h, Factory_DependencyAnalysis.cpp
*     RuleActionForTypeTable.h, RuleActionForTypeTable.cpp
*     Factory_TypeAnalysis.h, Factory_TypeAnalysis.cpp
*     FileSystem.h, FileSystem.cpp
*     FileMgr.h, FileMgr.cpp
*     paralellDependencyAnalysis.h, paralellDependencyAnalysis.cpp
*
* Build Command: devenv project3.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 06 Apr 2016
* - first release
*
*/

#include "../ParallelDependencyAnalysis/ParallelDependencyAnalysis.h"

class parallelDependencyExecutive
{
    using Path = std::string;
    using Pattern = std::string;
    using Patterns = std::vector<Pattern>;
public:
    parallelDependencyExecutive(Path _path, Patterns _patterns) : parallelAnalyzer(_path, _patterns) {}
    void explaination()
    {
        std::cout << "  This is demo for 687 OOD project 3: \n";
        std::cout << "  ======================================";
        std::cout << "  The process is divided into two passes:\n";
        std::cout << "  first pass: detect all the files given the patterns and path.\n";
        std::cout << "  construct type table for each file and merge them in the end.\n";
        std::cout << "  Only merged type table will be shown.\n";
        std::cout << "  second pass: detect all the files given the patterns and path.\n";
        std::cout << "  given the previous merged type table, construct dependency table \n";
        std::cout << "  for each file and merge them in the end.\n";
		std::cout << "  Only merged dependency table will be shown as the final result.\n\n";
		std::cout << "  ===================================================================";
		std::cout << "  4 threads will be running together and thread id is tagged after \n";
		std::cout << "  each task shown below\n";
    }
    void analyze()
    {
        parallelAnalyzer.analyze();
    }
private:
    parallelDependency parallelAnalyzer;
};
#endif