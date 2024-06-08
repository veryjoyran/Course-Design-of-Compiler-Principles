#include "lexer.h"
#include <iostream>
#include <sstream>

Lexer::Lexer(const std::string& input) : sourceCode(input) {}

int Lexer::getSymbolId(const std::string& str) {
    for (int i = 0; i < symbolNames.size(); ++i) {
        if (symbolNames[i] == str) {
            return i;
        }
    }
    symbolNames.push_back(str);
    return symbolNames.size() - 1;
}

int Lexer::getPosition(int category, int id) {
    for (int j = 0; j < symbolTable[category].size(); ++j) {
        if (symbolTable[category][j] == id) {
            return j;
        }
    }
    symbolTable[category].push_back(id);
    return symbolTable[category].size() - 1;
}

void Lexer::tokenize() {
    std::stringstream stream(sourceCode);
    std::string word;

    while (stream >> word) {
        if (isdigit(word[0])) {  // Number constants
            tokens.push_back(Token(0, getPosition(0, getSymbolId(word)), grammar.Id("Num")));
        } else if (keywords.count(word)) {  // Keywords
            tokens.push_back(Token(1, getPosition(1, getSymbolId(word)), grammar.Id(word)));
        } else if (delimiters.count(word)) {  // Delimiters
            tokens.push_back(Token(2, getPosition(2, getSymbolId(word)), grammar.Id(word)));
        } else {  // Identifiers
            tokens.push_back(Token(3, getPosition(3, getSymbolId(word)), grammar.Id("Iden")));
            identifiers.insert(word);
        }
    }
}
