#ifndef ASTNODE_H
#define ASTNODE_H
////////////////////////////////////////////////////////////////////////
//  ASTNode.h - declares new parsing rules and actions                //
//  ver 1.2                                                           //
//  Platform:      Mac Book Pro, OS X EI Captain 10.11                //
//  Application:   Code Analysis, 687 OOD Project2                    //
//  Author:        Xing Zhou, Syracuse University                     //
//                 Software Engineering                               //
//                 xzhou118@syr.edu                                   //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University        //
//                 (315) 443-3948, jfawcett@twcny.rr.com              //
////////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module provides evaluating and displaying the size and complexity
of all global functions, static member functions, and non-static member
functions in a package.

Public Interface:
=================
ASTTree tree;                          // create a ASTTree
ASTNode node;                          // create a Tree node
tree.setRoot(*node);                   // set the root of the tree
tree.gettRoot();                       // get the root of the tree
tree.ExtractScopeInfo();               // return all the nodes info.
node.name()                            // get node's name
node.addChildren(*node)                // add child to the node
node. setComplexity(number)            // set complexity of the node
node.size()                            // get the node's size

Build Process:
==============
Required files
- ScopeStack.h, ScopeStack.cpp

Build commands
- devenv Project2.sln

Maintenance History:
====================
ver 1.2 : 04 Mar 16
- changed endline calculation, and size() function
ver 1.1 : 03 Mar 16
- add ASTTree class
- provide TreeWalk methods to extract info from AST
ver 1.0 : 02 Mar 16
- first release

*/
//

#include <sstream>
#include <vector>
#include "../ScopeStack/ScopeStack.h"
#include <iostream>

class ASTNode
{
public:
    ASTNode() : _name("no_name"), _type("none"), _size(0), _complexity(1) {}
    ASTNode(const std::string& name, const std::string& type) : _name(name), _type(type), _complexity(1) {}
    std::string& name() { return _name; }
    void setName(const std::string& name) { _name = name; }
    std::string& type() { return _type; }
    void setType(const std::string& type) { _type = type; }
    std::vector<ASTNode*>& children() { return _children; }
    void addChildren(ASTNode* child) { _children.push_back(child); }
    void deleteLastChildren() { _children.pop_back(); }
    void setStartLine(size_t lineCountStart) { _lineCountStart = lineCountStart ; }
    void setEndLine(size_t lineCountEnd) { _lineCountEnd = lineCountEnd  ; }   // the real end line is the return line - 2
    size_t complexity() { return _complexity; }
    void setComplexity(size_t complexity) { _complexity = complexity; }
    void addOneComplexity() { _complexity = _complexity + 1; }
    size_t size()
    {
        _size = _lineCountEnd - _lineCountStart + 1;
        return _size;
    }
    std::string show()  // show the information of a single node
    {
        std::ostringstream temp;
        temp << "(";
        temp << _type;
        temp << ", ";
        temp << _name;
        temp << ", Start line:";
        temp << _lineCountStart;
        temp << ", End line:";
        temp << _lineCountEnd;
        temp << ", size:";
        temp << size();
        temp << ", complexity:";
        temp << _complexity;
        temp << ")";
        return temp.str();
    }
private:
    std::string _type;
    std::string _name;
    size_t _lineCountStart;
    size_t _lineCountEnd;
    std::vector<ASTNode*> _children;
    size_t _complexity;
    size_t _size;
};

class ASTTree
{
public:
    // constructor
    ASTTree() {}
    ASTTree(ASTNode* _root) : root(_root) {}
    // manipulate root 
    void setRoot(ASTNode* _root) { root = _root; }
    ASTNode* getRoot() { return root; }
    // extract node info from tree to string type
    std::string ExtractScopeInfo()
    { 
       return  TreeWalk(root);
    };
private:
    ASTNode* root;
    // walk through the tree and print out all the nodes information
    std::string TreeWalk(ASTNode* root)
    {
        std::ostringstream temp;
        static size_t indentLevel = 0;
        temp << "\n  " << std::string(2 * indentLevel, ' ') << root->show();
        auto iter = root->children().begin();
        ++indentLevel;
        while (iter != root->children().end())
        {
            temp << TreeWalk(*iter);
            ++iter;
        }
        --indentLevel;
        return temp.str();
    }  
};
#endif