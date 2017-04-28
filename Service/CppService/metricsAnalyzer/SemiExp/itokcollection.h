#ifndef ITOKCOLLECTION_H
#define ITOKCOLLECTION_H
///////////////////////////////////////////////////////////////////////
// ItokCollection.h - interface, specify methods to operate tokens   //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Lexical Scanner, CSE687 - Object Oriented Design     //
// Author:      Xing Zhou, Computer Engineering SU, Graduate         //
///////////////////////////////////////////////////////////////////////
/*
Module Purporse:
================
ITokCollion is an interface that supports substitution of different
types of scanners for parsing. In this solution

Maintenance History
====================
ver 1.0: 05 Feb 16
-first release

*/


struct ITokCollection
{
    virtual bool get(bool clear = true) = 0;
    virtual size_t length() = 0;
    virtual std::string& operator[] (size_t n) = 0;
    virtual std::string operator[](size_t n) const = 0;
    virtual size_t find(const std::string& tok) = 0;
    virtual void push_back(const std::string&tok) = 0;
    virtual bool remove(const std::string& tok) = 0;
    virtual bool remove(size_t i) = 0;
    virtual void toLower() = 0;
    virtual void trimFront() = 0;
    virtual void clear() = 0;
    virtual std::string show(bool showNewLines = false) = 0;
    virtual bool isComment(const std::string& tok) = 0;
    virtual ~ITokCollection() {};
};

#endif