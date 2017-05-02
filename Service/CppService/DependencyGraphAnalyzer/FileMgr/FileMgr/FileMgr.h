#ifndef FILEMGR_H
#define FILEMGR_H

////////////////////////////////////////////////////////////////////////
//  FileMgr.h - declares new parsing rules and actions                //
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
/*
Module Operations:
==================
DFS each file matching the user-provided patterns if given path.


Note:
Subclass will inherit and provide custom functionality inside "virtual void file(const File& file)" 
for operation on each file this package iterate through.


Public Interface:
=================
addPattern(const std::string& patt) = 0;
virtual void search() = 0;
virtual void find(const std::string& path) = 0; 

virtual functions:
=========================
virtual void file(const File& file)
  - is a hooker, which will be executed for each file this package searches.



Build Process:
==============
Required files
-     FileSystem.h,  FileSystem.cpp
      Utilities.h, Utilities.cpp

Build commands
- devenv Project3.sln

Maintenance History:
====================
ver 1.0 : 04 Apr 16
- first release


*/

#include <vector>
#include <iostream>
#include "FileSystem.h"

namespace FileManager
{
    class FileMgr
    {
    public:
        using Pattern = std::string;
        using Patterns = std::vector<Pattern>;
        using File = std::string;
        using Files = std::vector<File>;
        using Dir = std::string;
        using Dirs = std::vector<Dir>;
        using Path = std::string;

        FileMgr()
        {
            path_ = ".";
            patterns_.push_back("*.*");
        }

        FileMgr(const Path& path) : path_(path)
        {
            patterns_.push_back("*.*");
        }

        virtual ~FileMgr() {}

        void addPattern(const Pattern& patt)
        {
            if (patterns_.size() == 1 && patterns_[0] == "*.*")
                patterns_.pop_back();
            patterns_.push_back(patt);
        }

        void search()
        {
            find(path_);
        }

		
        virtual void file(const File& file)
        {
            std::cout << "\n    " << file;
        }

        virtual void dir(const Dir& dir)
        {
            std::cout << "\n  " << dir;
        }

        // gather relative path
        void find(const Path& path = "")
        {
            Path fpath = path;
            if (path == "")
                fpath = path_;
            for (auto patt : patterns_)
            {
                Files files = FileSystem::Directory::getFiles(fpath, patt);
                for (auto f : files)
                {
                    // This _file name is with relative path as suffix
                    File _file = FileSystem::Path::fileSpec(path, f);
                    file(_file);
                }
            }
            Dirs dirs = FileSystem::Directory::getDirectories(fpath);
            for (auto d : dirs)
            {
                if (d == "." || d == "..")
                    continue;
                Path dpath = fpath + "\\" + d;
                find(dpath);
            }
        }
    protected:
        Path path_;
        Files fileSpecs_;
        Patterns patterns_;
    };
}

#endif
