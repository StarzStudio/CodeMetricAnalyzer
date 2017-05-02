///////////////////////////////////////////////////////////////////////////
// TestExecutive.cpp - executive                                         //
// ver 1.0                                                               //
// Language:      C++, Visual Studio 2015                                //
// Platform:      Mac Book Pro, OS X EI Captain 10.11                    //
// Application:   dependency Analysis, 687 OOD Project3                  //
// Author:        Xing Zhou, Syracuse University, Software Engineering   //
//                xzhou118@syr.edu                                       //
///////////////////////////////////////////////////////////////////////////

#include "TestExecutive.h"

//----< test stub >--------------------------------------------------


#ifdef TEST_TESTEXECUTIVE

int main(int argc, char *argv[])
{
	using Path = std::string;
	using Pattern = std::string;
	using Patterns = std::vector<Pattern>;

	Path path;
	Patterns patterns;
	path = argv[1];
	for (int i = 2; i < argc; i++)
	{
		patterns.push_back(argv[i]);
	}

	SingleThreadExecutive exe(path, patterns);
	exe.start();

}

#endif