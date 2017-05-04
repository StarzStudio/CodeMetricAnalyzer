#ifndef THREADPOOL_H
#define THREADPOOL_H
///////////////////////////////////////////////////////////////////////////
// ThreadPool.h - create, store and provide threads                      //
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
* This package takes in callable object into a blocking queue, creates multiple threads
* to execute them, and also output results into blocking queque. Provide interface for
* clients to input callable oject and get result
*
* Public interface:
* ----------------
* setThreadNum(int)                  set # of threads in the pool
* start();                           let threads begin to work
* doWork();                          enqueue workItems
* resukt();                          dequeue result from result queue
*
* Build Process:
* --------------
* Required files: BlockingQueue.h  BlockingQueue.cpp
*                 Utilities.h, Utilities.cpp
*
* Build Command: devenv project3.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.1 : 28 Mar 2016
* - based on QueuedWorkItem package, change a single thread into
* a container which can hold multiple threads to dequeue workItems
* - provide API to set and get num of threads in the pool
* ver 1.0 : 27 Mar 2016
* - first release
*
*/

#include <functional>
#include <thread>
#include <queue>
#include "../BlockingQueue/BlockingQueue.h"

///////////////////////////////////////////////////////////////////////
// class to process work items
// - each work item is processed sequentially on a single child thread

class ThreadPool
{
    using WorkItem = std::function<void()>;
public:
    ThreadPool(int num = 4);
    void start();
    void doWork(WorkItem pWi);
    void wait();
    int size();
    void setThreadNum(int num = 4);
    ~ThreadPool();
private:
    int threadNum;
    std::vector<std::thread*> threadContainer;
    BlockingQueue<WorkItem> _workItemQueue;
};

#endif 