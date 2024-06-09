#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H

#include <bits/stdc++.h>
#include "lexer.h"

using namespace std;

enum { INT, DOUBLE, CHAR, BOOL, ARRAY, FUNCTION, STRUCT };

class Record {
public:
    int name;
    int type;
    int range;
    int line = 0;
    int ival;
    double dval;
    char cval;
    bool bval;
    int low, up, ctp, clen;
    int level = 0, foff, fn = 0, param, entry;

    void Init_Var(double v) {
        switch (type) {
            case INT: ival = v; break;
            case DOUBLE: dval = v; break;
            case CHAR: cval = v; break;
            case BOOL: bval = v; break;
        }
    }

    void Init_Arr(int a, int b, int c, int d) {
        type = ARRAY;
        low = a;
        up = b;
        ctp = c;
        clen = d;
    }

    void Init_Fun(int a, int b, int c, int d) {
        type = FUNCTION;
        foff = a;
        fn = b;
        param = c;
        entry = d;
    }
};

class SymTable {
public:
    Lexer lex;
    vector<map<int, vector<Record>>> Infos;
    vector<string> lines;
    set<string> Def{"int", "double", "char", "bool"};
    vector<Record> synbl;
    vector<Record> arr;

    void Get_SymTable();
    void Get_NewRecord(const string& type, int range, vector<Record>& S, int line, int name);
    void Pop_Record(int range, map<int, vector<Record>>& InfoTable);
    void Print_Table(const map<int, vector<Record>>& InfoTable);
    void Print_RegularTable();
};

#endif // _SYMBOLTABLE_H
