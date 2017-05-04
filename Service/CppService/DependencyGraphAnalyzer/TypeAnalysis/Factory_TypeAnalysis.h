#ifndef FACTORY_TYPEANALYSIS_H
#define FACTORY_TYPEANALYSIS_H
////////////////////////////////////////////////////////////////////////
//  Factory_TypeAnalysis.h - builds and configures parsers            //
//  ver 1.2                                                           //
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

  Note: when building Parser, the beginOfScope rule must be first to remove all
  the unnecessary \n for further test.

  Public Interface:
  =================
  getTable                        return type table
  Build();                        build all the necessary parts for the parser
  Attach(someFileName);           attach file to toker

  Build Process:
  ==============
  Required files
    - Parser.h, Parser.cpp, SemiExpression.h, SemiExpression.cpp,
    tokenizer.h, tokenizer.cpp, ItokCollection.h
    RuleActionForTypeTable.h, RuleActionForTypeTable.cpp
    ITable.h, Table.h, Table.cpp,

  Build commands
    - devenv Project3.sln

  Maintenance History:
  ====================
  ver 1.2 : 04 Apr 16
  - add buildFindNamespace();
  ver 1.1 : 03 Apr 16
  - pass short file name, i.e. fileName without path info, into context
  ver 1.0 : 01 Apr 16
  - first release

*/

//
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "../TypeAnalysis/RuleActionForTypeTable.h"
#include "../Table/ITable.h"
#include "../Table/Table.h"
#include "Parser.h"
#include <fstream>

///////////////////////////////////////////////////////////////
// build parser that writes its output to console

class ConfigParser : IBuilder
{
    using pTable = ITable<TypeValue>*;

public:
    ConfigParser() {};
    ~ConfigParser();
    bool Attach(const std::string& name, bool isFile = true);
    Parser* Build();
    pTable getTable();
private:
    // Builder must hold onto all the pieces

    std::ifstream* pIn;
    Scanner::Toker* pToker;
    Scanner::SemiExp* pSemi;
    Parser* pParser;
    pTable _pTable;

    // add Rules and Actions

    contextForType* pContextForType;
    BeginningOfScope* pBeginningOfScope;
    IncreaseCount* pIncreaseCount;
    EndOfScope* pEndOfScope;
    DecreaseCount* pDecreaseCount;
    GlobalFunction* pGlobalFunction;
    MakeRecordForGlobalFunc* pMakeRecordForGlobalFunc;
    Type* pType;
    MakeRecordForType* pMakeRecordForType;
    FindNamspace* pFindNamspace;
    AddNamespace* pAddNamespace;
    MemberDef* pMemberDef;
    MakeRecordForMemberDef* pMakeRecordForMemberDef;
	AliasTypedef* pAliasTypedef;
	MakeRecordForAliasTypedef * pMakeRecordForAliasTypedef;
	AliasUsing* pAliasUsing;
	MakeRecordForAliasUsing* pMakeRecordForAliasUsing;
	GlobalLambda* pGlobalLambda;
	PushLambda* pPushLambda;
    /* Declaration* pDeclaration;
     MakeRecordForDeclare* pMakeRecordForDeclare;*/

     // build rules and actions
    void buildBeginningOfScope();
    void buildEndOfScope();
    void buildGlobalFunction();
    void buildType();
    void buildFindNamespace();
    void buildMemberFunc();
	void buildAlias();             // for both typedef and using
	void buildLambda();
    // void buildDeclaration();

     // prohibit copies and assignments
    ConfigParser(const ConfigParser&) = delete;
    ConfigParser& operator=(const ConfigParser&) = delete;
};

#endif
