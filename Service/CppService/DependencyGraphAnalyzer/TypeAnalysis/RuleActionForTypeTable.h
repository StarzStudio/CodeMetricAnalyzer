#ifndef RULEACTIONFORTYPETABLE_H
#define RULEACTIONFORTYPETABLE_H
////////////////////////////////////////////////////////////////////////
//  RuleActionForTypeTable.h - declares new parsing rules and actions //
//  ver 1.4                                                           //
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
  This module provides rule to detect all the types in a file, and provides
  corresponding actions to construct a type table

  Note: whatever rule comes first, it must remove all the \n for further test
  Type rule are coupled with beginning and end scope rule: After enter the type scope,
  then we can use begin and end scope rule to detect whether this is global context

  Public Interface:
  =================

  Build Process:
  ==============
  Required files
	- Parser.h, Parser.cpp,
	  ITable.h, Table.h, Table.cpp,
	  ItokCollection.h, SemiExp.h, SemiExp.cpp,
	  tokenizer.h, tokenizer.cpp
	  FileSystem.h, FileSystem.cpp
  Build commands
	- devenv Project3.sln

  Maintenance History:
  ====================
  ver 1.4 : 07 Apr 16
  - detect lambda symbol
  ver 1.3 : 07 Apr 16
  - add rule to detect typedef and using alias into type table
  provide mechanism to avoid alias in the type scope but focus on global scope
  ver 1.2 : 06 Apr 16
  - add rule to detect X::x(){}
  - modify function def action to detect whether key is dupicate before adding new record
  ver 1.1 : 04 Apr 16
  - remove design of putting N/A if namespace collection size is zero
  ver 1.0 : 29 Mar 16
  - first release

  TODO:
  - rule should detect wrapper declaration, global function declaration
  - provide contextForType class to send signal, let all rule class have a pointer to that class
*/

#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "../SemiExp/itokcollection.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../Table/ITable.h"
#include "../Table/Table.h"
#include "../FileMgr/FileMgr/FileSystem.h"

class contextForType
{
	using pTable = ITable<TypeValue>*;
public:
	contextForType(pTable table) :global(true), _countScopeSignal(false), _scopeCount(1), _pTable(table) {}
	// set in the global context signal
	void setGlobal(bool signal)
	{
		global = signal;
	}
	// return true if this is global function
	bool isGlobal()
	{
		return global;
	}
	// collection namepsace of current analyzed file
	void setNamespace(std::string _namespace)
	{
		namespaceCollection.push_back(_namespace);
	}
	// return namespace collection
	std::vector<std::string>& getNamespace()
	{
		return namespaceCollection;
	}
	// store name of current analzed file
	void setCurrentFile(std::string _currentFile)
	{
		currentFile = _currentFile;
	}
	// return name of current file
	const std::string& getCurrentFile()
	{
		return currentFile;
	}
	// set signal to start count scope number
	void setCountScopeSignal(bool signal)
	{
		_countScopeSignal = signal;
	}
	// get # of scope that has been counted
	bool getCountScopeSignal()
	{
		return _countScopeSignal;
	}
	// increase # of scope by 1
	void increaseScopeCount()
	{
		_scopeCount++;
	}
	// decrease # of scope by 1
	void decreaseScopeCount()
	{
		_scopeCount--;
	}
	// return # of scope count
	int getScopeCount()
	{
		return _scopeCount;
	}
	pTable getTable()
	{
		return _pTable;
	}
private:
	std::string currentFile;
	bool global;
	std::vector<std::string> namespaceCollection;
	bool _countScopeSignal;
	int _scopeCount;
	pTable _pTable;
};

///////////////////////////////////////////////////////////////
// rule to detect begin of scope after enter the class scope

class BeginningOfScope : public IRule
{
public:
	BeginningOfScope(contextForType* _pcontextForType)
	{
		pcontextForType = _pcontextForType;
	}
	bool doTest(ITokCollection*& pTc)
	{
		pTc->remove("\n");        // This step is necessary for further test
		if (pTc->find("{") < pTc->length() && pcontextForType->getCountScopeSignal())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
private:
	contextForType* pcontextForType;
};

class IncreaseCount : public IAction
{
public:
	IncreaseCount(contextForType* _pcontextForType)
	{
		pcontextForType = _pcontextForType;
	}
	void doAction(ITokCollection*& pTc)
	{
		pcontextForType->increaseScopeCount();
	}
private:
	contextForType* pcontextForType;
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
	EndOfScope(contextForType* _pcontextForType)
	{
		pcontextForType = _pcontextForType;
	}
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("}") < pTc->length() && pcontextForType->getCountScopeSignal())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
private:
	contextForType* pcontextForType;
};

class DecreaseCount : public IAction
{
public:
	DecreaseCount(contextForType* _pcontextForType)
	{
		pcontextForType = _pcontextForType;
	}
	void doAction(ITokCollection*& pTc)
	{
		pcontextForType->decreaseScopeCount();
		if (pcontextForType->getScopeCount() == 1 && pcontextForType->getCountScopeSignal() == true)
		{
			pcontextForType->setCountScopeSignal(false);
			pcontextForType->setGlobal(true);
		}
	}
private:
	contextForType* pcontextForType;
};

///////////////////////////////////////////////////////////////
// rule to detect global function definitions

class GlobalFunction : public IRule
{
public:
	GlobalFunction(contextForType* _pcontextForType);
	bool isSpecialKeyWord(const std::string& tok);
	bool findDoubleColon(ITokCollection*& pTc);
	bool isMain(ITokCollection*& pTc);
		bool isLambda(ITokCollection*& pTc);
	bool doTest(ITokCollection*& pTc);
private:
	contextForType* pcontextForType;
};

///////////////////////////////////////////////////////////////
// action to make record into type table
class MakeRecordForGlobalFunc : public IAction
{
public:
	MakeRecordForGlobalFunc(contextForType* _pcontextForType)
	{
		pcontextForType = _pcontextForType;
	}
	// return true if this is lambda function
	bool isLambda(ITokCollection*& pTc);
	void doAction(ITokCollection*& pTc);
private:
	contextForType* pcontextForType;
};

////////////////////////////////////////////////////////////////////////////
// rule to detect type such as: struct, class, enum
class Type : public IRule
{
public:
	Type(contextForType* _pcontextForType)
	{
		pcontextForType = _pcontextForType;
	}
	// define special wrapper keyword
	bool isWrapper(const std::string& tok)
	{
		const size_t numKeys = 4;
		const static std::string keys[numKeys] = {
			 "struct", "class", "enum", "union"
		};
		for (int i = 0; i < numKeys; ++i)
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
private:
	contextForType* pcontextForType;
};

///////////////////////////////////////////////////////////////
// action to add type into type table

class MakeRecordForType : public IAction
{
public:
	MakeRecordForType(contextForType* _pcontextForType)
	{
		pcontextForType = _pcontextForType;
	}
	// find out the type of this record
	std::string findType(ITokCollection*& pTc)
	{
		if (pTc->find("union") < pTc->length())
			return "union";
		if (pTc->find("enum") < pTc->length())
			return "enum";
		if (pTc->find("class") < pTc->length())
			return "class";
		if (pTc->find("struct") < pTc->length())
			return "struct";
		return "global function";
	}
	void doAction(ITokCollection*& pTc)
	{
		// start count scope
		pcontextForType->setCountScopeSignal(true);
		// indicate this is in type scope, not global environment
		pcontextForType->setGlobal(false);
		std::string type = findType(pTc);
		std::string name = (*pTc)[pTc->find(type) + 1];
		std::vector<std::string> namespaceCollection;
		if (pcontextForType->getNamespace().size() != 0)
		{
			namespaceCollection = pcontextForType->getNamespace();
		}
		std::string fileName = pcontextForType->getCurrentFile();
		TypeValue value(type, namespaceCollection, fileName);
		pcontextForType->getTable()->addRecord(name, value);
	}
private:
	contextForType* pcontextForType;
};

////////////////////////////////////////////////////////////////////////////
// rule to detect type such as: struct, class, enum
class FindNamspace : public IRule
{
public:
	FindNamspace(contextForType* _pcontextForType)
	{
		pcontextForType = _pcontextForType;
	}
	bool doTest(ITokCollection*& pTc)
	{
		pTc->remove("\n");        // This step is necessary for further test
		ITokCollection& tc = *pTc;
		size_t len = tc.find("{");
		// len must bigger than 1
		// otherwise, else { may cause out of range problem
		if (1 < len && len < tc.length())
		{
			if ((tc[len - 2]) == "namespace")
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
private:
	contextForType* pcontextForType;
};

///////////////////////////////////////////////////////////////
// action to add type into type table

class AddNamespace : public IAction
{
public:
	AddNamespace(contextForType* _pcontextForType)
	{
		pcontextForType = _pcontextForType;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		int len = tc.find("{");
		std::string _namespace = tc[len - 1];
		pcontextForType->setNamespace(_namespace);
	}
private:
	contextForType* pcontextForType;
};

////////////////////////////////////////////////////////////////////////////
// rule to detect type such as: struct, class, enum
class MemberDef : public IRule
{
public:
	MemberDef(contextForType* _pcontextForType)
	{
		pcontextForType = _pcontextForType;
	}
	// find :: before the function name
	// e.g. ::func(){}
	bool findDoubleColon(ITokCollection*& pTc)
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
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			if (len < tc.length() && findDoubleColon(pTc))
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
private:
	contextForType* pcontextForType;
};

///////////////////////////////////////////////////////////////
// action to add type into type table

class MakeRecordForMemberDef : public IAction
{
public:
	MakeRecordForMemberDef(contextForType* _pcontextForType)
	{
		pcontextForType = _pcontextForType;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		std::string type = "Member Function";
		std::string name = (*pTc)[pTc->find("(") - 1];
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
private:
	contextForType* pcontextForType;
};

///////////////////////////////////////////////////////////////
// detect typedef and using
class AliasTypedef : public IRule
{
public:
	AliasTypedef(contextForType* _pcontextForType)
	{
		pcontextForType = _pcontextForType;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc.find("typedef") < tc.length() && pcontextForType->isGlobal())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
private:
	contextForType* pcontextForType;
};

///////////////////////////////////////////////////////////////
// action to add typedef alias

class MakeRecordForAliasTypedef : public IAction
{
public:
	MakeRecordForAliasTypedef(contextForType* _pcontextForType)
	{
		pcontextForType = _pcontextForType;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		std::string name = tc[tc.find("typedef") + 2];
		std::string type = "ClassAlias";
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
private:
	contextForType* pcontextForType;
};

///////////////////////////////////////////////////////////////
// detect typedef and using
class AliasUsing : public IRule
{
public:
	AliasUsing(contextForType* _pcontextForType)
	{
		pcontextForType = _pcontextForType;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc.find("using") < tc.length() && pcontextForType->isGlobal())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
private:
	contextForType* pcontextForType;
};

///////////////////////////////////////////////////////////////
// action to add typedef alias

class MakeRecordForAliasUsing : public IAction
{
public:
	MakeRecordForAliasUsing(contextForType* _pcontextForType)
	{
		pcontextForType = _pcontextForType;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		std::string name = tc[tc.find("using") + 1];
		std::string type = "ClassAlias";
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
private:
	contextForType* pcontextForType;
};


///////////////////////////////////////////////////////////////
// rule to detect function definitions

class GlobalLambda : public IRule
{
public:
	GlobalLambda(contextForType* _pcontextForType)
	{
		pcontextForType = _pcontextForType;
	}
	bool findBrackets(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		size_t first = tc.find("[");
		size_t second = tc.find("]");
		if (first < tc.length() && second < tc.length() && first < second)
			return true;
		return false;
	}
	// detect whether this is operator[] case but not lambda
	bool findNoOperator(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc.find("operator") == tc.length())
			return true;
		return false;
	}
	// detect whether this lambda anonymous, if not, return true
	bool NameExist(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc.find("=") < tc.length())
			return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{") 
		{
			if (!(findNoOperator(pTc) && findBrackets(pTc) && NameExist(pTc)))
				return true;
			if (tc[tc.find("]") + 1] == "(")     // this guarantees the SemiExp is not an arry intialization
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
private:
	contextForType* pcontextForType;
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushLambda : public IAction
{
public:
	PushLambda(contextForType* _pcontextForType)
	{
		pcontextForType = _pcontextForType;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		std::string name = tc[tc.find("=") - 1];
		std::string type = "lambda";
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
private:
	contextForType* pcontextForType;
};
#endif
