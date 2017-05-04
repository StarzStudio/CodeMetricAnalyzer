#ifndef DEPENDENCYANALYSIS_H
#define DEPENDENCYANALYSIS_H
//////////////////////////////////////////////////////////////////////////////
// DependencyAnalysis.h - analyze dependency relationship between files   //
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
* This package uses the services of ThreadPool to exective callable object.
* It shall be generic.
*
* Public interface:
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
*
* Build Command: devenv project3.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 27 Mar 2016
* - first release
*
* Planned Additions and Changes:
* ------------------------------
* - move resultTable to parallel... package
*/

#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include "../Tokenizer/Tokenizer.h"
#include "../Table/ITable.h"
#include "../Table/Table.h"
#include "../TypeAnalysis/Parser.h"
#include "Factory_DependencyAnalysis.h"
#include "../TypeAnalysis/TypeAnalysis.h"

using namespace  Scanner;
class DependencyAnalysis
{
    using pTypeTable = ITable<TypeValue>*;
    using pResultTable = ITable<ResultValue>*;
    using File = std::string;
public:
    DependencyAnalysis(File _fileName, pTypeTable typetable) : fileName(_fileName), configure(typetable) {}
    DependencyAnalysis(pTypeTable typetable, File _fileName) : fileName(_fileName), configure(typetable) {}
    pResultTable analyze();
private:
    pTypeTable _pTypeTable;
    pResultTable _pResultTable;
    File fileName;
    ConfigParserDependency configure;
};

#endif