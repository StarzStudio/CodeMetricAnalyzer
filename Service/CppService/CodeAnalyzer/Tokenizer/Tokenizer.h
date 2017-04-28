#ifndef TOKENIZER_H
#define TOKENIZER_H
///////////////////////////////////////////////////////////////////////
// Tokenizer.h - read words from a std::stream                       //
// ver 4.1                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Xing Zhou, Computer Engineering SU, Graduate         //
// source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a public Toker class and private ConsumeState class.
* Toker reads words from a std::stream, throws away whitespace and comments
* and returns words from the stream in the order encountered.  Quoted
* strings and certain punctuators and newlines are returned as single tokens.
*
* This is a new version, based on the State Design Pattern.  Older versions
* exist, based on an informal state machine design.
* 
* Public Interface:
* ------------------
* bool attach(std::istream* pIn);              // bind a new file
* std::string getTok();                        // return current collected token
* bool canRead();                              // detect whether the file can still be read
* void showSpecialSingleChar();                // display Special Char token collection
* void setSpecialSingleChars(std::string ssc); // set  Special Char token collection 
* void Clear();                                // clear file stream
* void returnComment(bool returnComment);      // turn on/off return Comment option
* size_t currentLineCount();                   // get current tokernizing postion line number
* bool isComment(const std::string& tok);      // detect whether this is a piece of comment token
* void resetLineCount();                       // can be called from upper level to reset LineCount back to 1
*
* Build Process:
* --------------
* Required Files: Tokenizer.h, Tokenizer.cpp
*
* Build Command: devenv Project2.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 4.1 : 12 Mar 2016
* - provide a resetLineCount function to reset static member LineCount from 
*   outside of this package after starting a new file to tokerize. Otherwise 
*   the LineCount will count based the last line number of the previous file
* ver 4.0 : 11 Mar 2016
* - change the EatDuoString and EatSingleString not to discard " or '
* - fix a bug in EatDuoString when detect '\\' case
* ver 3.9 : 10 Mar 2016
* - add linecount facility to record the current tokernizing position
* - each state eatchar function provide function to detect the \n and increase linecount by one
* ver 3.8 : 08 Feb 2016
* - provide returnComment function, add static var bool discardComment
* ver 3.7 : 07 Feb 2016
* - add two new state: EatDuoQuoString and EatSinQuoString to deal with ""
*   string and '' string, which also takes care of the situation which has escape '"
*   inside the string
* - fix a bug of null string "" case, by change do {} to while (){}.
* ver 3.6 : 06 Feb 2016
* - change SpecialSingleChar and SpecialCharPairs vector to string type
* - update the findSpecialChar, transfer int input to string type using string
*   stream.
* - fix a bug that findSpecialChar can not find '\n' ,change the way to transfer
*   input type
* ver 3.5 : 05 Feb 2016
* - add SpecialSingleChar and SpecialCharPairs list as container of defined
*   special character cases (in ConsumeState Class)
* - provide methods: setSpecialSingleChar and setSpecialCharPairs for both
*   list to add new case (in ConsumeState Class)
*   add findSpecialChar function and IsCharPair function to detect special char
* - replace the concrete EatNewLine class with EatSpecial class which utilizes
*   above two functions, along change the nextState()
* ver 3.4 : 03 Feb 2016
* - fixed bug that prevented reading of last character in source by
*   clearing stream errors at beginning of ConsumeState::nextState()
* ver 3.3 : 02 Feb 2016
* - declared ConsumeState copy constructor and assignment operator = delete
* ver 3.2 : 28 Jan 2016
* - fixed bug in ConsumeState::nextState() by returning a valid state
*   pointer if all tests fail due to reaching end of file instead of
*   throwing logic_error exception.
* ver 3.1 : 27 Jan 2016
* - fixed bug in EatCComment::eatChars()
* - removed redundant statements assigning _pState in derived eatChars()
*   functions
* - removed calls to nextState() in each derived eatChars() and fixed
*   call to nextState() in ConsumeState::consumeChars()
* ver 3.0 : 11 Jun 2014
* - first release of new design
*
* Planned Additions and Changes:
* ------------------------------
* - provide option to return comments
* - Return quoted strings as single token.  This must handle \" and \'
*   correctly.
* - Consider converting eatNewLine() to eatSpecialTokens() - see below
* - Return [, ], {, }, (, ), <, >, :, ; as single character tokens
* - Return <<, >>, +=, -=, *=, /=, :: as two character tokens
*/
#include <iosfwd>
#include <string>
#include <vector>

namespace Scanner
{
    class ConsumeState;

    class Toker
    {
    public:
        Toker();
        Toker(const Toker&) = delete;
        ~Toker();
        Toker& operator=(const Toker&) = delete;
        bool attach(std::istream* pIn);
        std::string getTok();
        bool canRead();
        void showSpecialSingleChar();
        void showSpecialCharPairs();
        void setSpecialSingleChars(std::string ssc);
        void setSpecialCharPairs(std::string scp);
        void Clear();
        void returnComment(bool returnComment);
        size_t currentLineCount();
        bool isComment(const std::string& tok);
        void resetLineCount();
    private:
        ConsumeState* pConsumer;

    };
}
#endif
