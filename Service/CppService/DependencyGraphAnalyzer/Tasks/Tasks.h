#ifndef TASKS_H
#define TASKS_H
///////////////////////////////////////////////////////////////////////////
// Tasks.h - executive callable objects using services of threadpool     //
// ver 1.1                                                               //
// Language:      C++, Visual Studio 2015                                //
// Platform:      Mac Book Pro, OS X EI Captain 10.11                    //
// Application:   dependency Analysis, 687 OOD Project3                  //
// Author:        Xing Zhou, Syracuse University, Software Engineering   //
//                xzhou118@syr.edu                                       //
///////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package uses the services of ThreadPool to exective callable object.
* It shall be generic.
*
* Note: Task must exists if callobj is meant to be executed by Thread pool
* because threadpool takes in pointer to the callObj so task must maintain
* that callObj not out of scope by storing it as its private member
*
* Public interface:
*
* doWork(CallObj);                 give one callable object for threadPool to execute
* Result();                        get the result from threadPool
* setThreadNum(int);               set size of threadPool
* threadPoolSize();                get size of threadPool
*
* Build Process:
* --------------
* Required Files: ThreadPool.h, ThreadPool.cpp
*                 BlockingQueue.h, BlockingQueue.cpp
*                 Utilities.h, Utilities.cpp
*
* Build Command: devenv project3.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.1 : 01 Apr 2016
* - change threadpool member to static
* ver 1.0 : 27 Mar 2016
* - first release
*
*/

#include "../ThreadPool/ThreadPool.h"

class Tasks
{
    using WorkItem = std::function<void()>;
public:
    Tasks(int num = 4);
    ~Tasks();
    void doWork(WorkItem work);
    void setThreadNum(int num = 4);
    int threadPoolSize();
private:
    static ThreadPool threadPool;
    std::vector<WorkItem> callObjCollection;
};

#endif