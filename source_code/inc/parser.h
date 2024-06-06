#ifndef _PARSER_H
#define _PARSER_H

#include <iostream>
#include <stack>
#include <vector>
// #include <bits/stdc++.h>     
// #include "Grammar.h"         // 包含文法相关定义的头文件
// #include "Lexer.h"           // 包含词法分析器相关定义的头文件
// #include "SymbolTable.h"     // 包含符号表相关定义的头文件
// #include "optimizequa.h"     // 包含四元式优化相关定义的头文件
using namespace std;

struct Action //语义动作
{
    int pos;
    int type = 0;
    int object;
    int token_id;
};

struct Quat //四元式结构体
{
    int op;
    int a;
    int b;
    int res;

    Quat(int op = 0, int a = 0, int b = 0, int res = 0) :
        op(op), a(a), b(b), res(res) { }    
};

enum { GEQ = 1, PUSH, ASSI, IF, ELSE, ENDIF, LAB, GOTO, WE, WH, DO };

struct Parser
{
    //map<pair<int, int>, int> LL1Table; // LL1分析表，用于语法分析
    //Grammar gram;                      // 文法实例
    //Lexer lex;                         // 词法分析器实例
    //vector<Token> tokens;              // 记号流，存储词法分析生成的记号
    stack<int> SEM;                    // 语义栈，用于语义分析
    stack<int> SToken;                 // 符号栈，用于语法分析
    int cnt_t = 0;                     // 临时变量计数器，用于编码t1, t2等
    vector<Quat> quats;                // 存储生成的四元式集合
    //Action acts[maxn];                 // 语义动作数组
    vector<string> acttype{"GEQ", "PUSH", "ASSI", "IF", "ELSE", "ENDIF", "LAB", "GOTO",
                           "WE", "WH", "DO"}; // 动作类型字符串表示
    //SymTable symtbl; 

    void Get_Action();                      // 获取语义动作
    void Debug_LL1(stack<int>, int, int);   // 调试LL1分析过程
    void Get_LL1Table();                    // 生成LL1分析表
    void LL1();                             // 执行LL1语法分析
    void GEQ(int, int);                     // 处理GEQ动作
    void PUSH(int, int);                    // 处理PUSH动作
    void ASSI();                            // 处理ASSI动作
    void IF();                              // 处理IF动作
    void ELSE();                            // 处理ELSE动作
    void ENDIF();                           // 处理ENDIF动作
    void LAB();                             // 处理LAB动作
    void GOTO();                            // 处理GOTO动作
    void WE();                              // 处理WE动作
    void WH();                              // 处理WH动作
    void DO();                              // 处理DO动作

    void Get_Quats();                       // 获取四元式
    void Print_Quat(Quat);                  // 打印四元式
    void Print_Act(Action);                 // 打印语义动作
    void Debug_Quats(stack<int>, stack<Action>, int, int); // 调试四元式
    int Get_Type(int, int);                 // 获取类型
    
};


#endif //_PARSER_H