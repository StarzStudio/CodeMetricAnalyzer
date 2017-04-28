#ifndef SCOPESTACK_H
#define SCOPESTACK_H
/////////////////////////////////////////////////////////////////////////////
// ScopeStack.h - implements template stack holding specified element type //
// ver 2.0                                                                 //
// Platform:      Mac Book Pro, OS X EI Captain 10.11                      //
// Application:   Code Analysis, 687 OOD Project2                          //
// Author:        Xing Zhou, Syracuse University                           //
//                Software Engineering                                     //
//                xzhou118@syr.edu                                         //
// Source:        Jim Fawcett, CST 4-187, Syracuse University              //
//                (315) 443-3948, jfawcett@twcny.rr.com                    //
/////////////////////////////////////////////////////////////////////////////
/*
  Package Operations:
  ===================
  This package contains one class: ScopeStack<element>.  The element type
  is an application specific type designed to hold whatever information you
  need to stack.

  Public Interface:
  =================
  ScopeStack<element> stack;
  // define some element type
  element elem;
  stack.push(elem);
  element popped = stack.pop();

  Required Files:
  ===============
  ScopeStack.h, ScopeStack.cpp

  Build Command:
  ==============
  devenv Project2.sln /rebuild debug

  Maintenance History:
  ====================
  ver 2.1 : 02 Jun 11
  - changed ScopeStack type to single template argument intended to be
    an application specific type (usually a struct that holds whatever
    is needed)
  ver 2.0 : 02 Feb 11
  - fixed bug in pop() method found by Drumil Bhattad
  ver 1.0 : 31 Jan 11
  - first release
*/
#include <list>
#include <iostream>
// element is a application specific type.  It must provide a function:
// std::string show() that returns a string display of its parts. see
// test stub for an example.

template<typename element>
class ScopeStack {
public:
    typedef typename std::list<element>::iterator iterator;
  void push(const element& item);
  element pop();
  element top();
  size_t size();
  iterator begin();
  iterator end();
private:
  std::list<element> stack;
};

template<typename element>

size_t ScopeStack<element>::size() { return stack.size(); }

template<typename element>
typename  ScopeStack<element>::iterator ScopeStack<element>::begin() { return stack.begin(); }

template<typename element>
typename  ScopeStack<element>::iterator ScopeStack<element>::end() { return stack.end(); }

template<typename element>
void ScopeStack<element>::push(const element& item)
{
  stack.push_back(item);
}

template<typename element>
element ScopeStack<element>::pop() 
{ 
  element item = stack.back(); 
  stack.pop_back(); 
  return item; 
}

template<typename element>
element ScopeStack<element>::top()
{
  return stack.back();
}

template<typename element>
void showStack(ScopeStack<element>& stack, bool indent = true)
{
  if (stack.size() == 0)
  {
    std::cout << "\n  ScopeStack is empty";
    return;
  }
  typename ScopeStack<element>::iterator iter = stack.begin();
  while (iter != stack.end())
  {
    std::string strIndent = std::string(2 * stack.size(), ' ');
    if (!indent)
      strIndent = "";
    element temp = *iter;
    std::cout << "\n  " << strIndent << temp->show();
    ++iter;
  }
}

template<typename element>
void showStack(ScopeStack<element*>& stack, bool indent = true)
{
  if (stack.size() == 0)
  {
    std::cout << "\n  ScopeStack is empty";
    return;
  }
  typename ScopeStack<element*>::iterator iter = stack.begin();
  while (iter != stack.end())
  {
    std::string strIndent = std::string(2 * stack.size(), ' ');
    if (!indent)
      strIndent = "";
    std::cout << "\n  " << strIndent << ((*iter)->show());
    ++iter;
  }
}
#endif
