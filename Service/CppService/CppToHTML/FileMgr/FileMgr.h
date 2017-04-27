#ifndef FILEMGR_H
#define FILEMGR_H

/////////////////////////////////////////////////////////////////////
// FileMgr.h - recursively go through all the files in a dir       //
// ver 1.2                                                         //
// platform: Win 10												                         //
// Xing Zhou, work.xingzhou@gmail.com, Syracuse University         //
// Application: MEAN stack using C++ to analyze code metrics       //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a class, FileMgr, and object factory function,
* Create().  It's mission is to search the directory tree, rooted at
* a specified path, looking for files that match a set of specified
* patterns.
*
* It provides virtual functions file(...), dir(...), and done(), which
* an application can override to provide application specific processing
* for the events: file found, directory found, and finished processing.
*
* The package also provides interface hooks that serve the same purpose
* but allow multiple receivers for those events.
*
* Required Files:
* ---------------
*   FileMgr.h, FileMgr.cpp, IFileMgr.h, 
*   FileSystem(Linux).h, FileSystem(Linux).cpp
*
*
* Maintenance History:
* --------------------
* ver 3.0 : 24 Apr 2017
* - manage the linux version: 
*   - shall add a '/' after each dir when traverse for linux
*     in windows, add '\\'
*/

#include "IFileMgr.h"
#include "../FileSystem/FileSystem.h"

namespace FileManager
{
class FileMgr : public IFileMgr
{
public:
  using patterns = std::vector<std::string>;
  using File = std::string;
  using Dir = std::string;
  using fileSubscribers = std::vector<IFileEventHandler *>;
  using dirSubscribers = std::vector<IDirEventHandler *>;
  using doneSubscribers = std::vector<IDoneEventHandler *>;

  virtual ~FileMgr() {}

  //----< set default file pattern >-------------------------------

  FileMgr(const std::string &path) : path_(path)
  {
    patterns_.push_back("*.*");
    pInstance_ = this;
  }
  //----< return instance of FileMgr >-----------------------------

  static IFileMgr *getInstance()
  {
    return pInstance_;
  }
  //----< add file pattern, removing default on first call >-------

  void addPattern(const std::string &patt)
  {
    if (patterns_.size() == 1 && patterns_[0] == "*.*")
      patterns_.pop_back();
    patterns_.push_back(patt);
  }
  //----< applications can overload this or reg for fileEvt >------

  virtual void file(const File &f)
  {
    ++numFilesProcessed;
    for (auto pEvtHandler : fileSubscribers_)
    {
      pEvtHandler->execute(f);
    }
    std::cout << "  traverse file: " << f.c_str() << std::endl;
  }
  //----< applications can overload this or reg for dirEvt >-------

  virtual void dir(const Dir &fpath)
  {
    for (auto pEvtHandler : dirSubscribers_)
    {
      pEvtHandler->execute(fpath);
    }
  }
  //----< applications can overload this or reg for doneEvt >------

  virtual void done()
  {
    for (auto pEvtHandler : doneSubscribers_)
    {
      pEvtHandler->execute(numFilesProcessed);
    }
  }
  //----< start search on previously specified path >--------------

  void search()
  {
    find(path_);
    done();
  }
  //----< search current path including subdirectories >-----------

  void find(const std::string &path)
  {
     std::cout << "file manager is search relative path: " << path.c_str() << std::endl;
     	std::cout << "Filemanager current path: " << FileSystem::Directory::getCurrentDirectory().c_str() << std::endl;
    std::string fpath = FileSystem::Path::getFullFileSpec(path);
    std::cout << "file manager is search path: " << fpath.c_str() << std::endl;
    dir(fpath);
    for (auto patt : patterns_)
    {
      std::vector<std::string> files = FileSystem::Directory::getFiles(fpath, patt);
      for (auto f : files)
      {
        File _file = FileSystem::Path::fileSpec(fpath, f);
        file(_file);
      }
    }
    std::vector<std::string> dirs = FileSystem::Directory::getDirectories(fpath);
    for (auto d : dirs)
    {
      if (d == "." || d == "..")
        continue;
      std::string dpath = fpath + "/" + d;
      find(dpath);
    }
  }
  //----< applications use this to register for notification >-----

  void regForFiles(IFileEventHandler *pHandler)
  {
    fileSubscribers_.push_back(pHandler);
  }
  //----< applications use this to register for notification >-----

  void regForDirs(IDirEventHandler *pHandler)
  {
    dirSubscribers_.push_back(pHandler);
  }
  //----< applications use this to register for notification >-----

  void regForDone(IDoneEventHandler *pHandler)
  {
    doneSubscribers_.push_back(pHandler);
  }

private:
  std::string path_;
  patterns patterns_;
  size_t numFilesProcessed = 0;
  fileSubscribers fileSubscribers_;
  dirSubscribers dirSubscribers_;
  doneSubscribers doneSubscribers_;
  static IFileMgr *pInstance_;
};

struct FileMgrFactory
{
  static IFileMgr *create(const std::string &path);
};

inline IFileMgr *FileMgrFactory::create(const std::string &path)
{
  return new FileMgr(path);
}
}
#endif
