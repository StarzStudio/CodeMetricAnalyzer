//////////////////////////////////////////////////////////////////////////////
// parallelDependency.cpp - parallel analyze dependency relationship        //
// ver 1.1                                                                  //
// Language:      C++, Visual Studio 2015                                   //
// Platform:      Mac Book Pro, OS X EI Captain 10.11                       //
// Application:   dependency Analysis, 687 OOD Project3                     //
// Author:        Xing Zhou, Syracuse University, Software Engineering      //
//                xzhou118@syr.edu                                          //
//////////////////////////////////////////////////////////////////////////////

#include "ParallelDependencyAnalysis.h"

// constructor
parallelDependency::parallelDependency()
{
    typeFileMgr = new doTypeAnalysisFileMgr(&externalBQForTypeTable);
    typeFileMgr->setPath(path);
    typeFileMgr->setPatterns(patterns_);
}
parallelDependency::parallelDependency(Path _path, Patterns patterns) :path(_path), patterns_(patterns)
{
    typeFileMgr = new doTypeAnalysisFileMgr(&externalBQForTypeTable);
    typeFileMgr->setPath(path);
    typeFileMgr->setPatterns(patterns_);
}

// detect files meeting pattern given path
// enqueue each file wrapped in a lambda into a extern queue to
// the threadpool
// do Type Analysis for each file, this function may wrapped inside a lambda
// capture fileName as parameter
void parallelDependency::ParallelTypeAnalysis()
{
    WorkItem startTypeFileMgr = [this]()
    {
        typeFileMgr->find(this->path);
        typeFileMgr->stop();
    };
    task.doWork(startTypeFileMgr);
}

// this must begin after all the Type table are done analyzing and merged
void parallelDependency::ParallelDependencyAnalysis()
{
    dependencyFileMgr = new doDependencyAnalysisFileMgr(&bigTypeTable, &externalBQForDependencyTable);
    dependencyFileMgr->setPath(this->path);
    dependencyFileMgr->setPatterns(this->patterns_);
    WorkItem startDepeFileMgr = [this]()
    {
        dependencyFileMgr->find(path);
        dependencyFileMgr->stop();
    };
    task.doWork(startDepeFileMgr);
}

// merge all the partial retults into single result table
void parallelDependency::mergePartialResults()
{
    WorkItem startMergeDependencyTable = [this]() {
        while (true)
        {
            pDependencyTable wi = externalBQForDependencyTable.deQ();
            // if get the table which contains endOfTypeAnalysis as stop signal, then return
            if (wi->find("endOfDependencyAnalysis"))
            {
                delete wi;
                bigDependencyTable = DependencyTableMergeEngine.getMergedTable();

                printBigDependencyTable();
                return;
            }
            DependencyTableMergeEngine.merge(wi);
            delete wi;
        }
    };
    task.doWork(startMergeDependencyTable);
}

// merge all the partial retults into single result table
void parallelDependency::mergePartialTypeTable()
{
    WorkItem startMergeTypeTable = [this]() {
        while (true)
        {
            pTypeTable wi = externalBQForTypeTable.deQ();
            // if get the table which contains endOfTypeAnalysis as stop signal, then return
            if (wi->find("endOfTypeAnalysis"))
            {
                delete wi;
                bigTypeTable = TypeTableMergeEngine.getMergedTable();
                printBigTypeTable();
                //std::cout << "  Mission Complete\n";
                //////////////
                // start dependency analysis area
                //////
                ParallelDependencyAnalysis();
                mergePartialResults();
                return;
            }
            TypeTableMergeEngine.merge(wi);
            delete wi;
        }
    };
    task.doWork(startMergeTypeTable);
}

void parallelDependency::analyze()
{
    ParallelTypeAnalysis();
    mergePartialTypeTable();
}

void parallelDependency::setPath(Path _path)
{
    path = _path;
}

//----< test stub >--------------------------------------------------

#ifdef TEST_PARALLELDEPENDENCYANALYSIS

#include "../Utilities/Utilities.h"
#define TEST1
#define zTEST2

int main()
{
    using Util = Utilities::StringHelper;
    using WorkItem = std::function<void()>;

#ifdef TEST1
    std::vector<std::string> patterns{ "*.h" , "*.cpp" };
    std::string path = "..";
    parallelDependency pd(path, patterns);
    pd.analyze();

    int a;
    std::cin >> a;
#endif
    //TypeAnalysis ta("../TypeAnalysis/TypeAnalysis.h");
    //ITable<TypeValue>* table = ta.analyze();
    //table->print();

#ifdef TEST2
    std::cout << "\n  main thread id = " << std::this_thread::get_id();

    Tasks  task(4);
    using Result = ITable<TypeValue>*;
    BlockingQueue<Result> externalBQForTypeTable;
    doTypeAnalysisFileMgr dtaf(&externalBQForTypeTable);

    // constructor type table
    TypeValue typeValue1;
    std::vector<std::string> namespaceCollection{ "namespaceTest1" };
    std::string type = "global function";
    std::string fileName = "firstFile.h";
    TypeValue typeValue2(type, namespaceCollection, fileName);

    Table<TypeValue> typeTable;
    typeTable.addRecord("gfunc1", typeValue1);
    Table<TypeValue> typeTable2;
    typeTable2.addRecord("gfunc2", typeValue1);
    Table<TypeValue> typeTable3;
    typeTable3.addRecord("gfunc3", typeValue1);
    Table<TypeValue> typeTable4;
    typeTable4.addRecord("gfunc4", typeValue1);
    Table<TypeValue> typeTable5;
    typeTable5.addRecord("gfunc5", typeValue1);
    std::vector<Table<TypeValue>> tableCollection;
    tableCollection.push_back(typeTable);
    tableCollection.push_back(typeTable2);
    tableCollection.push_back(typeTable3);
    tableCollection.push_back(typeTable4);
    tableCollection.push_back(typeTable5);
    //typeTable.print();

    // define 1st work item, enqueue table

    //WorkItem wi1 = [&tableCollection, &externalBQForTypeTable]() {
    //    for (Table<TypeValue> t : tableCollection)
    //    {
    //        std::cout << "\n  working on thread " << std::this_thread::get_id() <<std::endl;
    //        Result table = new Table<TypeValue>(t);
    //        /*std::cout << "  current table is: \n";
    //        table->print();*/
    //        externalBQForTypeTable.enQ(table);
    //    }
    //};

    //task.doWork(wi1);

    std::vector<std::string> fileCollection = { "..\\TypeAnalysis\\Factory_TypeAnalysis.h",
                                               "..\\TypeAnalysis\\RuleActionForTypeTable.h",
                                               "..\\TypeAnalysis\\Parser.h",
                                               "..\\TypeAnalysis\\TypeAnalysis.h" };
    WorkItem wi1 = [&fileCollection, &externalBQForTypeTable]() {
        for (auto f : fileCollection)
        {
            std::cout << "\n  working on thread " << std::this_thread::get_id() << std::endl;
            TypeAnalysis ta(f);
            Result tempTable = ta.analyze();
            Result table = new Table<TypeValue>(*dynamic_cast<Table<TypeValue>*> (tempTable));
            //std::cout << "  current table is: \n";
            //table->print();
            externalBQForTypeTable.enQ(table);
        }
    };

    task.doWork(wi1);
    //define 2nd work item, merge table
    Table<TypeValue> bigTypeTable;

    WorkItem wi2 = [&bigTypeTable, &externalBQForTypeTable]()
    {
        while (true)
        {
            std::cout << "\n  working on thread " << std::this_thread::get_id() << std::endl;
            Result pTable = externalBQForTypeTable.deQ();
            bigTypeTable.mergeTables(pTable);
            std::cout << "  big table is: \n";
            bigTypeTable.print();
            delete pTable;
        }
    };

    task.doWork(wi2);
    int a;
    std::cin >> a;
#endif
}
#endif