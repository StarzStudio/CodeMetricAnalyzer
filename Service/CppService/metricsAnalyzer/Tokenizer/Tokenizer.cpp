///////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                     //
// ver 4.1                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Xing Zhou, Computer Engineering SU, Graduate         //
// source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////


#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>
#include <sstream>

namespace Scanner
{
    class ConsumeState
    {
    public:
        ConsumeState();
        ConsumeState(const ConsumeState&) = delete;
        ConsumeState& operator=(const ConsumeState&) = delete;
        virtual ~ConsumeState();
        void attach(std::istream* pIn) { _pIn = pIn; }
        virtual void eatChars() = 0;
        void consumeChars() {
            _pState->eatChars();
            _pState = nextState();
        }
        bool canRead() { return _pIn->good(); }
        std::string getTok() { return token; }
        bool hasTok() { return token.size() > 0; }
        ConsumeState* nextState();
        bool findSpecialSingle(int currChar);
        bool isCharPairs(int currChar);
        void setSpecialSingleChars(std::string ssc);
        void setSpecialCharPairs(std::string scp);
        void returnComment(bool returnComment = false);
        void Clear();
        void showSpecialSingleChar();
        void showSpecialCharPairs();
        size_t currentLineCount();
        void resetLineCount()
        {
           lineCount = 1;
        }
    protected:
        static bool  discardComment;
        static std::string token;
        static std::istream* _pIn;
        static bool first_constructor;
        static bool first_destructor;
        // static int prevChar;
        static int currChar;
        static size_t lineCount;
        static ConsumeState* _pState;
        static ConsumeState* _pEatCppComment;
        static ConsumeState* _pEatCComment;
        static ConsumeState* _pEatWhitespace;
        static ConsumeState* _pEatPunctuator;
        static ConsumeState* _pEatAlphanum;
        static ConsumeState* _pEatSpecial;
        static ConsumeState* _pEatDouQuoString;
        static ConsumeState* _pEatSinQuoString;
        static std::vector<std::string> SpecialSingleChars;
        static std::vector<std::string> SpecialCharPairs;
        std::vector<std::string> ToSpecialCharSet(const std::string& src);
    };
}

using namespace Scanner;

size_t  ConsumeState::lineCount = 1;
bool  ConsumeState::discardComment = false;
std::string ConsumeState::token;
std::istream* ConsumeState::_pIn = nullptr;
bool ConsumeState::first_constructor = true;
bool ConsumeState::first_destructor = true;
//int ConsumeState::prevChar;
int ConsumeState::currChar;
ConsumeState* ConsumeState::_pState = nullptr;
ConsumeState* ConsumeState::_pEatCppComment = nullptr;
ConsumeState* ConsumeState::_pEatCComment = nullptr;
ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
ConsumeState* ConsumeState::_pEatSpecial = nullptr;
ConsumeState* ConsumeState::_pEatDouQuoString = nullptr;
ConsumeState* ConsumeState::_pEatSinQuoString = nullptr;
// initialize SpecialSingleChars vector.  Special one character tokens: <, >, [, ], (, ), {, }, :, =, +, -, *,. \n
std::vector<std::string> ConsumeState::SpecialSingleChars{ "\n", "<", ">", "{", "}", "[", "]", "(", ")", ":", "=", "+", "-", "*", "." };
//  initialize SpecialCharPairs vector.Special two character tokens : << , >> , ::, ++, --, == , +=, -=, *=, /=
std::vector<std::string> ConsumeState::SpecialCharPairs{ "<<", ">>", "::", "++", "--", "==", "+=", "-=", "*=", "/=" };

void testLog(const std::string& msg);

// add new case of special one char token into the predefined list
void ConsumeState::setSpecialSingleChars(std::string ssc)
{
    SpecialSingleChars.clear();
    SpecialSingleChars = ToSpecialCharSet(ssc);
}

// add new case of special two char token into the predefined list
void ConsumeState::setSpecialCharPairs(std::string scp)
{
    SpecialSingleChars.clear();
    SpecialCharPairs = ToSpecialCharSet(scp);
}
// utility function to convert input string to Special char set, use "," as a divider
std::vector<std::string> ConsumeState::ToSpecialCharSet(const std::string& src)
{
    std::vector<std::string> temp(src.size());
    size_t count = 0;
    for (size_t i = 0; i < src.size(); i++)
    {
        // discard white space
        if (isspace(src[i]) && src[i] != '\n')
            continue;
        // ',' is a divider
        if (src[i] != ',')
            temp[count] += src[i];
        else
        {
            count++;
        }
    }
    return temp;
}
// print out SpecialSingleChar, called by Toker::SpecialSingleChar
void ConsumeState::showSpecialSingleChar()
{
    for (size_t i = 0; i < SpecialSingleChars.size(); i++)
    {
        std::cout << SpecialSingleChars.at(i);
    }
}
//print out SpecialCharPairs, called by Toker::SpecialSingleChar  
void ConsumeState::showSpecialCharPairs()
{
    for (size_t i = 0; i < SpecialCharPairs.size(); i++)
    {
        std::cout << SpecialCharPairs.at(i);
    }
}

// decide whether to return Comment by set flag
void ConsumeState::returnComment(bool returnComment)
{
    discardComment = returnComment;
}

//----< called by Toker constructor >--------------------------------

void ConsumeState::Clear()
{
    first_constructor = first_destructor = true;
}
// to find whether current char is in special char list
bool ConsumeState::findSpecialSingle(int currChar)
{
    std::string str_currChar = "";
    char c_currChar = currChar;
    str_currChar += c_currChar;

    std::vector<std::string>::const_iterator i;
    for (i = SpecialSingleChars.begin(); i != SpecialSingleChars.end(); i++)
    {
        if (str_currChar.compare(*i) == 0)
            return true;
    }
    return false;
}

//  if findSpecialSingle is true , decide whether it's the case of char pairs
bool ConsumeState::isCharPairs(int currChar)
{
    if (findSpecialSingle(currChar) == false) return false;
    if (!_pIn->good())   return false;                      // end of stream

    std::stringstream temp, temp2;
    std::string str_currChar, str_nextChar;
    char c_currChar = currChar;
    temp << c_currChar;
    temp >> str_currChar;
    char c_nextChar = _pIn->peek();
    temp2 << c_nextChar;
    temp2 >> str_nextChar;
    std::string twoCharPair = str_nextChar + str_currChar;
    std::vector<std::string>::const_iterator i;
    for (i = SpecialCharPairs.begin(); i != SpecialCharPairs.end(); i++)
    {
        if (twoCharPair.compare(*i) == 0)
            return true;
    }
    return false;
}

ConsumeState* ConsumeState::nextState()
{
    if (!(_pIn->good()))
        return nullptr;
    int chNext = _pIn->peek();
    if (chNext == EOF)
    {
        _pIn->clear();
        // if peek() reads end of file character, EOF, then eofbit is set and
        // _pIn->good() will return false.  clear() restores state to good
    }
    if (std::isspace(currChar) && currChar != '\n')
        return _pEatWhitespace;
    if (currChar == '/' && chNext == '/')
        return _pEatCppComment;
    if (currChar == '/' && chNext == '*')
        return _pEatCComment;
    // this if condition must place above the if (ispunct)
    if (findSpecialSingle(currChar))
        return _pEatSpecial;
    if (currChar == '\"')
        return _pEatDouQuoString;
    if (currChar == '\'')
        return _pEatSinQuoString;
    if (std::isalnum(currChar))
        return _pEatAlphanum;
    if (ispunct(currChar))
        return _pEatPunctuator;
    if (!_pIn->good())                     // end of stream
        return _pEatWhitespace;
   // throw(std::logic_error("invalid type"));
}
// return number of newlines collected from stream 
size_t ConsumeState::currentLineCount()
{ 
    return lineCount;
}

class EatWhitespace : public ConsumeState
{
public:
    virtual void eatChars()
    {
        token.clear();
        //std::cout << "\n  eating whitespace";
        do {
            if (!_pIn->good())  // end of stream
                return;
            currChar = _pIn->get();
            if (currChar == '\n')
                ++lineCount;
        } while (std::isspace(currChar) && currChar != '\n');
    }
};

class EatCppComment : public ConsumeState
{
public:
    virtual void eatChars()
    {
        token.clear();
        //std::cout << "\n  eating C++ comment";
        do {
            if (discardComment)
                token += currChar;
            if (!_pIn->good())  // end of stream
                return;
            currChar = _pIn->get();
            if (currChar == '\n')
                ++lineCount;
        } while (currChar != '\n');
    }
};

class EatCComment : public ConsumeState
{
public:
    virtual void eatChars()
    {
        token.clear();
        //std::cout << "\n  eating C comment";
        do {
            if (discardComment)
                token += currChar;
            if (!_pIn->good())  // end of stream
                return;
            currChar = _pIn->get();
            if (currChar == '\n')
                ++lineCount;
        } while (currChar != '*' || _pIn->peek() != '/');
        if (discardComment)
        {
            token += currChar;
            token += _pIn->get();
            if (!_pIn->good())  // end of stream
                return;
            currChar = _pIn->get();
            if (currChar == '\n')
                ++lineCount;
        }
        else
        {
            _pIn->get();
            if (!_pIn->good())  // end of stream
                return;
            currChar = _pIn->get();
            if (currChar == '\n')
                ++lineCount;
        }
    }
};

class EatPunctuator : public ConsumeState
{
public:
    virtual void eatChars()
    {
        token.clear();
        //std::cout << "\n  eating punctuator";
        do {
            if (currChar == '\'')
            {
                return;
            }
            if (currChar == '\"')
            {
                return;
            }
            if (findSpecialSingle(currChar))
            {
                return;
            }
            token += currChar;
            if (!_pIn->good())  // end of stream
                return;
            currChar = _pIn->get();
            if (currChar == '\n')
                ++lineCount;
        } while (ispunct(currChar));
    }
};

class EatAlphanum : public ConsumeState
{
public:
    virtual void eatChars()
    {
        token.clear();
        //std::cout << "\n  eating alphanum";
        do {
            token += currChar;
            if (!_pIn->good())  // end of stream
                return;
            currChar = _pIn->get();
            if (currChar == '\n')
                ++lineCount;
        } while (isalnum(currChar));
    }
};

class EatSpecial : public ConsumeState
{
public:
    // eat special one or two chars token
    virtual void eatChars()
    {
        token.clear();

        token += currChar;

        if (!_pIn->good())          // end of stream
            return;

        if (isCharPairs(currChar))  // the case of special char pairs
        {
            currChar = _pIn->get();
            if (currChar == '\n')
                ++lineCount;
            token += currChar;
            if (!_pIn->good())          // end of stream
                return;
        }
        currChar = _pIn->get();
        if (currChar == '\n')
            ++lineCount;
    }
};

class EatDouQuoString : public ConsumeState
{
public:
    // eat double quoted string
    virtual void eatChars()
    {
        token.clear();
        //std::cout << "\n  eating string";
        // currChar = _pIn->get();         // discard "
        token += currChar;
        currChar = _pIn->get();
        while (currChar != '\"') {
            if (currChar == '\\')
            {
                /* if (_pIn->peek() == '\\' && _pIn->peek()->peek() == '\"')
                {
                token += currChar;
                currChar = _pIn->get();
                break;
                }*/
                if (_pIn->peek() == '\"')
                    currChar = _pIn->get();  
                if (_pIn->peek() == '\\')
                    currChar = _pIn->get();
            }
            token += currChar;
            currChar = _pIn->get();
        }

        if (!_pIn->good())  // end of stream
            return;
        token += currChar;
        currChar = _pIn->get();
        if (currChar == '\n')
            ++lineCount;
    }

};

class EatSinQuoString : public ConsumeState
{
public:
    // eat single quoted string
    virtual void eatChars()
    {
        token.clear();
        //std::cout << "\n  eating string";
        //currChar = _pIn->get();            // discard the open '
        token += currChar;
        currChar = _pIn->get();
        if (currChar == '\\')          // if '' contains \ ,then take in whatever succeed it
        {
            if (_pIn->peek() == '\'' &&_pIn->peek() == '\\')  // if this is the '\'' '\\'case, then discard proceeded \.
            {
                token += _pIn->get();  // add ' or '\'
            }
            else
            {
                token += currChar;    // if not the '\'' case, then we don't discard \.
                token += _pIn->get();
            }
        }
        else
            token += currChar;

        //_pIn->get();                 // discard the close '

        if (!_pIn->good())  // end of stream
            return;
        token += currChar;
        currChar = _pIn->get();      // get to char succeed to '
        if (currChar == '\n')
            ++lineCount;
    }

};

ConsumeState::ConsumeState()
{
    if (first_constructor)   // can be reset to true by clear()
    {
        first_constructor = false;
        _pEatAlphanum = new EatAlphanum();
        _pEatCComment = new EatCComment();
        _pEatCppComment = new EatCppComment();
        _pEatPunctuator = new EatPunctuator();
        _pEatWhitespace = new EatWhitespace();
        _pEatSpecial = new EatSpecial();
        _pEatDouQuoString = new EatDouQuoString();
        _pEatSinQuoString = new EatSinQuoString();
        _pState = _pEatWhitespace;
    }
}

ConsumeState::~ConsumeState()
{
    if (first_destructor) // can be reset to true by clear()
    {
        first_destructor = false;
        discardComment = false;
        _pIn = nullptr;
        delete _pEatAlphanum;
        delete _pEatCComment;
        delete _pEatCppComment;
        delete _pEatPunctuator;
        delete _pEatWhitespace;
        delete _pEatSpecial;
        delete _pEatDouQuoString;
        delete _pEatSinQuoString;
        SpecialSingleChars.clear();
        SpecialCharPairs.clear();
    }
}


Toker::Toker() : pConsumer(new EatWhitespace())
{
    Clear();
};

Toker::~Toker() { delete pConsumer; }

// reset static LineCount number back to the original state: 1. For tokernizing next file to use
void Toker::resetLineCount()
{
    pConsumer->resetLineCount();
}
// print out SpecialSingleChar
void Toker::showSpecialSingleChar()
{
    pConsumer->showSpecialSingleChar();
}
//print out SpecialCharPairs
void Toker::showSpecialCharPairs()
{
    pConsumer->showSpecialCharPairs();
}

// clear the static varible in ConsumeState class
void Toker::Clear()
{
    pConsumer->Clear();
}

// add new case of special one char token into the predefined list
void Toker::setSpecialSingleChars(std::string ssc)
{
    pConsumer->setSpecialSingleChars(ssc);
}

// add new case of special two char token into the predefined list
void Toker::setSpecialCharPairs(std::string scp)
{
    pConsumer->setSpecialCharPairs(scp);
}

// decide whether to return comments as tokens
void Toker::returnComment(bool returnComment)
{
    pConsumer->returnComment(returnComment);
}
// detect whether tok is a comment piece
bool Toker::isComment(const std::string& tok)
{
    if (tok.find("//") < tok.size() || tok.find("/*") < tok.size())
        return true;
    return false;
}
// bind stream with class
bool Toker::attach(std::istream* pIn)
{
    if (pIn != nullptr && pIn->good())
    {
        pConsumer->attach(pIn);
        return true;
    }
    return false;
}
// get a single token from stream
std::string Toker::getTok()
{
    while (true)
    {
        if (!pConsumer->canRead())
            return "";
        pConsumer->consumeChars();
        if (pConsumer->hasTok())
            break;
    }
    return pConsumer->getTok();
}

// return the current line number
size_t Toker::currentLineCount()
{
    return pConsumer->currentLineCount();
}
bool Toker::canRead() { return pConsumer->canRead(); }

//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
    EatSpecial state;
    // test findSpecialSingle function:
    int currChar_test1 = '{';
    if (state.findSpecialSingle(currChar_test1))
        std::cout << "find special single char successfully, function correct" << std::endl;

    Toker toker_2;
    //// test showSpecialCharPairs function:
    //toker_2.showSpecialSingleChar();
    //// test showSpecialCharPairs function:
    //toker_2.showSpecialCharPairs();

    //// test setSpecialSingleChars function:
    //std::string ssc = "<, >, [, ], (, )";
    //std::cout << "\n  default spcial singel char are: \n";
    //toker_2.showSpecialSingleChar();
    //toker_2.setSpecialSingleChars(ssc);
    //std::cout << "\n  After change, spcial singel char are: \n";
    //toker_2.showSpecialSingleChar();

    //// test setSpecialSingleChars function:
    //std::string scp = "<<, >>, ++, ++, --, -=";
    //std::cout << "\n  default spcial singel char are: \n";
    //toker_2.showSpecialCharPairs();
    //toker_2.setSpecialCharPairs(scp);
    //std::cout << "\n  After change, spcial singel char are: \n";
    //toker_2.showSpecialCharPairs();



    // because Clear function, new constructor will renew all the concrete state instance
    Toker toker;

    // test returnComment option
    // bool doReturn = true;
    // toker.returnComment(doReturn);
    // std::string fileSpec = "../Tokenizer/Tokenizer.h";

   //std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
    std::string fileSpec = "../TestExecutive/testFileStore/testLocaiton3/sert.txt";

    //std::string fileSpec = "../Tokenizer/Test.txt";
    //std::string fileSpec = "../Tokenizer/testForSpecialChar.txt";
    //std::string fileSpec = "../Tokenizer/testQuota.txt";
    //std::string fileSpec = "../Tokenizer/testReturnComment.txt";
    //std::string fileSpec = "../Tokenizer/totalOpenBrace.txt";
    // std::string fileSpec = "test[]instringcase.txt";

    std::ifstream in(fileSpec);
    if (!in.good())
    {
        std::cout << "\n  can't open " << fileSpec << "\n\n";
        return 1;
    }

    toker.attach(&in);
    do
    {
        std::string tok = toker.getTok();
        if (tok == "\n")
            tok = "newline";
        std::cout << "\n -- " << tok;
    } while (in.good());

    std::cout << "\n\n";






    return 0;
}
#endif
