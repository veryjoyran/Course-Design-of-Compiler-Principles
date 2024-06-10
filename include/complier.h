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
void parser_test();
void run();


class Complier {
public:
    Grammar grammar;
    string origin;
    Lexer lexer;
    vector<Token> tokens;
    Parser parser;
    vector<Quat> quats;
    SymTable symtbl;
    vector<string> lines;
    Opt opt;
    LexicalAnalyzer lexical;


    int get_opt_type(string op);
    string get_t(string str);
    void Init_Gram();
    void Lexical_Analysis();
    void Grammatical_Analysis();
    void Init_SymTable();
    void Get_Quats();
    void Init_opt();
};





#endif