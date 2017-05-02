#ifndef ITABLE_H
#define ITABLE_H
///////////////////////////////////////////////////////////////////////////
// ITable.h - executive callable objects using services of threadpool    //
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
* provide abstact class for Value, Table and Merge engine
*
*    Public interface:
*    ITable:
*    addRecord(const Key& _name, const value& _value) = 0;
*    Record getRecord(const Key& _name) = 0;
*    void deleteRecord(const Key& Name) = 0;
*    void changeValue(const Key& _name, const value& _value) = 0;
*    value getValue(const Key& _name) = 0;
*    value& operator[](Key _name) = 0;
*    int size() = 0;
*    void clear() = 0;
*    std::vector<Key> getKeyList() = 0;
*    bool find(const Key& Name) = 0;
*    void print() = 0;
*    void printHeader()
*
*    IValue:
*
*    print()
*    virtual void printHeader()
*
*    ITableMerge:
*    merge
*
*
* Build Process:
* --------------
* Required Files:
*
* Build Command: devenv project3.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.1 : 30 Mar 2016
* - add value interface
* - add print and printHeader function into the ITable;
* ver 1.0 : 27 Mar 2016
* - first release
*
*/

#include <map>
#include <string>
#include <vector>

template<typename value>
class ITableMerge;

template<typename value>
class ITable
{
    using Key = std::string;
    using Record = std::pair<Key, value>;
public:
    ~ITable() {}
    virtual void addRecord(const Key& _name, const value& _value) = 0;
    virtual Record getRecord(const Key& _name) = 0;
    virtual void deleteRecord(const Key& Name) = 0;
    virtual void changeValue(const Key& _name, const value& _value) = 0;
    virtual value getValue(const Key& _name) = 0;
    virtual value& operator[](Key _name) = 0;
    virtual int size() = 0;
    virtual void clear() = 0;
    virtual std::vector<Key> getKeyList() = 0;
    virtual bool find(const Key& Name) = 0;
    virtual void print() = 0;
    virtual void printHeader() = 0;
    // virtual void mergeTables(ITable<value>* pTable, ITableMerge<value>* mergeEngine) = 0;
};

// the value which is meant to be pair with key in the table
class IValue
{
public:
    virtual void print() = 0;
    virtual void printHeader() = 0;
    ~IValue() {}
};

// define merge table behabiour
template<typename value>
class ITableMerge
{
public:
    virtual void merge(ITable<value>* table) = 0;
};
#endif