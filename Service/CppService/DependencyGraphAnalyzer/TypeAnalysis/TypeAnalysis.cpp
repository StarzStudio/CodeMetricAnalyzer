///////////////////////////////////////////////////////////////////////////
// TypeAnalysis.cpp - construct type table based by analyzing a file         //                                                         //
// ver 1.1                                                               //
// Language:      C++, Visual Studio 2015                                //
// Platform:      Mac Book Pro, OS X EI Captain 10.11                    //
// Application:   dependency Analysis, 687 OOD Project3                  //
// Author:        Xing Zhou, Syracuse University, Software Engineering   //
//                xzhou118@syr.edu                                       //
///////////////////////////////////////////////////////////////////////////

#include "TypeAnalysis.h"


// set into a file
void TypeAnalysis::setFile(std::string _fileName)
{
    fileName = _fileName;
}
// begin analyze a single file
ITable<TypeValue>* TypeAnalysis::analyze()
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
        _pTypeTable = configure.getTable();
    }
    catch (std::exception& ex)
    {
        std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    return _pTypeTable;
}

// ------------------< test stub > ---------------

#ifdef  TEST_TYPEANALYSIS

int main()
{
    // set file
    //std::string fileSpec = "..\\ParallelDependencyAnalysis\\ParallelDependencyAnalysis.h";
    //std::string fileSpec = "..\\ParallelDependencyAnalysis\\ParallelDependencyAnalysis.cpp";
    //std::string fileSpec = "..\\DependencyAnalysis\\RuleActionForDependency.h";
    std::string fileSpec = "test2.txt";

    //std::string fileSpec = "../TypeAnalysis/test2.txt";
    TypeAnalysis analyzer(fileSpec);
    // get table
    ITable<TypeValue>* table = analyzer.analyze();
    // print out
    table->print();
    return 0;

	

}
#endif //  debug