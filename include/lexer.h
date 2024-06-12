#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include "grammar.h"
#include "lexical.h"

struct Token {
    int i;
    int j;
    int Vt_id;
    int type = -1;
    int level=0;

    Token(int i = 0, int j = 0, int Vt_id = 0) : i(i), j(j), Vt_id(Vt_id) {}
};

class Lexer {
public:
    Lexer() : 
        grammar(),
        sourceCode(""),
        symbolNames(),
        tokens(),
        state(0),
        str(""),
        error_flag(false),
        i(0) 
    {
        for (int j = 0; j < 6; ++j) {
            symbolTable[j] = std::vector<int>();
        }
    }
     Lexer(const std::string& input) : 
        grammar(),
        sourceCode(input),
        symbolNames(),
        tokens(),
        state(0),
        str(""),
        error_flag(false),
        i(0) 
    {
        for (int j = 0; j < 6; ++j) {
            symbolTable[j] = std::vector<int>();
        }
    }
    int getId(const std::string& str);
    int getPos(int category, int id);
    void getTokens();
    bool is_Char(char c);
    bool is_Dig(char c);
    void next_state(char ch);
    

    Grammar grammar;
    std::string sourceCode;
    std::vector<std::string> symbolNames;
    std::vector<int> symbolTable[6]; // Array of vectors for different categories
    std::vector<Token> tokens;
    std::set<std::string> keywords{"int", "double", "char", "bool","if", "else", "goto", "while"};
    std::set<std::string> delimiters{",", ":", ";", "=", "+", "-", "{", "}", "*", "/", "(", ")", "#", ">", "<", "==", ">=", "<=", "!="};
    std::set<std::string> identifiers;
    int state = 0;
    std::string str;
    bool error_flag = false;
    int i = 0;
};

#endif // LEXER_H
