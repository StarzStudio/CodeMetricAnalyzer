////////////////////////////////////////////////////////////////////////
//  ConfigureParser.cpp - builds and configures parsers               //
//  ver 2.1                                                           //
//                                                                    //
//  Lanaguage:     Visual C++ 2015                                    //
//  Platform:      Mac Book Pro, OS X EI Captain 10.11                //
//  Application:   Code Analysis, 687 OOD Project2                    //
//  Author:        Xing Zhou, Syracuse University                     //
//                 Software Engineering                               //
//                 xzhou118@syr.edu                                   //
//  Source:        Jim Fawcett, CST 2-187, Syracuse University        //
//                 (315) 443-3948, jfawcett@twcny.rr.com              //
////////////////////////////////////////////////////////////////////////

#include <fstream>
#include "Parser.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

using namespace Scanner;

//----< destructor releases all parts >------------------------------

ConfigParseToConsole::~ConfigParseToConsole()
{
  // when Builder goes out of scope, everything must be deallocated
    delete pInitializerScope;
    delete pPushInitializerScope;
    delete pWrapperScope;
    delete pPushWrapperScope;
    delete pCondScope;
    delete pPushCondScope;
    delete pHandlePush;
    delete pBeginningOfScope;
    delete pHandlePop;
    delete pEndOfScope;
    delete pPushFunction;
    delete pFunctionDefinition;
    delete pRepo;
    delete pParser;
    delete pSemi;
    delete pToker;
    pIn->close();
    delete pIn;
}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParseToConsole::Attach(const std::string& name, bool isFile)
{
  if(pToker == 0)
    return false;
  pIn = new std::ifstream(name);
  if (!pIn->good())
    return false;
  return pToker->attach(pIn);
}

// configure to manage start scope
void ConfigParseToConsole::buildBeginningOfScope()
{
    pBeginningOfScope = new BeginningOfScope();
    pHandlePush = new HandlePush(pRepo);
    pBeginningOfScope->addAction(pHandlePush);
    pParser->addRule(pBeginningOfScope);
}

// configure to manage end scope
void ConfigParseToConsole::buildEndOfScope()
{
    pEndOfScope = new EndOfScope();
    pHandlePop = new HandlePop(pRepo);
    pEndOfScope->addAction(pHandlePop);
    pParser->addRule(pEndOfScope);
}

// configure to detect and act on function definitions
void ConfigParseToConsole::buildFunctionDefinition()
{
    pFunctionDefinition = new FunctionDefinition;
    pPushFunction = new PushFunction(pRepo);
    pFunctionDefinition->addAction(pPushFunction);
    pParser->addRule(pFunctionDefinition);
}

// configure to detect and act on local scope (if, while, for, do-while)
void ConfigParseToConsole::buildCondScope()
{
    pCondScope = new CondScope;
    pPushCondScope = new PushCondScope(pRepo);
    pCondScope->addAction(pPushCondScope);
    pParser->addRule(pCondScope);
}

// configure to detect and act on Wrapper scope (namespace, class, struct)
void ConfigParseToConsole::buildWrapperScope()
{
    pWrapperScope = new WrapperScope;
    pPushWrapperScope = new PushWrapperScope(pRepo);
    pWrapperScope->addAction(pPushWrapperScope);
    pParser->addRule(pWrapperScope);
}

// configure to detect and act on intialize scope, such as arry[] = { }

void ConfigParseToConsole::buildInitializerScope()
{
    pInitializerScope = new InitializerScope;
    pPushInitializerScope = new PushInitializerScope(pRepo);
    pInitializerScope->addAction(pPushInitializerScope);
    pParser->addRule(pInitializerScope);
}

void ConfigParseToConsole::buildLambdaDefinition()
{
    pLambdaDefinition = new LambdaDefinition;
    pPushLambda = new PushLambda(pRepo);
    pLambdaDefinition->addAction(pPushLambda);
    pParser->addRule(pLambdaDefinition);
}


//----< Here's where all the parts get assembled >----------------

Parser* ConfigParseToConsole::Build()
{
  try
  {
    // add Parser's main parts

    pToker = new Toker;
    pToker->returnComment(false);
    pSemi = new SemiExp(pToker);
    pParser = new Parser(pSemi);
    pRepo = new Repository(pToker);

    // configure to manage scope
    // these must come first - they return true on match
    // so rule checking continues
    buildBeginningOfScope();
    buildEndOfScope();
    // configure to detect and act on function definitions
    // these will stop further rule checking by returning false
    buildFunctionDefinition();
    // configure to detect and act on Wrapper scope (namespace, class, struct)
    buildWrapperScope();
    // configure to detect and act on local scope (if, while, for, do-while)
    buildCondScope();
    // configure to detect and act on intialize scope, such as arry[] = { }
    buildInitializerScope();
    // configure to detect lambda
    buildLambdaDefinition();
    return pParser;
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
    return 0;
  }
}

#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ConfigureParser module\n "
            << std::string(32,'=') << std::endl;

  // collecting tokens from files, named on the command line

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');

    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << argv[i] << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }
      // now that parser is built, use it

      while(pParser->next())
        pParser->parse();
      std::cout << "\n\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
  }
}

#endif


