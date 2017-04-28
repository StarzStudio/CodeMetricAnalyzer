#ifndef CONFIGUREPARSER_H
#define CONFIGUREPARSER_H
////////////////////////////////////////////////////////////////////////
//  ConfigureParser.h - builds and configures parsers                 //
//  ver 2.1                                                           //
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
  ConfigParseToConsole conConfig;
  conConfig.Build();
  conConfig.Attach(someFileName);

  Build Process:
  ==============
  Required files
    - ConfigureParser.h, ConfigureParser.cpp, Parser.h, Parser.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp, ASTNode.h, ASTNode.cpp
      SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands (either one)
    - devenv Project2.sln
    - cl /EHsc /DTEST_PARSER ConfigureParser.cpp parser.cpp \
         ActionsAndRules.cpp \
         semiexpression.cpp tokenizer.cpp /link setargv.obj

  Maintenance History:
  ====================
  ver 2.2 : 03 Mar 16
  - provide interface to return Repo
  ver 2.1 : 19 Feb 16
  - Added PrintFunction action to FunctionDefinition rule
  ver 2.0 : 01 Jun 11
  - Major revisions to begin building a strong code analyzer
  ver 1.1 : 01 Feb 06
  - cosmetic changes to ConfigureParser.cpp
  ver 1.0 : 12 Jan 06
  - first release

*/

//
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "ActionsAndRules.h"
#include "Parser.h"
#include <fstream>

///////////////////////////////////////////////////////////////
// build parser that writes its output to console

class ConfigParseToConsole : IBuilder
{
public:
  ConfigParseToConsole() {};
  ~ConfigParseToConsole();
  bool Attach(const std::string& name, bool isFile=true);
  Parser* Build();
  Repository* getRepo() { return pRepo; }
  void resetLineCount() 
  {
      pToker->resetLineCount();
  }
private:
  // Builder must hold onto all the pieces

  std::ifstream* pIn;
  Scanner::Toker* pToker;
  Scanner::SemiExp* pSemi;
  Parser* pParser;
  Repository* pRepo;

  // add Rules and Actions

  BeginningOfScope* pBeginningOfScope;
  HandlePush* pHandlePush;
  EndOfScope* pEndOfScope;
  HandlePop* pHandlePop;
  FunctionDefinition* pFunctionDefinition;
  PushFunction* pPushFunction;
  CondScope* pCondScope;
  PushCondScope* pPushCondScope;
  WrapperScope* pWrapperScope;
  PushWrapperScope* pPushWrapperScope;
  InitializerScope* pInitializerScope;
  PushInitializerScope* pPushInitializerScope;
  LambdaDefinition* pLambdaDefinition;
  PushLambda* pPushLambda;

  // build rules and actions
  void buildBeginningOfScope();
  void buildEndOfScope();
  void buildFunctionDefinition();
  void buildCondScope();
  void buildWrapperScope();
  void buildInitializerScope();
  void buildLambdaDefinition();

  // prohibit copies and assignments

  ConfigParseToConsole(const ConfigParseToConsole&) = delete;
  ConfigParseToConsole& operator=(const ConfigParseToConsole&) = delete;
};


#endif
