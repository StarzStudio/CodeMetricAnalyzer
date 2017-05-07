//////////////////////////////////////////////////////////////////////////////
// DependencyAnalysis.cpp - analyze dependency relationship between files   //
// ver 1.0                                                                  //
// Language:      C++, Visual Studio 2015                                   //
// Platform:      Mac Book Pro, OS X EI Captain 10.11                       //
// Application:   dependency Analysis, 687 OOD Project3                     //
// Author:        Xing Zhou, Syracuse University, Software Engineering      //
//                xzhou118@syr.edu                                          //
//////////////////////////////////////////////////////////////////////////////

#include "DependencyAnalysis.h"
#include "../FileMgr/FileMgr.h"
#include "../FileMgr/IFileMgr.h"

ITable<ResultValue>* DependencyAnalysis::analyze()
{
    // factory part
    Parser* pParser = configure.Build();
    try
    {
        if (pParser)
        {
            if (!configure.Attach(fileName))
            {
                std::cout << "\n  could not open file " << fileName << std::endl;
                return nullptr;
            }
        }
        else
        {
            std::cout << "\n\n  Parser not built\n\n";
            return nullptr;
        }
        // now that parser is built, use it

        while (pParser->next())
            pParser->parse();

        // display table content
        _pResultTable = configure.getTable();
    }
    catch (std::exception& ex)
    {
        std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    return _pResultTable;
}

// ------------------< test stub > ---------------

#ifdef  TEST_DEPENDENCYANALYSIS
#define zSOURCE1   // multiple source files
#define SOURCE2   // single source file
#define SINGLE
#define zMULTIPLE

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
    depenSpeciFileMgr(pTypeTable _bigTable) :typeTable(_bigTable) {}

    virtual void file(const File& f)
    {
        std::cout << "\n  " << dir_ + "\\" + f + "\n";
        DependencyAnalysis da(f, typeTable);
        pDepenTable depenTable = da.analyze();
        depenTable->print();
        //bigTable->mergeTables(typetable);
    }

    virtual void dir(const Dir& d)
    {
        dir_ = d;
    }
private:
    Dir dir_;
    pTypeTable typeTable;
};

//using File = std::string;
//using Files = std::vector<std::string>;
//Files getCppandHfile(File _file)
//{
//    size_t len = _file.length();
//    File h;
//    File cpp;
//    File fileNameWithtoutExt;
//    // extract .h or .cpp and fabricate two file name both .h and .cpp
//    if (_file.find(".h") != std::string::npos)
//    {
//        fileNameWithtoutExt = _file.substr(0, len - 2);
//    }
//    else
//    {
//        fileNameWithtoutExt = _file.substr(0, len - 4);
//    }
//    h = fileNameWithtoutExt + ".h";
//    cpp = fileNameWithtoutExt + ".cpp";
//    Files files{ h, cpp };
//    return files;
//}

int main()
{
#ifdef SOURCE1  // merged table
    Table<TypeValue> bigTypeTable;
    TypeTableMerge TypeTableMergeEngine;
    typeSpeciFileMgr tsfm(&TypeTableMergeEngine);
    tsfm.addPattern("*.cpp");
    tsfm.addPattern("*.h");
    tsfm.find("..");
    bigTypeTable = TypeTableMergeEngine.getMergedTable();
    bigTypeTable.print();
#endif
#ifdef SOURCE2  //single table
	
	//TypeAnalysis ta("RuleActionForDependency.h");
	TypeAnalysis ta("./ParallelDependencyExecutive/ParallelDependencyExecutive.cpp");
	ITable<TypeValue>* pSingleTypeTable = ta.analyze();
	pSingleTypeTable->print();
#endif


    // test generate .h and .cpp file
  /*  Files files = getCppandHfile("../asdsdsadsad/sadsad/tasdfasdf.h");
    Files files2 = getCppandHfile(".././asdsdsadsad/sadsad/tasdfasasdsaddf.cpp");
    for (auto f : files)
    {
        std::cout << "  files1:" << f + "\n";
    }
    for (auto f : files2)
    {
        std::cout << "  files2:" << f + "\n";
    }*/

#ifdef SINGLE   // single file input

    std::string fileSpec("test3.txt");
    // std::string fileSpec("../FileMgr/FileMgr/FileMgr.h");
#ifdef SOURCE1
	DependencyAnalysis da(fileSpec, &bigTypeTable);
#endif
#ifdef SOURCE2
	DependencyAnalysis da(fileSpec, pSingleTypeTable);
#endif
    ITable<ResultValue>* pResultTable = da.analyze();
    pResultTable->print();
#endif
#ifdef MULTIPLE   // path input

    //// set file
    //std::string fileSpecForType = "../TypeAnalysis/RuleActionForTypeTable.h";
    ////std::string fileSpec = "../TypeAnalysis/test2.txt";
    //TypeAnalysis typeAnalyzer(fileSpecForType);
    //// get type table
    //ITable<TypeValue>* typetable = typeAnalyzer.analyze();

    // intialize dependency analysis
    depenSpeciFileMgr dsfm(&bigTypeTable);
    dsfm.addPattern("*.cpp");
    dsfm.addPattern("*.h");
    dsfm.find("..");

#endif
}
#endif