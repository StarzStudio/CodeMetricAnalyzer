#ifndef TYPEANALYSIS_H
#define TYPEANALYSIS_H
///////////////////////////////////////////////////////////////////////////
// TypeAnalysis - construct type table based by analyzing a file         //                                                         //
// ver 1.1                                                               //
// Language:      C++, Visual Studio 2015                                //
// Platform:      Mac Book Pro, OS X EI Captain 10.11                    //
// Application:   dependency Analysis, 687 OOD Project3                  //
// Author:        Xing Zhou, Syracuse University, Software Engineering   //
//                xzhou118@syr.edu                                       //
///////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package takes a single file, analyzes all the types defined in it, create
* a type table which may reserved for furthre use.
*
* Public interface:
* setFile(string)               set file for analyzer
* analyze()                     after get the file, begin to analze it
*
* Build Process:
* --------------
* Required Files: Itokcollection.h, SemiExpression.h, SemiExpression.cpp,
*                 Tokenizer.h, Tokenizer.cpp,
*                 Parser.h, Parser.cpp,
*                 RuleActionForTypeTable.h, RuleActionForTypeTable.cpp
*                 Factory_TypeAnalysis.h, Factory_TypeAnalysis.cpp
*                 ITable.h, Table.h, Table.cpp
*                 FileSystem.h, FileSystem.cpp
*
* Build Command: devenv project3.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.1 : 02 Apr 2016
* - change ConfigurParser into member variable, thus TypeAnalysis can store the TypeTable
* and return table pointer without leaving to null's risk.
* ver 1.0 : 27 Mar 2016
* - first release
*
*
*/

#include <unordered_map>
#include <string>
#include <vector>
#include "../SemiExp/SemiExp.h"
#include "../SemiExp/itokcollection.h"
#include "../Tokenizer/Tokenizer.h"
#include "RuleActionForTypeTable.h"
#include "Parser.h"
#include "Factory_TypeAnalysis.h"
#include "../Table/Table.h"
#include "../Table/ITable.h"
#include "../FileSystem/FileSystem.h"

// this is a functor which creates a parser every time before do type analysis
class TypeAnalysis
{
public:
    TypeAnalysis() {}
    void setFile(std::string _fileName);
    TypeAnalysis(std::string _fileName) : fileName(_fileName) {}
    ITable<TypeValue>* analyze();
private:
    ITable<TypeValue>* _pTypeTable;
    std::string fileName;
    ConfigParser configure;
};

#endif