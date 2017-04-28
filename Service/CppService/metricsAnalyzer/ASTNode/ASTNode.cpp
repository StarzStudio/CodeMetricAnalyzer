////////////////////////////////////////////////////////////////////////
//  ASTNode.cpp - declares new parsing rules and actions              //
//  ver 1.2                                                           //
//  Platform:      Mac Book Pro, OS X EI Captain 10.11                //
//  Application:   Code Analysis, 687 OOD Project2                    //
//  Author:        Xing Zhou, Syracuse University                     //
//                 Software Engineering                               //
//                 xzhou118@syr.edu                                   //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University        //
//                 (315) 443-3948, jfawcett@twcny.rr.com              //
////////////////////////////////////////////////////////////////////////

// ----< test stub >------------------------------------------------------

#ifdef TEST_ASTNODE

#include "ASTNode.h"
#include <sstream>
#include <iostream>

int main()
{
    std::cout << "\n  Testing ASTNode using DFS traserval to show";
    std::cout << "\n =====================\n";


    ASTNode* root = new ASTNode("root", "class");
    root->setStartLine(1);
    root->setEndLine(156);
    root->setComplexity(16);

    ASTNode* fun1 = new ASTNode("fun1", "function");
    fun1->setStartLine(22);
    fun1->setEndLine(55);
    fun1->setComplexity(8);

    ASTNode* fun2 = new ASTNode("fun2", "function");
    fun2->setStartLine(56);
    fun2->setEndLine(78);
    fun2->setComplexity(7);

    ASTNode* fun3 = new ASTNode("fun3", "function");
    fun3->setStartLine(79);
    fun3->setEndLine(103);
    fun3->setComplexity(6);

    ASTNode* fun4 = new ASTNode("fun4", "function");
    fun4->setStartLine(104);
    fun4->setEndLine(156);
    fun4->setComplexity(7);

    // add functions to root's children     
    /* structure:
           * root
          /
         *   fun1
        /
       *     fun2
      /
     *       fun3
    /
   *         fun4          */
     
   
    root->addChildren(fun1);
    fun1->addChildren(fun2);
    fun2->addChildren(fun3);
    fun3->addChildren(fun4);

    std::cout << "\n  go through the AST: \n";

    ASTTree tree;

    tree.setRoot(root);
    std::cout << "\n  tree's scope information: \n" << tree.ExtractScopeInfo();

  


}
#endif