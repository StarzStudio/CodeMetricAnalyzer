////////////////////////////////////////////////////////////////////////
//  Factory_TypeAnalysis.cpp - builds and configures parsers          //
//  ver 1.1                                                           //
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
#include "Factory_DependencyAnalysis.h"
#include "../FileMgr/FileMgr/FileSystem.h"
#include "../TypeAnalysis/TypeAnalysis.h"

using namespace Scanner;
using pTable = ITable<TypeValue>*;

//----< destructor releases all parts >------------------------------

ConfigParserDependency::~ConfigParserDependency()
{
    // when Builder goes out of scope, everything must be deallocated
   // delete pMemberImple;
    delete resultTable;
    delete pContext;
    delete pDependency;
    delete pUsingNamespace;
    delete pUsingNamespaceAction;
    delete pParser;
    delete pSemi;
    delete pToker;
    pIn->close();
    delete pIn;
}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParserDependency::Attach(const std::string& name, bool isFile)
{
    if (pToker == 0)
        return false;
    pIn = new std::ifstream(name);
    if (!pIn->good())
        return false;
    // let context class know the name of current analyzed file
    std::string shortFileName = FileSystem::Path::getName(name);
    pContext->setCurrentFile(shortFileName);
    return pToker->attach(pIn);
}

//----< Here's where all the parts get assembled >----------------

Parser* ConfigParserDependency::Build()
{
    try
    {
        // add Parser's main parts

        pToker = new Toker;
        pToker->returnComments(false);
        pSemi = new SemiExp(pToker);
        pParser = new Parser(pSemi);
        resultTable = new Table<ResultValue>();
        pContext = new ContextForDependency(typetable, resultTable);
        pDependency = new Dependency(pContext);
        // pMemberImple = new MemberImple(pContext);
        pUsingNamespace = new UsingNamespace(pContext);
        pUsingNamespaceAction = new UsingNamespaceAction(pContext);
        pUsingNamespace->addAction(pUsingNamespaceAction);
        pParser->addRule(pDependency);
        pParser->addRule(pUsingNamespace);
        //pParser->addRule(pMemberImple);
        return pParser;
    }
    catch (std::exception& ex)
    {
        std::cout << "\n\n  " << ex.what() << "\n\n";
        return 0;
    }
}

// return table
ITable<ResultValue>* ConfigParserDependency::getTable()
{
    return resultTable;
}
#ifdef TEST_CONFIGUREPARSERDEPENDENCY

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
        // set file
        std::string fileSpec = "../TypeAnalysis/RuleActionForTypeTable.h";

        std::cout << "\n  TypeTable coming from file: " << fileSpec;
        std::cout << "\n  " << std::string(16 + strlen(argv[i]), '-');
        std::cout << "\n";
        // intialize typeTable

        TypeAnalysis analyzer(fileSpec);
        // get table
        ITable<TypeValue>* table = analyzer();
        // print out
        table->print();
        //Table<TypeValue> _table = *table;
        ConfigParserDependency configure(table);
        Parser* pParser = configure.Build();
        try
        {
            if (pParser)
            {
                std::string file = "test2.txt";
                if (!configure.Attach(file))
                {
                    std::cout << "\n  could not open file " << file << std::endl;
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