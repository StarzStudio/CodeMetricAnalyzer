///////////////////////////////////////////////////////////////////////////////
//  RuleActionForDependency.cpp - implements new parsing rules and actions   //
//  ver 1.1                                                                  //
//  Platform:      Mac Book Pro, OS X EI Captain 10.11                       //
//  Application:   Code Analysis, 687 OOD Project2                           //
//  Author:        Xing Zhou, Syracuse University                            //
//                 Software Engineering                                      //
//                 xzhou118@syr.edu                                          //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University               //
//                 (315) 443-3948, jfawcett@twcny.rr.com                     //
///////////////////////////////////////////////////////////////////////////////

#include "RuleActionForDependency.h"
#include "../TypeAnalysis/TypeAnalysis.h"
#include <fstream>

bool Dependency::haveDoubleColon(ITokCollection*& pTc, std::string name)
{
	size_t len = pTc->find(name);
	// must bigger than 1, otherwise will have out of index issue
	if (1 < len && (*pTc)[len - 1] == "::")
		return true;
	return false;
}
void Dependency::AddOneFileInDependentValue(ResultValue& _ValueResult, File _file)
{
	if (!_ValueResult.find(_file))
		_ValueResult.add(_file);
}

void Dependency::searchNamespace(std::string key)
{
	ITable<ResultValue>& ResultTable = *(pContextForDependency->getResultTable());
	ITable<TypeValue>& TypeTable = *(pContextForDependency->getTypeTable());
	std::string currentFile = pContextForDependency->getCurrentFile();
	ResultValue& ValueResult = *(pContextForDependency->getValueResult());
	TypeValue ValueType = TypeTable[key];
	for (auto _namespace : ValueType.getNamespaceCollection())
	{
		if (pContextForDependency->find(_namespace))
		{
			AddOneFileInDependentValue(ValueResult, ValueType.getFileName());
			ResultTable.changeValue(currentFile, ValueResult);
			break;
		}
	}
}
// detect whether there is using before the token, if so, this token can not be taken into accout
// even if been found in the type table
bool  Dependency::NoUsingBefore(std::string key, ITokCollection*& pTc)
{
	ITokCollection& tc = *pTc;
	size_t len = tc.find(key);
	if (len > 0 && tc[len - 1] == "using")
	{
		return true;
	}
	return false;
}

// detect whether this symbol is member function type
bool  Dependency::memberFunctionType(std::string key)
{
	ITable<TypeValue>& TypeTable = *(pContextForDependency->getTypeTable());
	TypeValue value = TypeTable[key];
	if (value.getType() == "Member Function")
	{
		return true;
	}
	return false;
}
// do preTest, if not meet the requirement, then jump out 
bool Dependency::preTest(std::string key, ITokCollection*& pTc)
{
	ITable<TypeValue>& TypeTable = *(pContextForDependency->getTypeTable());
	std::string currentFile = pContextForDependency->getCurrentFile();
	if (NoUsingBefore(key, pTc))
	{
		return false;
	}
	if (memberFunctionType(key))
	{
		return false;
	}
	TypeValue value = TypeTable[key];
	if (value.getFileName() == currentFile)
	{
		return false;
	}
	return true;
}
void Dependency::doAction(std::string key, ITokCollection*& pTc)
{
	ITable<TypeValue>& TypeTable = *(pContextForDependency->getTypeTable());
	std::string currentFile = pContextForDependency->getCurrentFile();
	ITable<ResultValue>& ResultTable = *(pContextForDependency->getResultTable());
	ResultValue& ValueResult = *(pContextForDependency->getValueResult());

	TypeValue value = TypeTable[key];
	std::vector<std::string> namespaceCollection = value.getNamespaceCollection();
	if (namespaceCollection.size() == 0)
	{
		ValueResult.add(value.getFileName());
		ResultTable.changeValue(currentFile, ValueResult);
	}
	else
	{
		if (haveDoubleColon(pTc, key))
		{
			std::string _namespace = (*pTc)[pTc->find(key) - 2];
			if (value.find(_namespace))
			{
				AddOneFileInDependentValue(ValueResult, value.getFileName());
				ResultTable.changeValue(currentFile, ValueResult);
			}
		}
		else
		{
			searchNamespace(key);
		}
	}
}
bool Dependency::doTest(ITokCollection*& pTc)
{
	ITable<ResultValue>& ResultTable = *(pContextForDependency->getResultTable());
	ITable<TypeValue>& TypeTable = *(pContextForDependency->getTypeTable());
	std::string currentFile = pContextForDependency->getCurrentFile();
	ResultValue& ValueResult = *(pContextForDependency->getValueResult());
	std::vector<std::string> collection = pContextForDependency->getNamespaceCollection();
	pTc->remove("\n");
	for (auto key : TypeTable.getKeyList())
	{
		// if found token in the key list
		if (pTc->find(key) != pTc->length())
		{
			if (!preTest(key, pTc))
			{
				break;
			}
			doAction(key, pTc);
		}
	}
	return true;
}
//----< test stub >--------------------------------------------------

#ifdef TEST_RULEACTIONFORDEPENDENCY

using  namespace Scanner;

int main(int argc, char* argv[])
{
	std::cout << "\n  Testing Rule and action for dependency analysis\n "
		<< std::string(30, '=') << std::endl;

	try
	{
		//std::string fileSpec = "../DependencyAnalysis/test.txt";
		std::string fileSpec = "../DependencyAnalysis/RuleActionForDependency.h";
		std::ifstream in(fileSpec);
		if (!in.good())
		{
			std::cout << "\n  can't open " << fileSpec << "\n\n";
			return 1;
		}

		//// fabricate typeTable
		//std::vector<std::string> namespaceCollection1{ "ns1" };
		//std::vector<std::string> namespaceCollection2{ "ns2" };
		//std::vector<std::string> namespaceCollection3{ "N\\A" };
		//std::string type1 = "class";
		//std::string type2 = "global function";
		//std::string fileName1 = "file1.h";
		//std::string fileName2 = "file2.h";
		//std::string fileName3 = "file3.h";
		//std::string fileName4 = "file4.h";
		//std::string fileName5 = "file5.h";
		//std::string fileName6 = "file6.h";
		//std::string fileName7 = "file7.h";
		//std::string fileName8 = "file8.h";
		//std::string fileName9 = "file9.h";
		//TypeValue typeValue1(type1, namespaceCollection1, fileName1);
		//TypeValue typeValue2(type2, namespaceCollection1, fileName2);
		//TypeValue typeValue3(type1, namespaceCollection2, fileName3);
		//TypeValue typeValue4(type1, namespaceCollection2, fileName4);
		//TypeValue typeValue5(type1, namespaceCollection3, fileName5);
		//TypeValue typeValue6(type1, namespaceCollection2, fileName6);
		//TypeValue typeValue7(type1, namespaceCollection2, fileName7);
		//TypeValue typeValue8(type1, namespaceCollection2, fileName8);
		//TypeValue typeValue9(type1, namespaceCollection2, fileName9);

		//ITable<TypeValue>* pTypeTable = new Table<TypeValue>();
		//ITable<TypeValue>& typetable = *pTypeTable;
		//typetable.addRecord("n1", typeValue1);
		//typetable.addRecord("n2", typeValue2);
		//typetable.addRecord("n3", typeValue3);
		//typetable.addRecord("n4", typeValue4);
		//typetable.addRecord("n5", typeValue5);
		//typetable.addRecord("n6", typeValue6);
		//typetable.addRecord("n7", typeValue7);
		//typetable.addRecord("n8", typeValue8);
		//typetable.addRecord("n9", typeValue9);
		//// typetable.addRecord("n8", typeValue1);  // same type name, different file
		//std::cout << "  TypeTable is:\n";
		//std::cout << "  ---------------\n";
		//typetable.print();

		// fabricate typeTable
		std::vector<std::string> namespaceCollection1{ "ns1" };
		std::vector<std::string> namespaceCollection2{ "ns2" };
		std::vector<std::string> namespaceCollection3{ "N\\A" };
		std::string type1 = "class";
		std::string type2 = "global function";
		std::string fileName1 = "file1.h";
		std::string fileName2 = "file2.h";
		std::string fileName3 = "file3.h";
		std::string fileName4 = "file4.h";
		std::string fileName5 = "file5.h";
		std::string fileName6 = "file6.h";
		std::string fileName7 = "file7.h";
		std::string fileName8 = "file8.h";
		std::string fileName9 = "file9.h";
		TypeValue typeValue1(type1, namespaceCollection3, fileName1);
		TypeValue typeValue2(type1, namespaceCollection3, fileName2);
		TypeValue typeValue3(type1, namespaceCollection3, fileName3);
		TypeValue typeValue4(type1, namespaceCollection3, fileName4);
		TypeValue typeValue5(type1, namespaceCollection3, fileName5);
		TypeValue typeValue6(type1, namespaceCollection2, fileName6);
		TypeValue typeValue7(type1, namespaceCollection2, fileName7);
		TypeValue typeValue8(type1, namespaceCollection2, fileName8);
		TypeValue typeValue9(type1, namespaceCollection2, fileName9);

		ITable<TypeValue>* pTypeTable = new Table<TypeValue>();
		ITable<TypeValue>& typetable = *pTypeTable;
		typetable.addRecord("ContextForDependency", typeValue1);
		typetable.addRecord("Dependency", typeValue2);
		typetable.addRecord("UsingNamespace", typeValue3);
		typetable.addRecord("UsingNamespaceAction", typeValue4);
		//typetable.addRecord("n5", typeValue5);
		//typetable.addRecord("n6", typeValue6);
		//typetable.addRecord("n7", typeValue7);
		//typetable.addRecord("n8", typeValue8);
		//typetable.addRecord("n9", typeValue9);
		// typetable.addRecord("n8", typeValue1);  // same type name, different file
		std::cout << "  TypeTable is:\n";
		std::cout << "  ---------------\n";
		typetable.print();

		// intialize and add action for rule
		ITable<ResultValue>* resultTable = new Table<ResultValue>();
		ContextForDependency* pContext = new ContextForDependency(pTypeTable, resultTable);
		pContext->setCurrentFile("RuleActionForTypeTable.h");
		Dependency* d = new Dependency(pContext);
		UsingNamespace* uc = new UsingNamespace(pContext);
		UsingNamespaceAction* uca = new UsingNamespaceAction(pContext);
		uc->addAction(uca);

		Toker toker;
		toker.attach(&in);
		SemiExp se(&toker);
		Parser parser(&se);

		// add rule for parser
		parser.addRule(d);
		parser.addRule(uc);

		while (se.get())
			parser.parse();
		std::cout << "\n\n";

		resultTable->print();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
	}
}
#endif;