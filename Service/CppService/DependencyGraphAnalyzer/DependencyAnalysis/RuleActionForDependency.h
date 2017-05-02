#ifndef RULEACTIONFORDEPENDENCY_H
#define RULEACTIONFORDEPENDENCY_H
////////////////////////////////////////////////////////////////////////
//  RuleActionForDependency.h - declares new parsing rules and actions//
//  ver 1.1                                                           //
//  Language:      Visual C++ 2016                                    //
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
This module provides actions and rules to parser through a file and find
dependent files based on given type table

Public Interface:
=================

Build Process:
==============
Required files
-     Parser.h, Parser.cpp,
      ITable.h, Table.h, Table.cpp,
      ItokCollection.h, SemiExp.h, SemiExp.cpp,
      tokenizer.h, tokenizer.cpp
Build commands
- devenv Project3.sln

Maintenance History:
====================
ver 1.2 : 07 Apr 16
- add mechanism to detect member function type and using suffix to discard 
adding dependency
ver 1.1 : 02 Apr 16
- merge DoulbeColon and NoDoubleColon rule class into one class:Dependency
- elimate redundant adding dependent file by changing ResultValue class's add
  behaviour in Table.h file
ver 1.0 : 29 Mar 16
- first release

TODO:
- rule should detect wrapper declaration, global function declaration

*/

#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include "../TypeAnalysis/Parser.h"
#include "../SemiExp/itokcollection.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../Table/ITable.h"
#include "../Table/Table.h"

using pResultTable = ITable<ResultValue>*;
using pTypeTable = ITable<TypeValue>*;

///////////////////////////////////////////////////////////////
// ContextForDependency which will store type table for analysis, result table to store result
// store current file's namespace collection ,current file name, Value that will store
// dependency file's name collection

class ContextForDependency
{
    using Collection = std::vector<std::string>;
public:
    ContextForDependency(pTypeTable typeTable, pResultTable resultTable) : _pTypeTable(typeTable), _pResultTable(resultTable)
    {
        pValueResult = new ResultValue();
    }
    // add one namespace string into collection
    void addNamespace(const std::string& _namespace)
    {
        _namespaceCollection.push_back(_namespace);
    }
    // get the collection of current file's namespace
    Collection& getNamespaceCollection()
    {
        return _namespaceCollection;
    }
    // find if a namespace is in this collection
    bool find(std::string _namespace)
    {
        for (auto it = _namespaceCollection.begin(); it != _namespaceCollection.end(); it++)
        {
            if ((*it) == _namespace)
                return true;
        }
        return false;
    }
    // return type Table
    pTypeTable getTypeTable()
    {
        return _pTypeTable;
    }
    // return result table
    pResultTable getResultTable()
    {
        return _pResultTable;
    }
    // tell ContextForDependency current file name, and initialize result table
    void setCurrentFile(std::string _currentFile)
    {
        currentFile = _currentFile;
        ResultValue value;
        _pResultTable->addRecord(currentFile, value);
    }
    // return current file name
    const std::string& getCurrentFile()
    {
        return currentFile;
    }
    // get the value which will be store into result table, in which key is current file name
    ResultValue* getValueResult()
    {
        return pValueResult;
    }
private:
    Collection _namespaceCollection;
    pTypeTable _pTypeTable;
    pResultTable _pResultTable;
    std::string currentFile;
    ResultValue* pValueResult;
};

// detect and add dependency
class Dependency : public IRule
{
    using Key = std::string;
    using File = std::string;
    using Files = std::vector<std::string>;
public:
    Dependency(ContextForDependency* _pContextForDependency) :pContextForDependency(_pContextForDependency) {}
    bool haveDoubleColon(ITokCollection*& pTc, std::string name);
    void searchNamespace(std::string key);
    void AddOneFileInDependentValue(ResultValue& _ValueResult, File _file);
	bool NoUsingBefore(std::string key, ITokCollection*& pTc);
	bool memberFunctionType(std::string key);
	bool preTest(std::string key, ITokCollection*& pTc);
	void doAction(std::string key, ITokCollection*& pTc);
    bool doTest(ITokCollection*& pTc);
private:
    ContextForDependency* pContextForDependency;
};

// detect namespace
class UsingNamespace : public IRule
{
public:
    UsingNamespace(ContextForDependency* _pContextForDependency) :pContextForDependency(_pContextForDependency) {}
    bool doTest(ITokCollection*& pTc)
    {
        ITokCollection& tc = *pTc;
        tc.remove("\n");
        size_t len = tc.find("namespace");
        // len must bigger than 0, or index out of range,
        // e.g. namespace ns {}
        if (0 < len && len != tc.length() && tc[len - 1] == "using")
        {
            doActions(pTc);
            return true;
        }
        return true;
    }
private:
    ContextForDependency* pContextForDependency;
};

// add namespace
class UsingNamespaceAction : public IAction
{
public:
    UsingNamespaceAction(ContextForDependency* _pContextForDependency)
    {
        pContextForDependency = _pContextForDependency;
    }
    void doAction(ITokCollection*& pTc)
    {
        ITokCollection& tc = *pTc;
        pContextForDependency->addNamespace(tc[tc.find("namespace") + 1]);
    }
private:
    ContextForDependency* pContextForDependency;
};

#endif
