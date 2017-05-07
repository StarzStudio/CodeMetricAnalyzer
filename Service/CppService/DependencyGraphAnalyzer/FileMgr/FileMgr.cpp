////////////////////////////////////////////////////////////////////////
//  FileMgr.cpp - declares new parsing rules and actions              //
//  ver 1.1                                                           //
//  Language:      Visual C++ 2016                                    //
//  Platform:      Mac Book Pro, OS X EI Captain 10.11                //
//  Application:   dependency Analysis, 687 OOD Project3              //
//  Author:        Xing Zhou, Syracuse University                     //
//                 Software Engineering                               //
//                 xzhou118@syr.edu                                   //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University        //
//                 (315) 443-3948, jfawcett@twcny.rr.com              //
////////////////////////////////////////////////////////////////////////
#include "FileMgr.h"
#include "../Utilities/Utilities.h"

using namespace FileManager;

class AppSpecificFileMgr : public FileMgr
{
public:
    AppSpecificFileMgr() {}

    virtual void file(const File& f)
    {
        std::cout << "\n  " << dir_ + "\\" + f;
    }

    virtual void dir(const Dir& d)
    {
        dir_ = d;
    }

private:
    Dir dir_;
};

using namespace Utilities;
using Utils = StringHelper;

//----< test stub >--------------------------------------------------

#ifdef TEST_FILEMGR

int main()
{
    Utils::Title("MT3Q6 - Demonstrating FileMgr");
    putline();
    /* Utils::title("Base class behavior");
     FileMgr fm("../..");
     fm.addPattern("*.h");
     fm.addPattern("*.cpp");
     fm.find();
     putline();*/

    Utils::title("Derived class behavior");
    AppSpecificFileMgr asfm;
    asfm.addPattern("*.cpp");

    asfm.find("..\\..");
    putline();

    std::cout << "\n\n";
}

#endif