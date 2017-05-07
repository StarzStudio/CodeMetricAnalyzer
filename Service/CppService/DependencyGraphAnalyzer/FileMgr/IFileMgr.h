#ifndef FILEMGR_H
#define FILEMGR_H
////////////////////////////////////////////////////////////////////////
//  IFileMgr.h - declares new parsing rules and actions                //
//  ver 1.0                                                           //
//  Language:      Visual C++ 2016                                    //
//  Platform:      Mac Book Pro, OS X EI Captain 10.11                //
//  Application:   dependency Analysis, 687 OOD Project3              //
//  Author:        Xing Zhou, Syracuse University                     //
//                 Software Engineering                               //
//                 xzhou118@syr.edu                                   //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University        //
//                 (315) 443-3948, jfawcett@twcny.rr.com              //
////////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
This module provides Interface for FileMgr

Public Interface:
=================
addPattern(const std::string& patt) = 0;
virtual void search() = 0;
virtual void find(const std::string& path) = 0;
virtual void regForFiles(IFileEventHandler* pEventHandler) = 0;
virtual void regForDirs(IDirEventHandler* pEventHandler) = 0

Build Process:
==============
Required files
-     FileSystem.h,  FileSystem.cpp


Build commands
- devenv Project3.sln

Maintenance History:
====================
ver 1.0 : 04 Apr 16
- first release


*/


#include <string>

namespace FileManager
{
    struct IFileEventHandler
    {
        virtual void execute(const std::string& fileSpec) = 0;
    };

    struct IDirEventHandler
    {
        virtual void execute(const std::string& dirSpec) = 0;
    };

    struct IFileMgr
    {
        virtual ~IFileMgr() {}
        virtual void addPattern(const std::string& patt) = 0;
        virtual void search() = 0;
        virtual void find(const std::string& path) = 0;
        virtual void regForFiles(IFileEventHandler* pEventHandler) = 0;
        virtual void regForDirs(IDirEventHandler* pEventHandler) = 0;
    };

    struct FileMgrFactory
    {
        static IFileMgr* create(const std::string& path);
    };
}
#endif