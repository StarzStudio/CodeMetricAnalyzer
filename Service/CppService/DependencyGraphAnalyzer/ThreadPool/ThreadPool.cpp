///////////////////////////////////////////////////////////////////////////
// ThreadPool.cpp - create threads and provide executing service         //
// ver 1.1                                                               //
// Language:      C++, Visual Studio 2015                                //
// Platform:      Mac Book Pro, OS X EI Captain 10.11                    //
// Application:   dependency Analysis, 687 OOD Project3                  //
// Author:        Xing Zhou, Syracuse University, Software Engineering   //
//                xzhou118@syr.edu                                       //
///////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include "ThreadPool.h"
#include "../Utilities/Utilities.h"

// constructor : take the # of starting working threads

ThreadPool::ThreadPool(int num) : threadNum(num) {}

//----< wait for child thread to terminate >---------------------------

void ThreadPool::wait()
{
    for (auto it = threadContainer.begin(); it != threadContainer.end(); it++)
    {
        (*it)->join();
    }
}
//----< enqueue work item >--------------------------------------------

void ThreadPool::doWork(WorkItem Wi)
{
    _workItemQueue.enQ(Wi);
}

// set # of threads
// note: this must be called before start()

void ThreadPool::setThreadNum(int num)
{
    threadNum = num;
}

// return the size of created threads

int ThreadPool::size()
{
    return threadNum;
}

//----< start child thread that dequeus work items >-------------------

void ThreadPool::start()
{
    std::function<void()> threadProc = [&]() {
        while (true)
        {
            //std::cout << " I am working\n";
            WorkItem Wi = _workItemQueue.deQ();
            Wi();
        }
    };
    for (int i = 0; i < threadNum; i++)
    {
        threadContainer.push_back(new std::thread(threadProc));
    }
}
//----< clean up heap >------------------------------------------------

ThreadPool ::~ThreadPool()
{
    threadContainer.clear();
}

//----< test stub >--------------------------------------------------

#ifdef zTEST_THREADPOOL

using Util = Utilities::StringHelper;
using WorkItem = std::function<void()>;

int main()
{
    Util::Title("Enqueued Work Items");

    std::cout << "\n  main thread id = " << std::this_thread::get_id();

    ThreadPool processor;
    processor.start();

    // define 1st work item

    WorkItem  wi1 = []() {
        std::cout << "\n  working on thread " << std::this_thread::get_id();
        return "Hello from wi1";
    };

    processor.doWork(wi1);

    // define 2nd work item

    WorkItem  wi2 = []()
    {
        std::cout << "\n  working on thread " << std::this_thread::get_id();
        size_t sum = 0;
        for (size_t i = 0; i < 100000; ++i)
            sum += i;
        return "wi2 result = " + Utilities::Converter<size_t>::toString(sum);
    };

    processor.doWork(wi2);

    // the following calls to result() block until results are enqueued

    processor.doWork(nullptr);

    // wait for child thread to complete

    processor.wait();
    std::cout << "\n\n";
}
#endif