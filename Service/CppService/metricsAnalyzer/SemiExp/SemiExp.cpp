///////////////////////////////////////////////////////////////////////
// SemiExpression.cpp - collect tokens for analysis                  //
// ver 3.7                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Xing Zhou, Computer Engineering SU, Graduate         //
// source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <exception>
#include "SemiExp.h"
#include "../Tokenizer/Tokenizer.h"

using namespace Scanner;

// construtor 
SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {}

// return a SemiExpression consisting of a series of tokens
bool SemiExp::get(bool clear)
{
    if (_pToker == nullptr)
        throw(std::logic_error("no Toker reference"));

    if (clear)   _tokens.clear();


    while (true)
    {
        std::string token = _pToker->getTok();
        if (token == "")
            break;
        _tokens.push_back(token);
        if (isTerminator(token))
            return true;

    }
    return false;
}

// find out if this token is terminator
bool SemiExp::isTerminator(Token token)
{

    if (token == ":" && _tokens.size() > 1)
    {
        std::vector<Token>::iterator prev_token;
        prev_token = _tokens.end() - 2;

        if (*prev_token == "public" || *prev_token == "protected"
            || *prev_token == "private")
            return true;
    }

    if (token == "{")
        return true;
    if (token == "}")
        return true;
    if (token == ";")
        if (!detectForLoop(_tokens))
            return true;
    if (token == "\n")
    {
        trimFront();
        if (_tokens[0] == "#")
            return true;
    }

    return false;
}

// detect whether this is the for(;;) case 
bool  SemiExp::detectForLoop(std::vector<Token> _tokens)
{
    std::vector<Token>::const_iterator i;
    int count = 0;
    // calculate how many ; before this ;
    for (i = _tokens.begin(); i != _tokens.end() - 1; i++)
    {
        if (*i == ";")
            count++;
    }
    if (count == 2)    // there are two ; before, the for loop is over
        return false;

    // detect whether this ; is in for loop
    for (i = _tokens.begin(); i != _tokens.end() - 1; i++)
    {
        if (*i == "for" && *(i + 1) == "(")
            return true;
    }

    return false;
}

// return a token by index, and can be assigned
Token& SemiExp::operator[](size_t n)
{
    if (n < 0 || size_t(n) >= _tokens.size())
        throw(std::invalid_argument("index out of range"));
    return _tokens[n];
}
// readonly
Token SemiExp::operator[](size_t n) const
{
    if (n < 0 || n >= _tokens.size())
        throw(std::invalid_argument("index out of range"));
    return _tokens[n];
}
// return the length of the vector
size_t SemiExp::length()
{
    return _tokens.size();
}
// This function will return the index of a particular token 
// in the vector<string>. return -1 if not found
size_t SemiExp::find(const std::string& tok)
{
    for (size_t i = 0; i < _tokens.size(); i++)
    {
        if (_tokens.at(i) == tok)
            return i;
    }
    return length();
}
bool SemiExp::isComment(const std::string& tok)
{
    return _pToker->isComment(tok);
}
// push a token in the end of the vector
void SemiExp::push_back(const std::string&tok)
{
    _tokens.push_back(tok);
}

// display on the console window the tokens
std::string SemiExp::show(bool showNewLines)
{
    std::ostringstream out;
    out << "\n  ";
    for (auto token : _tokens)
    {
        if (!showNewLines)
        {
            if (token != "\n")
                out << token << " ";
        }
        else
            out << token << " ";
    }
    out << "\n";
    return out.str();
}

// remove token by name
bool SemiExp::remove(const std::string& tok)
{
    std::vector<Token>::iterator i;
    for (i = _tokens.begin(); i != _tokens.end(); i++)
    {
        if (*i == tok)
        {
            _tokens.erase(i);
            return true;
        }
    }
    return false;
}
// remove token by index
bool SemiExp::remove(size_t i)
{
    if (i < 0 || i >= _tokens.size())
    {
        throw(std::invalid_argument("index out of range"));
        return false;
    }
    _tokens.erase(_tokens.begin() + i );
    return true;
}

// transfer all tokens into lower case
void SemiExp::toLower()
{
    for (auto token : _tokens)
    {
        for (size_t i = 0; i < token.size(); i++)
        {
            if (65 <= token[i] && token[i] <= 90)
                token[i] += 32;
        }
    }
}

// if a block a SemiExpression has new line , i.e "\n" 
// in the front, then remove them all
void SemiExp::trimFront()
{
    std::vector<Token>::iterator i = _tokens.begin();
    while (i != (_tokens.end() - 1) )  // if go to the last element, we can not delete
    {
        if (*i == "\n")
            i = _tokens.erase(i);
        else
            i++;
    }
}

// clear the _token vector's content
void Scanner::SemiExp::clear()
{
    _tokens.clear();
}

//----< test stub >--------------------------------------------------

#ifdef TEST_SEMIEXP
int main()
{
    // initialization :
    Toker toker;
    //std::string fileSpec = "../Tokenizer/Tokenizer.cpp";

    // test isTerminator function
    //std::string fileSpec = "testIsTerminator.txt";

    // test detectForloop function
   // std::string fileSpec = "../TestExecutive/testFileStore/testLocaiton1/testLocation1.txt";
    std::string fileSpec = "../TestExecutive/testFileStore/testLocaiton3/sert.txt";


    std::fstream in(fileSpec);
    if (!in.good())
    {
        std::cout << "\n  can't open file " << fileSpec << "\n\n";
        return 1;
    }
    toker.attach(&in);

    SemiExp semi(&toker);


    while (semi.get())
    {
        std::cout << "\n  -- semiExpression --";
        std::cout << semi.show();
    }
    /*
    May have collected tokens, but reached end of stream
    before finding SemiExp terminator.
    */
    if (semi.length() > 0)
    {
        std::cout << "\n  -- semiExpression --";
        std::cout << semi.show();
        std::cout << "\n\n";
    }

    // test find function
    std::vector<std::string> testFind =
    {
        "\n", "<", ">", "{", "}", "[", "]", "(", ")", ":", "=", "+", "-", "*", "."
    };
    size_t position = semi.find(">");
    if (position == 2)
        std::cout << "\n  find function is correct" << std::endl;


    return 0;
}
#endif

