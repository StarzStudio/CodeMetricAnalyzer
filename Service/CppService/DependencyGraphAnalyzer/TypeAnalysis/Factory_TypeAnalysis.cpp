////////////////////////////////////////////////////////////////////////
//  Factory_TypeAnalysis.cpp - builds and configures parsers          //
//  ver 1.2                                                           //
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
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "Factory_TypeAnalysis.h"

using namespace Scanner;
using pTable = ITable<TypeValue>*;

//----< destructor releases all parts >------------------------------

ConfigParser::~ConfigParser()
{
    // when Builder goes out of scope, everything must be deallocated
    delete pContextForType;
    delete pBeginningOfScope;
    delete pIncreaseCount;
    delete pEndOfScope;
    delete pDecreaseCount;
    delete pGlobalFunction;
    delete pMakeRecordForGlobalFunc;
    delete pType;
    delete pMakeRecordForType;
    delete pFindNamspace;
    delete pAddNamespace;
    //delete pDeclaration;
    //delete pMakeRecordForDeclare;
    delete pMemberDef;
    delete pMakeRecordForMemberDef;
	delete pAliasTypedef;
	delete pAliasUsing;
	delete pGlobalLambda;
	delete pPushLambda;
    delete _pTable;
    delete pParser;
    delete pSemi;
    delete pToker;
    pIn->close();
    delete pIn;
}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParser::Attach(const std::string& name, bool isFile)
{
    if (pToker == 0)
        return false;
    pIn = new std::ifstream(name);
    if (!pIn->good())
        return false;
    // let context class know the name of current analyzed file
    std::string shortFileName = FileSystem::Path::getName(name);
    pContextForType->setCurrentFile(shortFileName);
    return pToker->attach(pIn);
}

// configure to manage start scope
void ConfigParser::buildBeginningOfScope()
{
    pBeginningOfScope = new BeginningOfScope(pContextForType);
    pIncreaseCount = new IncreaseCount(pContextForType);
    pBeginningOfScope->addAction(pIncreaseCount);
    pParser->addRule(pBeginningOfScope);
}

// configure to manage end scope
void ConfigParser::buildEndOfScope()
{
    pEndOfScope = new EndOfScope(pContextForType);
    pDecreaseCount = new DecreaseCount(pContextForType);
    pEndOfScope->addAction(pDecreaseCount);
    pParser->addRule(pEndOfScope);
}

// configure to detect and act on global function declaration
void ConfigParser::buildGlobalFunction()
{
    pGlobalFunction = new GlobalFunction(pContextForType);
    pMakeRecordForGlobalFunc = new MakeRecordForGlobalFunc(pContextForType);
    pGlobalFunction->addAction(pMakeRecordForGlobalFunc);
    pParser->addRule(pGlobalFunction);
}

// configure to detect and act on Type scope (: class, struct..)
void ConfigParser::buildType()
{
    pType = new Type(pContextForType);
    pMakeRecordForType = new MakeRecordForType(pContextForType);
    pType->addAction(pMakeRecordForType);
    pParser->addRule(pType);
}

// configure to detect and act on Type scope (: class, struct..)
void ConfigParser::buildFindNamespace()
{
    pFindNamspace = new FindNamspace(pContextForType);
    pAddNamespace = new AddNamespace(pContextForType);
    pFindNamspace->addAction(pAddNamespace);
    pParser->addRule(pFindNamspace);
}

// configure to detect member function definition
void ConfigParser::buildMemberFunc()
{
    pMemberDef = new MemberDef(pContextForType);
    pMakeRecordForMemberDef = new MakeRecordForMemberDef(pContextForType);
    pMemberDef->addAction(pMakeRecordForMemberDef);
    pParser->addRule(pMemberDef);
}
// configure to detect alias
void ConfigParser::buildAlias()
{
	// typedef part
	pAliasTypedef = new AliasTypedef(pContextForType);
	pMakeRecordForAliasTypedef = new MakeRecordForAliasTypedef(pContextForType);
	pAliasTypedef->addAction(pMakeRecordForAliasTypedef);
	pParser->addRule(pAliasTypedef);
	// using part
	pAliasUsing = new AliasUsing(pContextForType);
	pMakeRecordForAliasUsing = new MakeRecordForAliasUsing(pContextForType);
	pAliasUsing->addAction(pMakeRecordForAliasUsing);
	pParser->addRule(pAliasUsing);

}
// configure to detect global scope lambda
void ConfigParser::buildLambda()
{
	pGlobalLambda = new GlobalLambda(pContextForType);
	pPushLambda = new PushLambda(pContextForType);
	pGlobalLambda->addAction(pPushLambda);
	pParser->addRule(pGlobalLambda);
}

//----< Here's where all the parts get assembled >----------------

Parser* ConfigParser::Build()
{
    try
    {
        // add Parser's main parts

        pToker = new Toker;
        pToker->returnComments(false);
        pSemi = new SemiExp(pToker);
        pParser = new Parser(pSemi);
        _pTable = new Table<TypeValue>();
        pContextForType = new contextForType(_pTable);

        buildFindNamespace();   // detect namespace scope
       // buildDeclaration();
        buildMemberFunc();
        buildGlobalFunction();  // configure to detect and act on global function declaration
        buildType();            // configure to detect and act on type scope ( class, struct..)
		buildAlias();
		buildLambda();
        buildBeginningOfScope();
        buildEndOfScope();
	   

        return pParser;
    }
    catch (std::exception& ex)
    {
        std::cout << "\n\n  " << ex.what() << "\n\n";
        return 0;
    }
}

// return table
pTable ConfigParser::getTable()
{
    return _pTable;
}
#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>

int main(int argc, char* argv[])
{
    std::cout << "\n  Testing ConfigureParser module\n "
        << std::string(32, '=') << std::endl;

    // collecting tokens from files, named on the command line

    if (argc < 2)
    {
        std::cout
            << "\n  please enter name of file to process on command line\n\n";
        return 1;
    }

    for (int i = 1; i < argc; ++i)
    {
        std::cout << "\n  Processing file " << argv[i];
        std::cout << "\n  " << std::string(16 + strlen(argv[i]), '-');

        ConfigParser configure;
        Parser* pParser = configure.Build();
        try
        {
            if (pParser)
            {
                if (!configure.Attach(argv[i]))
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

            while (pParser->next())
                pParser->parse();
            std::cout << "\n\n";

            // display table content
            configure.getTable()->print();
        }
        catch (std::exception& ex)
        {
            std::cout << "\n\n    " << ex.what() << "\n\n";
        }
        std::cout << "\n\n";
    }
}

#endif