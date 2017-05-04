#ifndef TABLE_H
#define TABLE_H
///////////////////////////////////////////////////////////////////////////
// Table.h - define the table to store type information                  //
// ver 1.5                                                               //
// Language:      C++, Visual Studio 2015                                //
// Platform:      Mac Book Pro, OS X EI Captain 10.11                    //
// Application:   dependency Analysis, 687 OOD Project3                  //
// Author:        Xing Zhou, Syracuse University, Software Engineering   //
//                xzhou118@syr.edu                                       //
///////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* provide application specific value, table to store data
*
* Public interface:
* ----------------
*   Table:
*   addRecord(const Key& Name, const Value& record);
*   Record getRecord(const Key& Name);
*   void changeValue(const Key& Name, const Value& record);
*   Value getValue(const Key& Name);
*   Value& operator[](Key name);
*   int size();
*   void deleteRecord(const Key& Name);
*   void clear();
*   std::vector<Key> getKeyList();
*   bool find(const Key& Name);
*   void printHeader();
*   void print();
*
*    Value:
*    setType
*    setFileName
*    addNamespace
*    getFileName
*    find
*    print
*
* Build Process:
* --------------
* Required files: ITable.h
*                 Utilities.h, Utilities.cpp
*
* Build Command: devenv project3.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.5 : 06 Apr 2016
* - change behaviour of dependency table merge, replace old item when megering
* ver 1.4 : 02 Apr 2016
* - elimate redundant adding dependent file int RuleAction class
*  by changing ResultValue class's add  function behaviour
* ver 1.3 : 30 Mar 2016
* - add print function to display table in a nice format
* ver 1.2 : 29 Mar 2016
* - provide value class for type table and result table
* ver 1.1 : 28 Mar 2016
* - add an interface class for table
* ver 1.0 : 27 Mar 2016
* - first release
*
* TODO:
* change print function to detect the field length
*/
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "../Table/ITable.h"
#include <iostream>
#include <iomanip>
#include <sstream>

class TypeValue : IValue
{
public:
    TypeValue() {}
    TypeValue(const std::string& _type, const std::vector<std::string>& _namespace, std::string& _fileName)
        : Type(_type), Namespace(_namespace), FileName(_fileName) {}
    // set the type record
    void setType(const std::string& _Type)
    {
        Type = _Type;
    }
    // get the type record
    std::string getType()
    {
        return Type;
    }
    // add one namespace element into the namespace record
    void addNamespace(const std::string& _Namespace)
    {
        Namespace.push_back(_Namespace);
    }
    // get the size of namespace record
    int size_Namespace()
    {
        return Namespace.size();
    }
    // set the FileName record
    void setFileName(const std::string& _FileName)
    {
        FileName = _FileName;
    }
    // get Namespace collection
    const std::vector<std::string>& getNamespaceCollection()
    {
        return Namespace;
    }
    // find if a namespace is in this collection
    bool find(std::string _namespace)
    {
        for (auto it = Namespace.begin(); it != Namespace.end(); it++)
        {
            if ((*it) == _namespace)
                return true;
        }
        return false;
    }
    // get the FileName record
    std::string getFileName()
    {
        return FileName;
    }
    // print the Value info in a format
    void print()
    {
        long save = std::cout.flags();    // save format state
        std::cout.fill(' ');
        std::cout.setf(std::ios::left, std::ios::adjustfield);
        std::cout << std::setw(20) << Type;
        std::cout << std::setw(35) << FileName;
        bool first = true;
        for (auto _namespace : Namespace)
        {
            if (first == true)
            {
                std::cout << _namespace;
                first = false;
            }
            else
            {
                std::cout << ", " << _namespace;
            }
        }
        std::cout << std::endl;
        std::cout.flags(save);
    }
    void printHeader()
    {
        std::cout.fill(' ');
        long save = std::cout.flags();    // save format state
        // table head
        std::cout.setf(std::ios::left, std::ios::adjustfield);
        std::cout << std::setw(20) << "Type";
        std::cout << std::setw(35) << "FileName";
        std::cout << std::setw(20) << "Namespace";
        std::cout << "\n";
        std::cout.flags(save);
    }
private:
    std::string Type;
    std::vector<std::string> Namespace;
    std::string FileName;
};

// the result is a single pair type
class ResultValue : IValue
{
    using File = std::string;
    using Value = std::vector<std::string>;
public:
    // add a item into the value collection
    void add(File fileName)
    {
        if (!find(fileName))
        {
            _value.push_back(fileName);
        }
    }
    // find if a file is in this collection
    bool find(std::string _file)
    {
        for (auto it = _value.begin(); it != _value.end(); it++)
        {
            if ((*it) == _file)
                return true;
        }
        return false;
    }
    // get the files collecion
    Value get()
    {
        return _value;
    }
    // set the value
    void set(Value value)
    {
        _value = value;
    }
    // print the Value info in a format
    void print()
    {
        std::cout << "\n  This file depends on the files below: \n";
        for (auto file : _value)
        {
            std::cout << "  " << file << std::endl;
        }
        std::cout << "\n";
    }
    void printHeader()
    {
        // do nothing, just to meet the interface
    }
private:
    Value _value;
};

// table to store type
template<typename Value>
class Table : public ITable<Value>
{
    using Key = std::string;
    using Record = std::pair<Key, Value>;
public:
    void addRecord(const Key& Name, const Value& record);
    Record getRecord(const Key& Name);
    void changeValue(const Key& Name, const Value& record);
    Value getValue(const Key& Name);
    Value& operator[](Key name);
    int size();
    void deleteRecord(const Key& Name);
    void clear();
    std::vector<Key> getKeyList();
    bool find(const Key& Name);
    void printHeader();
    void print();
    //void mergeTables(ITable<Value>* pTable, ITableMerge<Value>* mergeEngine);
private:
    std::map <Key, Value> table;
};
// add one record into the table
template<typename Value>
void Table<Value>::addRecord(const Key& Name, const Value& record)
{
    table[Name] = record;
}

// return a single record if provided Key
// return the first Record if not found
template<typename Value>
std::pair<std::string, Value> Table<Value>::getRecord(const Key& Name)
{
    if (table.find(Name) == table.end())
        throw(std::exception("no name found"));
    auto it = table.begin();
    for (; it != table.end(); it++)
    {
        if (it->first == Name)
            break;
    }
    return *it;
}

// change value given the Key
template<typename Value>
void Table<Value>::changeValue(const Key& Name, const Value& _value)
{
    table[Name] = _value;
}

// get the value if queryed name is known
template<typename Value>
Value Table<Value>::getValue(const Key& Name)
{
    if (table.find(Name) == table.end())
        throw(std::exception("no name found"));
    return table[Name];
}

template<typename Value>
Value& Table<Value>::operator[](Key name)
{
    if (table.find(name) == table.end())
        throw(std::exception("no name found"));
    return table[name];
}

//template<typename Value>
//Value Table<Value>::operator[](const Key name) const
//{
//    if (table.find(name) == table.end())
//        throw(std::exception("no name found"));
//    Value value;
//    return value ;
//}

// get the size of the table
template<typename Value>
int Table<Value>::size()
{
    return table.size();
}

// delete a specified record providing Key
template<typename Value>
void Table<Value>::deleteRecord(const Key& Name)
{
    table.erase(Name);
}

// clear the table content
template<typename Value>
void Table<Value>::clear()
{
    table.clear();
}

// return a list of all the names in the table
template<typename Value>
std::vector<std::string> Table<Value>::getKeyList()
{
    std::vector<Key> nameList;
    for (auto it = table.begin(); it != table.end(); ++it)
    {
        nameList.push_back(it->first);
    }
    return nameList;
}

// find whether a particular name is stored in the table, return true if found, false otherwise
template<typename Value>
bool Table<Value>::find(const Key& Name)
{
    std::map<Key, Value>::const_iterator position = table.find(Name);
    if (position != table.end())
        return true;
    else
        return false;
}

// merge partial table into the single big table.
//template<typename Value>
//void Table<Value>::mergeTables(ITable<Value>* table, ITableMerge<Value>* mergeEngine)
//{
//    mergeEngine->merge(table);
//};

// print table in a format
template<typename Value>
void Table<Value>::printHeader()
{
    Value value;
    std::cout.fill(' ');
    long save = std::cout.flags();    // save format state
    std::cout.setf(std::ios::left, std::ios::adjustfield);
    std::cout << "  " << std::setw(30) << "Name";
    value.printHeader();
}

// print table in a format
template<typename Value>
void Table<Value>::print()
{
    Value value;
    printHeader();
    for (auto pair : table)
    {
        std::cout.fill(' ');
        long save = std::cout.flags();    // save format state
        std::cout.setf(std::ios::left, std::ios::adjustfield);
        std::cout << "\n  " << std::setw(30) << pair.first;
        std::cout.flags(save);
        value = pair.second;
        value.print();
    }
}

// merge engine for type table
class TypeTableMerge : public ITableMerge<TypeValue>
{
public:
    virtual void merge(ITable<TypeValue>* table)
    {
        std::vector<std::string> nameList = table->getKeyList();
        for (auto name : nameList)
        {
            // if the name is not stored in the bigTable, then update it.
            if (!bigTable.find(name))
            {
                bigTable.addRecord(name, table->getValue(name));
            };
        }
    }
    Table<TypeValue> getMergedTable()
    {
        return bigTable;
    }
private:
    Table<TypeValue> bigTable;
};

// merge engine for dependency table
class DependencyTableMerge : public ITableMerge <ResultValue>
{
    using Key = std::string;
public:
    // update value when found duplicate
    void updateValueInBigTable(Key name, ITable<ResultValue>* table)
    {
        ResultValue resultInBigTable = bigTable.getValue(name);
        ResultValue newResult = resultInBigTable;
        ResultValue resultInTable = table->getValue(name);
        std::vector<std::string> files = resultInTable.get();
        for (auto f : files)
        {
            if (!resultInBigTable.find(f))
            {
                newResult.add(f);
            }
        }
        bigTable.changeValue(name, newResult);
    }
    virtual void merge(ITable<ResultValue>* table)
    {
        std::vector<std::string> nameList = table->getKeyList();
        for (auto name : nameList)
        {
            // if the name is not stored in the bigTable, then update it.
            if (!bigTable.find(name))
            {
                bigTable.addRecord(name, table->getValue(name));
            }
            else
            {
                updateValueInBigTable(name, table);
            }
            bigTable.addRecord(name, table->getValue(name));
        }
    }
    Table<ResultValue> getMergedTable()
    {
        return bigTable;
    }
private:
    Table<ResultValue> bigTable;
};
#endif 