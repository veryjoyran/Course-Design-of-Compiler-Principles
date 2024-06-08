#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include "grammar.h"

struct Token {
    int index;
    int position;
    int symbolId;
    int type = -1;
    int level;

    Token(int idx = 0, int pos = 0, int symId = 0) : index(idx), position(pos), symbolId(symId) {}
};

class Lexer {
public:
    Lexer(const std::string& input);
    int getSymbolId(const std::string& str);
    int getPosition(int category, int id);
    Grammar grammar;

    void tokenize();


    std::string sourceCode;
    std::vector<std::string> symbolNames;
    std::vector<int> symbolTable[6]; // Array of vectors for different categories
    std::vector<Token> tokens;
    std::set<std::string> keywords{"int", "double", "char", "if", "else", "goto", "while"};
    std::set<std::string> delimiters{",", ":", ";", "=", "+", "-", "{", "}", "*", "/", "(", ")", "#", ">", "<", "==", ">=", "<=", "!="};
    std::set<std::string> identifiers;

    
};

#endif // LEXER_H
