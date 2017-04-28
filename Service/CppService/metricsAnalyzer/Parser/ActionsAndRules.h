#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
////////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions        //
//  ver 2.9                                                           //
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
  This module defines several action classes.  Its classes provide 
  specialized services needed for specific applications.  The modules
  Parser, SemiExpression, and Tokenizer, are intended to be reusable
  without change.  This module provides a place to put extensions of
  these facilities and is not expected to be reusable. 

  Public Interface:
  =================
  Toker t(someFile);              // create tokenizer instance
  SemiExp se(&t);                 // create a SemiExp attached to tokenizer
  Parser parser(se);              // now we have a parser
  Rule1 r1;                       // create instance of a derived Rule class
  Action1 a1;                     // create a derived action
  r1.addAction(&a1);              // register action with the rule
  parser.addRule(&r1);            // register rule with parser
  while(se.getSemiExp())          // get semi-expression
    parser.parse();               //   and parse it

  Build Process:
  ==============
  Required files
    - Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp, 
      ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
      ItokCollection.h, SemiExpression.h, SemiExpression.cpp, 
      tokenizer.h, tokenizer.cpp, ASTNode.h, ASTNode.cpp
  Build commands (either one)
    - devenv Project2.sln
    - cl /EHsc /DTEST_PARSER parser.cpp ActionsAndRules.cpp \
         semiexpression.cpp tokenizer.cpp /link setargv.obj

  Maintenance History:
  ====================
  ver 2.9 : 11 Mar 16
  - delete lambdaScope rule
  - change FunctionRule to ignore lambda as function
  - modify the WrapScopeRule class, change its acitons, add keywords list
  add new keyword enum, union
  - include try and finally {} in WrapScopeRule class 
  ver 2.8 : 07 Mar 16
  - add function in PushFunction class to deal with operator(){} case
  ver 2.7 : 05 Mar 16
  - change the Handlepop action to add endlinecount for the first elem in the
  scopestack everytime the node at the top pops, thus in the end of the file global 
  node will have the right end line.
  ver 2.6 : 04 Mar 16
  - delete proprocessor and declaration and executable rule and corresponding 
  actions 
  ver 2.5 : 03 Mar 16
  - add new Rule class: LocalScope, Wrapper Scope to detect "if,while, do-while
  for" and "namespce, class, struct" case.
  - provide push action for both of the above classes
  - change stackScope's element type from ASTNode to ASTNode*
  - change the doTest return type to false if this test works and do the actions
  and this will stop Parser will any further rule test.
  ver 2.4 : 02 Mar 16
  - provide mechanism to add complexity for all the nodes in the stack
  when pushing a new node into the stack, and link this new node as children
  of the node behind ot it in the stack
  ver 2.3 : 01 Mar 16
  - remove ASTNode class from this package and provide a separate package to 
  hold ASTNode class
  ver 2.2 : 28 Feb 16
  - change Struct Element into ASTNode Class, devide linecount into start count 
  and end count
  - made all ASTNode Class variable private, provide interface to access them.
  ver 2.1 : 15 Feb 16
  - small functional change to a few of the actions changes display strategy
  - preface the (new) Toker and SemiExp with Scanner namespace
  ver 2.0 : 01 Jun 11
  - added processing on way to building strong code analyzer
  ver 1.1 : 17 Jan 09
  - changed to accept a pointer to interfaced ITokCollection instead
    of a SemiExpression
  ver 1.0 : 12 Jan 06
  - first release

*/
//
#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "../SemiExp/itokcollection.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../ASTNode/ASTNode.h"


///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.
/*
 * ToDo:
 * - add AST Node class 
 * - provide field to hold root of AST
 * - provide interface to access AST
 * - provide helper functions to build AST, perhaps in Tree class
 */
class Repository  // application specific
{
public:
  Repository(Scanner::Toker* pToker) 
  {
    p_Toker = pToker;
    // initialize AST's root : namespace global, add push it into scopestack
    ASTNode* root = new ASTNode("global", "namespace");
    tree = new ASTTree(root);
    root->setStartLine(2);    // the line will be 2 -1 = 1
    stack.push(root);
  }
  ScopeStack<ASTNode*>& scopeStack()
  {
    return stack;
  }
  Scanner::Toker* Toker()
  {
    return p_Toker;
  }
  size_t lineCount()
  {
    return (size_t)(p_Toker->currentLineCount());
  }
  // interface to the AST
  ASTTree* getASTTree()
  {
      return tree;
  }
private:
    ScopeStack<ASTNode*> stack;
    Scanner::Toker* p_Toker;
    ASTTree* tree ;
};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    pTc->remove("\n");        // This step is necessary for further test
    if(pTc->find("{") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction
{
  Repository* p_Repos;
public:
  HandlePush(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    //std::cout << "\n--BeginningOfScope rule";
    ASTNode* elem = new ASTNode();

    elem->setStartLine(p_Repos->lineCount());  
    elem->setComplexity(0);
    if (p_Repos->scopeStack().size() != 0)
    {
        ASTNode* father = p_Repos->scopeStack().top();  // get the father of this elem
        father->addChildren(elem);                      // link elem to father Node's children
        ScopeStack<ASTNode*>::iterator i = p_Repos->scopeStack().begin();
        for (; i != p_Repos->scopeStack().end(); i++)
                (*i)->addOneComplexity();
    }
    p_Repos->scopeStack().push(elem);
  }
};


///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("}") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
  Repository* p_Repos;
public:
  HandlePop(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    //std::cout << "\n--EndOfScope rule";
    if(p_Repos->scopeStack().size() == 0)
      return;
    ASTNode* elem = p_Repos->scopeStack().pop();
    elem->setEndLine(p_Repos->lineCount());

    // add the same end line to the global namespace node
    // every time the top node pops,
    // thus global node will have the end line of the file
    // when parser finishes
    ASTNode* bottomElem = *(p_Repos->scopeStack().begin());
    bottomElem ->setEndLine(p_Repos->lineCount());
  }
};


///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for(int i=0; i<5; ++i)
      if(tok == keys[i])
        return true;
    return false;
  }
  bool isLambda(ITokCollection*& pTc)
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
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if(tc[tc.length()-1] == "{")
    {
      size_t len = tc.find("(");
      if(len < tc.length() && !isSpecialKeyWord(tc[len-1])&& !isLambda(pTc))
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack
class PushFunction : public IAction
{
  Repository* p_Repos;
public:
  PushFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    // pop anonymous scope
     p_Repos->scopeStack().pop();
    // delete last child of the parent node
     p_Repos->scopeStack().top()->deleteLastChildren();

    // push function scope
     std::string name = (*pTc)[pTc->find("(") - 1];

    // detect whether is operator function
    if (pTc->find("operator") < pTc->length())
    {
        name = "operator";
        size_t i = pTc->find("operator") + 1;
        for (; i < pTc->find("("); i++)
        {
            name += (*pTc)[i];
        }
    }
    ASTNode* elem = new ASTNode(name,"function");
    elem->setStartLine(p_Repos->lineCount());
    // link this new node to the parent node
    ASTNode* father = p_Repos->scopeStack().top();  // get the father of this elem
    father->addChildren(elem);
    // push new node into stack
    p_Repos->scopeStack().push(elem);
  }
};
    
////////////////////////////////////////////////////////////////////////////
// rule to detect condition scope: for, if, while, do-while, swith
class CondScope : public IRule
{
public:
    bool isSpecialKeyWord(const std::string& tok)
    {
        const static std::string keys[]
            = { "for", "while", "switch", "if", "catch" };
        for (int i = 0; i<5; ++i)
            if (tok == keys[i])
                return true;
        return false;
    }
    bool doTest(ITokCollection*& pTc)
    {
        ITokCollection& tc = *pTc;
        if (tc[tc.length() - 1] == "{")
        {
            size_t len = tc.find("(");
            if (len < tc.length() && isSpecialKeyWord(tc[len - 1]))
            {
                doActions(pTc);
                return true;
            }
        }
        return true;
    }
};

///////////////////////////////////////////////////////////////
// action to push condition keyword scope node onto ScopeStack

class PushCondScope : public IAction
{
    Repository* p_Repos;
public:
    PushCondScope(Repository* pRepos)
    {
        p_Repos = pRepos;
    }
    void doAction(ITokCollection*& pTc)
    {
        // next statement is now  handled in PrintFunction
        // std::cout << "\n  FunctionDef: " << pTc->show();

        // pop anonymous scope
        p_Repos->scopeStack().pop();
        // delete last child of the parent node
        p_Repos->scopeStack().top()->deleteLastChildren();

        // push local scope
        std::string name = (*pTc)[pTc->find("(") - 1];
        ASTNode* elem = new ASTNode(name, "Condition");
        elem->setStartLine(p_Repos->lineCount());

        // link this new node to the parent node
        ASTNode* father = p_Repos->scopeStack().top();  // get the father of this elem
        father->addChildren(elem);
        // push new node into stack
        p_Repos->scopeStack().push(elem);

    }
};

////////////////////////////////////////////////////////////////////////////
// rule to detect Wrapper such as: namespace, struct, class scope
class WrapperScope : public IRule
{
public:
    // define special wrapper keyword
    bool isWrapper(const std::string& tok) 
    {                                      
        const size_t numKeys = 7;           
        const static std::string keys[numKeys] = {
            "namespace", "struct", "class", "enum", "union", "try", "finally"
        };
        for (int i = 0; i<numKeys; ++i)
            if (tok == keys[i])
                return true;
        return false;
    }
    
    bool doTest(ITokCollection*& pTc)
    {
        ITokCollection& tc = *pTc;
        if (tc.find("{") < tc.length())
        {
            for (size_t i = 0; i < tc.length(); ++i)
            {
                if (isWrapper(tc[i]))
                {
                    doActions(pTc);
                    return true;
                }
            }
        }
        return true;
    }
};

///////////////////////////////////////////////////////////////
// action to push wrapper scope node onto ScopeStack

class PushWrapperScope : public IAction
{
    Repository* p_Repos;
public:
    PushWrapperScope(Repository* pRepos)
    {
        p_Repos = pRepos;
    }
    int findType(ITokCollection*& pTc)
    {
        if (pTc->find("try") < pTc->length())
            return 1;
        if (pTc->find("finally") < pTc->length())
            return 1;
        if (pTc->find("namespace") < pTc->length())
            return 2;
        if (pTc->find("union") < pTc->length())
            return 2;
        if (pTc->find("enum") < pTc->length())
            return 2;
        if (pTc->find("class") < pTc->length())
            return 3;
        if (pTc->find("struct") < pTc->length())
            return 4;
        return 0;
    }
    void doAction(ITokCollection*& pTc)
    {
        p_Repos->scopeStack().pop();                           // pop anonymous scope 
        p_Repos->scopeStack().top()->deleteLastChildren();   // delete last child of the parent node
        ASTNode* elem = new ASTNode();
        switch (findType(pTc))
        {
            case 0:
                break;
            case 1:    // the namespace, union, enum case:
            {
                std::string type = (*pTc)[pTc->find("{") - 1];
                elem->setName("no_name");
                elem->setType(type);
                break;
            }
            case 2:      // the namespace, union, enum case:
            {
                std::string type = (*pTc)[pTc->find("{") - 2];
                std::string name = (*pTc)[pTc->find("{") - 1];
                elem->setName(name);
                elem->setType(type);
                break;
            }
            case 3:    // deal with class  case:
            {
                size_t classPostion = pTc->find("class");
                elem->setName((*pTc)[classPostion + 1]);
                elem->setType("class");
                break;
            }
            case 4:    // deal with struct  case:
            {
                size_t struPosition = pTc->find("struct");
                elem->setName((*pTc)[struPosition + 1]);
                elem->setType("class");
                break;
            }
        }
        elem->setStartLine(p_Repos->lineCount());       // link this new node to the parent node
        ASTNode* father = p_Repos->scopeStack().top();  // get the father of this elem
        father->addChildren(elem);
        p_Repos->scopeStack().push(elem);              // push new node into stack
    }
};

////////////////////////////////////////////////////////////////////////////
// rule to detect Initialize such as: namespace, struct, class scope
class InitializerScope : public IRule
{
public:
    // define special wrapper keyword
    bool isSpecialKeywords(const std::string& tok)
    {
        const size_t numKeys = 7;
        const static std::string keys[numKeys] = {
            "namespace", "struct", "class", "enum", "union", "try", "finally"
        };
        for (int i = 0; i<numKeys; ++i)
            if (tok == keys[i])
                return true;
        return false;
    }
    bool doTest(ITokCollection*& pTc)
    {
        if (pTc->find("{") < pTc->length())
        {
            ITokCollection& tc = *pTc;
            // detect special keywords
            for (size_t i = 0; i < tc.length(); ++i)
            {
                if (isSpecialKeywords(tc[i]))
                    return true;
            }
            // detect whether is empty scope
            size_t len = pTc->find("{");
            if (tc[len - 1] != ";" && tc[len - 1] != ")")         // if ¡®;¡¯ and ')'is not before ¡®{¡¯£¬
            {                                                     // ensure it's not empty or function and condition scope
                doActions(pTc);
                return true;
            }
        }
        return true;
    }
};

///////////////////////////////////////////////////////////////
// action to push intializer scope node onto ScopeStack

class PushInitializerScope : public IAction
{
    Repository* p_Repos;
public:
    PushInitializerScope(Repository* pRepos)
    {
        p_Repos = pRepos;
    }
    void doAction(ITokCollection*& pTc)
    {
      
        // pop anonymous scope
        p_Repos->scopeStack().pop();
        // delete last child of the parent node
        p_Repos->scopeStack().top()->deleteLastChildren();

        // push  wrapper scope
        ASTNode* elem = new ASTNode("no_name", "initializer");
        elem->setStartLine(p_Repos->lineCount());

        // link this new node to the parent node
        ASTNode* father = p_Repos->scopeStack().top();  // get the father of this elem
        father->addChildren(elem);
        // push new node into stack
        p_Repos->scopeStack().push(elem);
    }
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions

class LambdaDefinition : public IRule
{
public:
    bool findBrackets(ITokCollection*& pTc)
    {
        ITokCollection& tc = *pTc;
        size_t first = tc.find("[");
        size_t second = tc.find("]");
        if (first < tc.length() &&  second < tc.length()&& first < second)
            return true;
        return false;
    }
    bool NoOperator(ITokCollection*& pTc)
    {
        ITokCollection& tc = *pTc; 
        size_t length = tc.find("operator");
        if (length  == tc.length())
            return true;
        return false;
    }
    bool doTest(ITokCollection*& pTc)
    {
        ITokCollection& tc = *pTc;
        if (tc[tc.length() - 1] == "{")
        {
            if (!(NoOperator(pTc) && findBrackets(pTc)))
                return true;
            if (tc[tc.find("]")+1] == "(")     // this guarantees the SemiExp is not an arry intialization
            {
                doActions(pTc);
                return true;
            }
        }
        return true;
    }
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushLambda : public IAction
{
    Repository* p_Repos;
public:
    PushLambda(Repository* pRepos)
    {
        p_Repos = pRepos;
    }
    void doAction(ITokCollection*& pTc)
    {
        // pop anonymous scope
        p_Repos->scopeStack().pop();
        // delete last child of the parent node
        p_Repos->scopeStack().top()->deleteLastChildren();

        // push lambda scope
        ASTNode* elem = new ASTNode("no_name", "lambda");
        elem->setStartLine(p_Repos->lineCount());
        // link this new node to the parent node
        ASTNode* father = p_Repos->scopeStack().top();  // get the father of this elem
        father->addChildren(elem);
        // push new node into stack
        p_Repos->scopeStack().push(elem);
    }
};

#endif
