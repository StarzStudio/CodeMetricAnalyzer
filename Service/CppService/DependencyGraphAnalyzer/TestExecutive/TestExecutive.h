#ifndef TESTEXECUTIVE_H
#define TESTEXECUTIVE_H
///////////////////////////////////////////////////////////////////////////
// TestExecutive.h - executive                                           //
// ver 1.0                                                               //
// Language:      C++, Visual Studio 2015                                //
// Platform:      Mac Book Pro, OS X EI Captain 10.11                    //
// Application:   dependency Analysis, 687 OOD Project3                  //
// Author:        Xing Zhou, Syracuse University, Software Engineering   //
//                xzhou118@syr.edu                                       //
///////////////////////////////////////////////////////////////////////////


#include "../DependencyAnalysis/DependencyAnalysis.h"
#include "../FileMgr/FileMgr.h"
#include "../FileMgr/IFileMgr.h"
//#include "../noSQLDBAndPersist/Persist/Persist.h"

using namespace FileManager;

class typeSpeciFileMgr : public FileMgr
{
	using pTable = ITable<TypeValue>*;
public:
	typeSpeciFileMgr(TypeTableMerge* _TypeTableMergeEngine) :TypeTableMergeEngine(_TypeTableMergeEngine) {}

	virtual void file(const File& f)
	{
		std::cout << "\n  " << dir_ + "\\" + f + "\n";
		TypeAnalysis typeAnalyzer(f);
		pTable typetable = typeAnalyzer.analyze();
		//typetable->print();
		TypeTableMergeEngine->merge(typetable);
	}

	virtual void dir(const Dir& d)
	{
		dir_ = d;
	}
private:
	Dir dir_;
	TypeTableMerge* TypeTableMergeEngine;
};

class depenSpeciFileMgr : public FileMgr
{
	using pTypeTable = ITable<TypeValue>*;
	using pDepenTable = ITable<ResultValue>*;
public:
	depenSpeciFileMgr(DependencyTableMerge* _DependencyTableMergeEngine, pTypeTable _bigTable) :
		typeTable(_bigTable), DependencyTableMergeEngine(_DependencyTableMergeEngine){}

	virtual void file(const File& f)
	{
		std::cout << "\n  " << dir_ + "\\" + f + "\n";
		DependencyAnalysis da(f, typeTable);
		pDepenTable depenTable = da.analyze();
		DependencyTableMergeEngine->merge(depenTable);
		//depenTable->print();
		//bigTable->mergeTables(typetable);
	}

	virtual void dir(const Dir& d)
	{
		dir_ = d;
	}
private:
	Dir dir_;
	pTypeTable typeTable;
	DependencyTableMerge* DependencyTableMergeEngine;
};


class SingleThreadExecutive {
public:
	SingleThreadExecutive(std::string _path, std::vector<std::string> _patterns) : path(_path), patterns(_patterns) {}
	void start() {
		// get the big type table containing all the types in the test project 
		Table<TypeValue> bigTypeTable;
		TypeTableMerge typeTableMergeEngine;
		typeSpeciFileMgr tsfm(&typeTableMergeEngine);
		for (auto pattern : patterns) {
			tsfm.addPattern(pattern);
		}
		tsfm.find(path);
		bigTypeTable = typeTableMergeEngine.getMergedTable();
		bigTypeTable.print();
		
		// based on aforementioned type table, get the dependency table.
		Table<ResultValue> bigDepenTable;
		DependencyTableMerge dependencyTableMergeEngine;
		depenSpeciFileMgr dsfm(&dependencyTableMergeEngine, &bigTypeTable);
		for (auto pattern : patterns) {
			dsfm.addPattern(pattern);
		}
		dsfm.find(path);
		bigDepenTable = dependencyTableMergeEngine.getMergedTable();
		bigDepenTable.print();

		//tableToNosqlDB(bigDepenTable);

	}

	// void tableToNosqlDB(ITable<ResultValue>& depenTable) {
	// 	using namespace NoSQLDB;
	// 	using Data = std::string;

	// 	NoSqlDb<Data> db;
	// 	std::vector<std::string> key_vec(depenTable.getKeyList());
	// 	for (auto key : key_vec) {
	// 		ResultValue rv = depenTable[key];
	// 		Element<std::string> elem;
	// 		for (auto v : rv.get())
	// 		{
	// 			elem.saveChild(v);
	// 		}
	// 		db.saveRecord(key, elem);
	// 	}
	// 	Persist<Data> persist(db);
	// 	persist.saveToFile("dependency.txt");
	// }

private:
	std::string path;
	std::vector<std::string> patterns;
};

#endif