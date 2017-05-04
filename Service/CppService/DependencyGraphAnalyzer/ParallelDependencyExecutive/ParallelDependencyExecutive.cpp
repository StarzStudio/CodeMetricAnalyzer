//////////////////////////////////////////////////////////////////////////////
// parallelDependencyExecutive.cpp - test executive                         //
// ver 1.0                                                                  //
// Language:      C++, Visual Studio 2015                                   //
// Platform:      Mac Book Pro, OS X EI Captain 10.11                       //
// Application:   dependency Analysis, 687 OOD Project3                     //
// Author:        Xing Zhou, Syracuse University, Software Engineering      //
//                xzhou118@syr.edu                                          //
//////////////////////////////////////////////////////////////////////////////

#include "ParallelDependencyExecutive.h"
//----< test stub >--------------------------------------------------

#ifdef TEST_PARALLELDEPENDENCYEXECUTIVE

using Path = std::string;
using Pattern = std::string;
using Patterns = std::vector<Pattern>;

int main(int argc, char *argv[])
{
    Path path;
    Patterns patterns;
    path = argv[1];
    for (int i = 2; i < argc; i++)
    {
        patterns.push_back(argv[i]);
    }
    parallelDependencyExecutive pde(path, patterns);
    pde.explaination();
    pde.analyze();

    // this is to hold the program and not let the main thread die before all the
    // child thread finish their task
    int a;
    std::cin >> a;
}

#endif