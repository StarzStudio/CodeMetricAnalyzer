///////////////////////////////////////////////////////////////////////////////
//  RuleActionForTypeTable.cpp - implements new parsing rules and actions    //
//  ver 1.4                                                                  //
//  Platform:      Mac Book Pro, OS X EI Captain 10.11                       //
//  Application:   Code Analysis, 687 OOD Project2                           //
//  Author:        Xing Zhou, Syracuse University                            //
//                 Software Engineering                                      //
//                 xzhou118@syr.edu                                          //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University               //
//                 (315) 443-3948, jfawcett@twcny.rr.com                     //
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "RuleActionForTypeTable.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include <fstream>

GlobalFunction::GlobalFunction(contextForType* _pcontextForType) : pcontextForType(_pcontextForType) {}

// find :: before the function name
// e.g. ::func(){}
bool GlobalFunction::findDoubleColon(ITokCollection*& pTc)
{
    ITokCollection& tc = *pTc;
    if (tc[tc.length() - 1] == "{")
    {
        size_t len = tc.find("(");
        // must bigger than 2 to meet X::x() {
        // if not, it may x(){, which may cause out of range
        if (2 < len && len < tc.length())
        {
            if (tc[len - 2] == "::")
            {
                return true;
            }
        }
    }
    return false;
}
// detect whether special key words like below
bool GlobalFunction::isSpecialKeyWord(const std::string& tok)
{
    const static std::string keys[]
        = { "for", "while", "switch", "if", "catch" };
    for (int i = 0; i < 5; ++i)
        if (tok == keys[i])
            return true;
    return false;
}
// detect whether this function is lambda, return true if is
bool GlobalFunction::isLambda(ITokCollection*& pTc)
{
	ITokCollection& tc = *pTc;
	if (tc.find("operator") < tc.length())
		return false;
	if (tc.find("[") < tc.length() && tc.find("]") < tc.length())  // has [ or ]
	{
		// this case:arrary is parameter ( []), doesn't count
		if (tc.find("(") < tc.find("[") && tc.find(")") > tc.find("]"))
			return false;
		return true;
	}
	return false;

}

// detect whether this function is main()
bool GlobalFunction::isMain(ITokCollection*& pTc)
{
	ITokCollection& tc = *pTc;
	if (tc.find("main") < tc.length())
		return true;
	return false;
}


// begin test
bool GlobalFunction::doTest(ITokCollection*& pTc)
{
    ITokCollection& tc = *pTc;
    if (tc[tc.length() - 1] == "{")
    {
        size_t len = tc.find("(");
        if (len < tc.length() &&
            !isSpecialKeyWord(tc[len - 1]) &&
            !findDoubleColon(pTc) &&  // distinguish between global function and X::x
			!isLambda(pTc) &&
			!isMain(pTc))  
        {
            doActions(pTc);
            return true;
        }
    }
    return true;
}

bool MakeRecordForGlobalFunc::isLambda(ITokCollection*& pTc)
{
    ITokCollection& tc = *pTc;
    if (tc.find("operator") < tc.length())
        return false;
    if (tc.find("[") < tc.length() && tc.find("]") < tc.length())  // has [ or ]
    {
        // this case:arrary is parameter ( []), doesn't count
        if (tc.find("(") < tc.find("[") && tc.find(")") > tc.find("]"))
            return false;
        return true;
    }
    return false;
}
// action to add function record
void MakeRecordForGlobalFunc::doAction(ITokCollection*& pTc)
{
    std::string name = (*pTc)[pTc->find("(") - 1];
    if (isLambda(pTc))                        // change nane if this is lambda
        name = (*pTc)[pTc->find("=") - 1];
    else if (pTc->find("operator") < pTc->length())  // detect operator type
    {
        name = "operator";
        size_t i = pTc->find("operator") + 1;
        size_t parenPosi = pTc->find("(");
        // update the last occurance of (
        for (size_t i = parenPosi + 1; i < pTc->length(); i++)
        {
            if ((*pTc)[i] == "(")
                parenPosi = i;
        }
        // add string between operator and ( to the name
        for (; i < parenPosi; i++)
        {
            name += (*pTc)[i];
        }
    }

    std::string type;
    if (pcontextForType->isGlobal())
        type = "Global Function";
    else
        type = "Member Function";
    std::vector<std::string> namespaceCollection;
    if (pcontextForType->getNamespace().size() != 0)
    {
        namespaceCollection = pcontextForType->getNamespace();
    }
    std::string fileName = pcontextForType->getCurrentFile();
    TypeValue value(type, namespaceCollection, fileName);
    if (!pcontextForType->getTable()->find(name))
    {
        pcontextForType->getTable()->addRecord(name, value);
    }
}

//----< test stub >--------------------------------------------------

#ifdef TEST_RULEACTIONFORTYPETABLE

using  namespace Scanner;

int main(int argc, char* argv[])
{
    std::cout << "\n  Testing Rule and action class for construct type table\n "
        << std::string(30, '=') << std::endl;

    try
    {
        // std::string fileSpec = "test.txt";
        std::string fileSpec = "RuleActionForTypeTable.h";
        // std::string fileSpec = "RuleActionForTypeTable.cpp";

        std::ifstream in(fileSpec);
        if (!in.good())
        {
            std::cout << "\n  can't open " << fileSpec << "\n\n";
            return 1;
        }

        // intialize and add action for rule
        ITable<TypeValue>* typetable = new Table<TypeValue>();
        contextForType* pContext = new contextForType(typetable);
        pContext->setCurrentFile(fileSpec);
        BeginningOfScope bos(pContext);
        IncreaseCount ic(pContext);
        bos.addAction(&ic);
        EndOfScope eos(pContext);
        DecreaseCount dc(pContext);
        eos.addAction(&dc);
        GlobalFunction gf(pContext);
        MakeRecordForGlobalFunc mrfgf(pContext);
        gf.addAction(&mrfgf);
        Type t(pContext);
        MakeRecordForType mrft(pContext);
        t.addAction(&mrft);
        FindNamspace fn(pContext);
        AddNamespace an(pContext);
        fn.addAction(&an);
        MemberDef md(pContext);
        MakeRecordForMemberDef mrfmd(pContext);
        md.addAction(&mrfmd);

        Toker toker;
        toker.attach(&in);
        SemiExp se(&toker);
        Parser parser(&se);

        // add rule for parser
        parser.addRule(&fn);
        parser.addRule(&md);
        parser.addRule(&gf);
        parser.addRule(&t);
        parser.addRule(&bos);
        parser.addRule(&eos);

        while (se.get())
            parser.parse();
        std::cout << "\n\n";

        typetable->print();
    }
    catch (std::exception& ex)
    {
        std::cout << "\n\n  " << ex.what() << "\n\n";
    }
}
#endif