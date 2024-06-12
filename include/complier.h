#ifndef _COMPLIER_H
#define _COMPLIER_H

#include <bits/stdc++.h>
#include "grammar.h"
#include "lexer.h"
#include "parser.h"
#include "symbolTable.h"
#include "optimizequa.h"
#include "lexical.h"

void lexical_test();
void grammar_test();
void lexer_test();
void symbolTable_test();
void parser_test();
void run();


class Complier {
public:
    Grammar grammar;
    std::string origin;
    Lexer lexer;
    std::vector<Token> tokens;
    Parser parser;
    std::vector<Quat> quats;
    SymTable symtbl;
    std::vector<std::string> lines;
    Opt opt;

    // 默认构造函数
    Complier() {}

    int get_opt_type(std::string op);
    std::string get_t(std::string str);
    void Init_Gram();
    void Lexical_Analysis();
    void Grammatical_Analysis();
    void Init_SymTable();
    void Get_Quats();
    void Init_opt();
};



#endif