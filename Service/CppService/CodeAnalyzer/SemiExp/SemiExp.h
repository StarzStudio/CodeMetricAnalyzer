#ifndef SEMIEXPRESSION_H
#define SEMIEXPRESSION_H
///////////////////////////////////////////////////////////////////////
// SemiExpression.h - collect tokens for analysis                    //
// ver 3.7                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Xing Zhou, Computer Engineering SU, Graduate         //
// source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a public SemiExp class that collects and makes
* available sequences of tokens.  SemiExp uses the services of a Toker
* class to acquire tokens.  Each call to SemiExp::get() returns a
* sequence of tokens that ends in {.  This will be extended to use the
* full set of terminators: {, }, ;, and '\n' if the line begins with #.
*
* This is a new version for students in CSE687 - OOD, Spring 2016 to use
* for Project #1.
*
* Public Interface:
* -----------------
* size_t length();                              // return the length of current token collection
* size_t find(const std::string& tok);          // if not found, return the length()
* void push_back(const std::string&tok);        // add to the rear
* bool remove(const std::string& tok);          // remove the first one found
* bool remove(size_t i);                        // remove by index
* void toLower();                               // to lower String
* void trimFront();                             // trim all the \n in the most front of the collection
* std::string show(bool showNewLines = false);  // display SemiExp

* Build Process:
* --------------
* Required Files:
*   SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp,
*   ITokCollection.h
*
* Build Command: devenv Project2.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 3.7 : 11 Mar 2016
* - fix the find, let return value be length() if not found
* ver 3.6 : 10 Mar 2016
* - fix the remove(int ) function
* - fix the trimFront
* ver 3.5 : 09 Feb 2016
* - rewrite the terminator #...\n case detection
* ver 3.4 : 08 Feb 2016
* - wrap terminator detection into function isTerminator.
* - provide function to detect the for(;;) case
* ver 3.3 : 07 Feb 2016
* - inplement all the virtual functions in the ItokCollection interface
*   update the function get(), in which adds more terminators: { } ;
*   also: \n # , public:, private: ,protected:
* ver 3.2 : 02 Feb 2016
* - declared SemiExp copy constructor and assignment operator = delete
* - added default argument for Toker pointer to nullptr so SemiExp
*   can be used like a container of tokens.
* - if pToker is nullptr then get() will throw logic_error exception
* ver 3.1 : 30 Jan 2016
* - changed namespace to Scanner
* - fixed bug in termination due to continually trying to read
*   past end of stream if last tokens didn't have a semiExp termination
*   character
* ver 3.0 : 29 Jan 2016
* - built in help session, Friday afternoon
*
* Planned Additions and Changes:
* ------------------------------
* - implement ItokCollection interface
* - add public :, protected :, private : as terminators
* - move creation of tokenizer into semiExp constructor so
*   client doesn't have to write that code.
*/

#include <vector>
#include "../Tokenizer/Tokenizer.h"
#include "itokcollection.h"
#include <string>
namespace Scanner
{
    using Token = std::string;

    class SemiExp : public ITokCollection
    {
    public:
        SemiExp(Toker* pToker = nullptr);
        SemiExp(const SemiExp&) = delete;
        SemiExp& operator=(const SemiExp&) = delete;
        virtual bool get(bool clear = true);
        virtual size_t length();
        virtual std::string& operator[] (size_t n);
        std::string operator[](size_t n) const;
        virtual size_t find(const std::string& tok);
        virtual void push_back(const std::string&tok);
        virtual bool remove(const std::string& tok);
        virtual bool remove(size_t i);
        virtual void toLower();
        virtual void trimFront();
        virtual void clear();
        virtual std::string show(bool showNewLines = false);
        bool isComment(const std::string& tok);
    private:
        bool detectForLoop(std::vector<Token> _tokens);
        std::vector<Token> _tokens;
        Toker* _pToker;
        bool isTerminator(Token token);
    };
}
#endif
