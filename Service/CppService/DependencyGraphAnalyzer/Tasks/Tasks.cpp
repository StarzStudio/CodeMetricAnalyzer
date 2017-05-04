///////////////////////////////////////////////////////////////////////////
// Tasks.cpp - executive callable objects using services of threadpool   //
// ver 1.1                                                               //
// Language:      C++, Visual Studio 2015                                //
// Platform:      Mac Book Pro, OS X EI Captain 10.11                    //
// Application:   dependency Analysis, 687 OOD Project3                  //
// Author:        Xing Zhou, Syracuse University, Software Engineering   //
//                xzhou118@syr.edu                                       //
///////////////////////////////////////////////////////////////////////////

#include "Tasks.h"
#include "../Utilities/Utilities.h"

// initialzie threadpool and start it

ThreadPool Tasks::threadPool;

Tasks::Tasks(int num)
{
    threadPool.setThreadNum(num);
    threadPool.start();
}

// assign workItem to threadPool and execute

void Tasks::doWork(WorkItem work)
{
    threadPool.doWork(work);
}

// set # of threads in the threadpool

void Tasks::setThreadNum(int num)
{
    threadPool.setThreadNum(num);
}

// get the size of threadPool

int Tasks::threadPoolSize()
{
    return threadPool.size();
}

// destructor, join all threads in the pool and delete threadpool pointer

Tasks ::~Tasks()
{
    threadPool.wait();
}

//----< test stub >--------------------------------------------------

#ifdef zTEST_TASKS

using Util = Utilities::StringHelper;

using WorkItem = std::function<void()>;

int main()
{
    Util::Title("Enqueued Work Items");

    std::cout << "\n  main thread id = " << std::this_thread::get_id();

    Tasks  task(4);

    // define 1st work item

    WorkItem wi1 = []() {
        std::cout << "\n  working on thread " << std::this_thread::get_id();
        size_t sum = 0;
        for (size_t i = 0; i < 10000000; ++i)
            sum += i;
        return "wi1 result = " + Utilities::Converter<size_t>::toString(sum);
    };

    task.doWork(wi1);

    // define 2nd work item

    WorkItem wi2 = []()
    {
        std::cout << "\n  working on thread " << std::this_thread::get_id();
        size_t sum = 0;
        for (size_t i = 0; i < 10000000; ++i)
            sum += i;
        return "wi2 result = " + Utilities::Converter<size_t>::toString(sum);
    };

    task.doWork(wi2);

    // define 3rd work item

    WorkItem wi3 = []() {
        std::cout << "\n  working on thread " << std::this_thread::get_id();
        size_t sum = 0;
        for (size_t i = 0; i < 10000000; ++i)
            sum += i;
        return "wi3 result = " + Utilities::Converter<size_t>::toString(sum);
    };

    task.doWork(wi3);

    // define 4th work item

    WorkItem wi4 = []() {
        std::cout << "\n  working on thread " << std::this_thread::get_id();
        size_t sum = 0;
        for (size_t i = 0; i < 10000000; ++i)
            sum += i;
        return "wi4 result = " + Utilities::Converter<size_t>::toString(sum);
    };

    task.doWork(wi4);

    // define 5th work item

    WorkItem wi5 = []() {
        std::cout << "\n  working on thread " << std::this_thread::get_id();
        size_t sum = 0;
        for (size_t i = 0; i < 10000000; ++i)
            sum += i;
        return "wi5 result = " + Utilities::Converter<size_t>::toString(sum);
    };

    task.doWork(wi5);

    // define 6th work item

    WorkItem wi6 = []() {
        std::cout << "\n  working on thread " << std::this_thread::get_id();
        size_t sum = 0;
        for (size_t i = 0; i < 10000000; ++i)
            sum += i;
        return "wi6 result = " + Utilities::Converter<size_t>::toString(sum);
    };

    task.doWork(wi6);

    // the following calls to result() block until results are enqueued

    //std::cout << "\n  " << task.result();
    //std::cout << "\n  " << task.result();
    //std::cout << "\n  " << task.result();
    //std::cout << "\n  " << task.result();
    //std::cout << "\n  " << task.result();
    //std::cout << "\n  " << task.result();

    //task.doWork(nullptr);

    // wait for child thread to complete

    std::cout << "\n\n";
}
#endif