#ifndef FACTORY_DEPENDENCYANALYSIS_H
#define FACTORY_DEPENDENCYANALYSIS_H
////////////////////////////////////////////////////////////////////////
//  Factory_DependencyAnalysis.h - builds and configures parsers      //
//  ver 1.1                                                           //
//                                                                    //
//  Lanaguage:     Visual C++ 2015                                    //
//  Platform:      Mac Book Pro, OS X EI Captain 10.11                //
//  Application:   Code Analysis, 687 OOD Project2                    //
//  Application:   Prototype for CSE687 Pr1, Sp06                     //
//  Author:        Xing Zhou, Syracuse University                     //
//                 Software Engineering                               //
//                 xzhou118@syr.edu                                   //
//  Source:        Jim Fawcett, CST 2-187, Syracuse University        //
//                 (315) 443-3948, jfawcett@twcny.rr.com              //
////////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module builds and configures parsers.  It builds the parser
parts and configures them with application specific rules and actions.

Public Interface:
=================
ConfigParserDependency conConfig;
conConfig.Build();
conConfig.Attach(someFileName);

Build Process:
==============
Required files
-     Parser.h, Parser.cpp,
      ITable.h, Table.h, Table.cpp,
      ItokCollection.h, SemiExp.h, SemiExp.cpp,
      tokenizer.h, tokenizer.cpp
      RuleActionForDependency.h, RuleActionForDependency.cpp
Build commands (either one)
- devenv Project3.sln

Maintenance History:
====================
ver 1.1 : 04 Apr 16
- provide add UsingNamespace
ver 1.0 : 12 Jan 06
- first release

*/

//
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "RuleActionForDependency.h"
#include "../Table/ITable.h"
#include "../Table/Table.h"
#include "../TypeAnalysis/Parser.h"
#include <fstream>

using pTable = ITable<TypeValue>*;

///////////////////////////////////////////////////////////////
// build parser that writes its output to console

class ConfigParserDependency : IBuilder
{
public:
    ConfigParserDependency(pTable _typetable) :typetable(_typetable) {};
    ~ConfigParserDependency();
    bool Attach(const std::string& name, bool isFile = true);
    Parser* Build();
    ITable<ResultValue>* getTable();
private:
    // Builder must hold onto all the pieces

    std::ifstream* pIn;
    Scanner::Toker* pToker;
    Scanner::SemiExp* pSemi;
    Parser* pParser;
    // add Rules and Actions

    // intialize and add action for rule
    ITable<ResultValue>* resultTable;
    ITable<TypeValue>* typetable;
    ContextForDependency* pContext;
    Dependency* pDependency;
    UsingNamespace* pUsingNamespace;
    UsingNamespaceAction* pUsingNamespaceAction;
    //MemberImple* pMemberImple;

    // prohibit copies and assignments

    ConfigParserDependency(const ConfigParserDependency&) = delete;
    ConfigParserDependency& operator=(const ConfigParserDependency&) = delete;
};

#endif
