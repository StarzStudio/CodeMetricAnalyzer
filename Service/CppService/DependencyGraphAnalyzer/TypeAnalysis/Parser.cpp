////////////////////////////////////////////////////////////////////////
//  Parser.cpp - Analyzes C++ language constructs                     //
//  ver 1.4                                                           //
//  Platform:      Mac Book Pro, OS X EI Captain 10.11                //
//  Application:   Code Analysis, 687 OOD Project2                    //
//  Author:        Xing Zhou, Syracuse University                     //
//                 Software Engineering                               //
//                 xzhou118@syr.edu                                   //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University        //
//                 (315) 443-3948, jfawcett@twcny.rr.com              //
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "Parser.h"
#include "RuleActionForTypeTable.h"
#include "Factory_TypeAnalysis.h"

using namespace Scanner;
using namespace Utilities;

//----< register parsing rule >--------------------------------

void Parser::addRule(IRule* pRule)
{
    rules.push_back(pRule);
}
//----< get next ITokCollection >------------------------------

bool Parser::next()
{
    // default operation of doFold(pTokColl) is to simply return
    // - code analysis provides specific rules for handling
    //   for statements and operator expressions which are
    //   bound in ConfigureParser.cpp

    bool succeeded = pTokColl->get();
    if (!succeeded)
        return false;
    return true;
}

//----< parse the SemiExp by applying all rules to it >--------

bool Parser::parse()
{
    bool succeeded = false;
    for (size_t i = 0; i < rules.size(); ++i)
    {
        // if doTest return false, then stop any further checking
        if (rules[i]->doTest(pTokColl))
            succeeded = true;
        else break;
    }
    return succeeded;
}
//----< register action with a rule >--------------------------

void IRule::addAction(IAction *pAction)
{
    actions.push_back(pAction);
}
//----< invoke all actions associated with a rule >------------

void IRule::doActions(ITokCollection*& pTokColl)
{
    if (actions.size() > 0)
        for (size_t i = 0; i < actions.size(); ++i)
            actions[i]->doAction(pTokColl);
}

//----< test stub >--------------------------------------------

#ifdef TEST_PARSER

#include <queue>
#include <string>
#define Util StringHelper

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