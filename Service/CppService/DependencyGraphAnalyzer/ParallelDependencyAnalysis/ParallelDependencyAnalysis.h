#ifndef PARALLELDEPENDENCY_H
#define PARALLELDEPENDENCY_H
//////////////////////////////////////////////////////////////////////////////
// parallelDependency.h - parallel analyze dependency relationship          //
// ver 1.1                                                                  //
// Language:      C++, Visual Studio 2015                                   //
// Platform:      Mac Book Pro, OS X EI Captain 10.11                       //
// Application:   dependency Analysis, 687 OOD Project3                     //
// Author:        Xing Zhou, Syracuse University, Software Engineering      //
//                xzhou118@syr.edu                                          //
//////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package uses the services of ThreadPool to exective callable object.
* It shall be generic.
*
*   Public interface:
*   void ParallelTypeAnalysis();
*   void ParallelDependencyAnalysis();
*   void mergePartialTypeTable();
*   void mergePartialResults();
*   void analyze();
*   void printBigTypeTable()
*   void printBigDependencyTable()
*   void setPath(Path _path);
*
* Build Process:
* --------------
* Required Files:
*     Parser.h, Parser.cpp,
*     ITable.h, Table.h, Table.cpp,
*     ItokCollection.h, SemiExp.h, SemiExp.cpp,
*     tokenizer.h, tokenizer.cpp
*     RuleActionForDependency.h, RuleActionForDependency.cpp
*     Factory_DependencyAnalysis.h, Factory_DependencyAnalysis.cpp
*     RuleActionForTypeTable.h, RuleActionForTypeTable.cpp
*     Factory_TypeAnalysis.h, Factory_TypeAnalysis.cpp
*     FileSystem.h, FileSystem.cpp
*     FileMgr.h, FileMgr.cpp
*
* Build Command: devenv project3.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.2 : 06 Apr 2016
* - finish dependency pass
* ver 1.1 : 05 Apr 2016
* - finish type pass
* ver 1.0 : 27 Mar 2016
* - first release
*
* Planned Additions and Changes:
* ------------------------------
* -
*/

#include "../Table/ITable.h"
#include "../Table/Table.h"
#include "../TypeAnalysis/TypeAnalysis.h"
#include "../DependencyAnalysis/DependencyAnalysis.h"
#include "../FileMgr/FileMgr/FileMgr.h"
#include "../FileMgr/FileMgr/IFileMgr.h"
#include "../Tasks/Tasks.h"
#include "../BlockingQueue/BlockingQueue.h"

using namespace FileManager;

class doTypeAnalysisFileMgr : public FileMgr
{
    using WorkItem = std::function<void()>;
    using pTypeTable = ITable<TypeValue>*;
public:
    doTypeAnalysisFileMgr(BlockingQueue<pTypeTable>* _externalBQForTypeTable) : externalBQForTypeTable(_externalBQForTypeTable) {}
    virtual void file(const File& f)
    {
        std::cout << "\n  begin to do Type analysis on:";
        std::cout << "\n  " << dir_ + "\\" + f + "\n";
		std::cout << "  working on thread " << std::this_thread::get_id()<<std::endl;
        WorkItem  partialTypeAnalysis = [this, f]()
        {
            TypeAnalysis typeAnalyzer(f);
            pTypeTable tempTable = typeAnalyzer.analyze();
            pTypeTable table = new Table<TypeValue>(*dynamic_cast<Table<TypeValue>*> (tempTable));
            //table->print();
            externalBQForTypeTable->enQ(table);
        };
        task.doWork(partialTypeAnalysis);
    }
    void stop()
    {
        WorkItem  stop = [this]()
        {
            pTypeTable table = new Table<TypeValue>();
            TypeValue value;
            table->addRecord("endOfTypeAnalysis", value);
            //table->print();
            externalBQForTypeTable->enQ(table);
        };
        task.doWork(stop);
    }
    void setPath(std::string _path)
    {
        path_ = _path;
    }
    void setPatterns(std::vector<std::string> _patterns)
    {
        patterns_ = _patterns;
    }
private:
    Dir dir_;
    Tasks task;
    BlockingQueue<pTypeTable>* externalBQForTypeTable;
};

class doDependencyAnalysisFileMgr : public FileMgr
{
    using WorkItem = std::function<void()>;
    using Result = ITable<ResultValue>*;
    using Typetable = ITable<TypeValue>*;
public:
    doDependencyAnalysisFileMgr(Typetable _pTypeTable, BlockingQueue<Result>* _externalBQForDependencyTable) : pTypeTable(_pTypeTable), externalBQForDependencyTable(_externalBQForDependencyTable) {}
    virtual void file(const File& f)
    {
        std::cout << "  begin to do Dependency analysis on:";
        std::cout << "\n  " << dir_ + "\\" + f + "\n";
		std::cout << "\n  working on thread " << std::this_thread::get_id() << std::endl;
        // each lambda creates a unique DependencyAnalyzer
        WorkItem  partialTypeAnalysis = [this, f]()
        {
            DependencyAnalysis DependencyAnalyzer(f, pTypeTable);
            Result tempTable = DependencyAnalyzer.analyze();
            Result table = new Table<ResultValue>(*dynamic_cast<Table<ResultValue>*> (tempTable));
            //table->print();
            externalBQForDependencyTable->enQ(table);
        };
        task.doWork(partialTypeAnalysis);
    }
    virtual void dir(const Dir& d)
    {
        dir_ = d;
    }
    void setPath(std::string _path)
    {
        path_ = _path;
    }
    void setPatterns(std::vector<std::string> _patterns)
    {
        patterns_ = _patterns;
    }
    void stop()
    {
        WorkItem  stop = [this]()
        {
            Result table = new Table<ResultValue>();
            ResultValue value;
            table->addRecord("endOfDependencyAnalysis", value);
            //table->print();
            externalBQForDependencyTable->enQ(table);
        };
        task.doWork(stop);
    }
private:
    Dir dir_;
    Tasks task;
    Typetable pTypeTable;
    BlockingQueue<Result>* externalBQForDependencyTable;
};

class parallelDependency
{
    using WorkItem = std::function<void()>;
    using pTypeTable = ITable<TypeValue>*;
    using pDependencyTable = ITable<ResultValue>*;
    using Path = std::string;
    using Pattern = std::string;
    using Patterns = std::vector<Pattern>;
public:
    parallelDependency();
    parallelDependency(Path _path, Patterns patterns);
    void ParallelTypeAnalysis();
    void ParallelDependencyAnalysis();
    void mergePartialTypeTable();
    void mergePartialResults();
    void analyze();
    void printBigTypeTable()
    {
        std::cout << "  The merged type table is:\n";
		std::cout << "\n  working on thread " << std::this_thread::get_id() << std::endl;
        bigTypeTable.print();
    }
    void printBigDependencyTable()
    {
        std::cout << "  The merged dependency table is:\n";
		std::cout << "\n  working on thread " << std::this_thread::get_id() << std::endl;
        bigDependencyTable.print();
    }
    void setPath(Path _path);
private:
    TypeTableMerge TypeTableMergeEngine;
    DependencyTableMerge DependencyTableMergeEngine;
    Table<TypeValue> bigTypeTable;
    Table<ResultValue> bigDependencyTable;
    BlockingQueue<pTypeTable> externalBQForTypeTable;
    BlockingQueue<pDependencyTable> externalBQForDependencyTable;
    doTypeAnalysisFileMgr *typeFileMgr;
    doDependencyAnalysisFileMgr *dependencyFileMgr;
    Tasks task;
    Path path;
    Patterns patterns_;
    // bool typeAnaStopSignal;
};

#endif