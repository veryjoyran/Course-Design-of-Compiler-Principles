#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H

#include <bits/stdc++.h>
#include "lexer.h"

using namespace std;

// 枚举类型，用于表示不同的数据类型
enum { INT, DOUBLE, CHAR, BOOL, ARRAY, FUNCTION, STRUCT };

// 记录类，表示符号表中的一条记录
class Record {
public:
    int name;   // 变量名的ID
    int type;   // 变量的类型（INT、DOUBLE、CHAR、BOOL、ARRAY、FUNCTION、STRUCT）
    int range;  // 变量的作用域范围
    int line = 0; // 变量所在行
    int ival;   // 整数类型的值
    double dval; // 双精度类型的值
    char cval;  // 字符类型的值
    bool bval;  // 布尔类型的值
    int low, up, ctp, clen; // 数组相关的下界、上界、类型和长度
    int level = 0, foff, fn = 0, param, entry; // 函数相关的级别、偏移量、函数编号、参数和入口

    // 初始化变量的值
    void Init_Var(double v) {
        switch (type) {
            case INT: ival = v; break;
            case DOUBLE: dval = v; break;
            case CHAR: cval = v; break;
            case BOOL: bval = v; break;
        }
    }

    // 初始化数组的属性
    void Init_Arr(int a, int b, int c, int d) {
        type = ARRAY;
        low = a;
        up = b;
        ctp = c;
        clen = d;
    }

    // 初始化函数的属性
    void Init_Fun(int a, int b, int c, int d) {
        type = FUNCTION;
        foff = a;
        fn = b;
        param = c;
        entry = d;
    }
};

// 符号表类，管理所有的符号表记录
class SymTable {
public:
    Lexer lex; // 词法分析器对象
    vector<map<int, vector<Record>>> Infos; // 存储符号表信息
    vector<string> lines; // 存储输入的代码行
    set<string> Def{"int", "double", "char", "bool"}; // 定义的基本类型
    vector<Record> synbl; // 存储符号表记录
    vector<Record> arr; // 存储数组记录

    // 构造函数
    SymTable(const Lexer& lexer)
        : lex(lexer) {}

    // 获取符号表
    void Get_SymTable();
    // 创建新的记录并添加到符号表中
    void Get_NewRecord(const string& type, int range, vector<Record>& S, int line, int name);
    // 弹出指定范围的记录
    void Pop_Record(int range, map<int, vector<Record>>& InfoTable);
    // 打印符号表
    void Print_Table(const map<int, vector<Record>>& InfoTable);
    // 打印标准符号表
    void Print_RegularTable();
};

#endif // _SYMBOLTABLE_H
