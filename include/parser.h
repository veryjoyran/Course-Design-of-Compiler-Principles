#ifndef _PARSER_H
#define _PARSER_H

#include <bits/stdc++.h>
#include "grammar.h"
#include "lexer.h"
#include "symbolTable.h"
#include "optimizequa.h"

// Action 结构体，用于存储操作信息
struct Action {
    int pos;        // 操作位置
    int type = 0;   // 操作类型，默认为 0
    int object;     // 操作对象
    int tokenid;    // 对应的标记 ID
};

// Quat 结构体，用于表示四元式
struct Quat {
    int op, a, b, res;  // 操作符，操作数1，操作数2，结果
    Quat(int op = 0, int a = 0, int b = 0, int res = 0) 
        : op(op), a(a), b(b), res(res) {}
};

// 操作类型的枚举
enum { GEQ = 1, PUSH, ASSI, IF, ELSE, ENDIF, LAB, GOTO, WE, WH, DO };

// 解析器类
class Parser {
public:
    Parser(const std::vector<Token>& tokens, const Grammar& grammar, const Lexer& lexer, const SymTable& symtbl)
        : tokens(tokens), gram(grammar), lex(lexer), symtbl(symtbl) {
        Get_LL1Table();
    }

    void Get_Action();                       // 获取操作
    void Debug_LL1(std::stack<int>, int, int); // 调试 LL1 解析
    void Get_LL1Table();                     // 获取 LL1 分析表
    void LL1();                              // LL1 解析
    void GEQ(int, int);                      // GEQ 操作
    void PUSH(int, int);                     // PUSH 操作
    void ASSI();                             // ASSI 操作
    void IF();                               // IF 操作
    void ELSE();                             // ELSE 操作
    void ENDIF();                            // ENDIF 操作
    void LAB();                              // LAB 操作
    void GOTO();                             // GOTO 操作
    void WE();                               // WHILE 结束操作
    void WH();                               // WHILE 开始操作
    void DO();                               // DO 操作
    void Get_Quats();                        // 获取四元式
    void Print_Quat(Quat);                   // 打印四元式
    void Print_Act(Action);                  // 打印操作
    void Debug_Quats(std::stack<int>, std::stack<Action>, int, int); // 调试四元式
    int Get_Type(int, int);                  // 获取类型

    std::map<std::pair<int, int>, int> LL1Table; // LL1 分析表
    Grammar gram;                            // 文法对象
    Lexer lex;                               // 词法分析器对象
    std::vector<Token> tokens;               // 标记向量
    std::stack<int> SEM;                     // 语义栈
    std::stack<int> SToken;                  // 标记栈
    int cnt_t = 0;                           // 临时变量计数器
    std::vector<Quat> quats;                 // 四元式向量
    Action acts[maxn];                       // 操作数组
    std::vector<std::string> acttype{"GEQ", "PUSH", "ASSI", "IF", "ELSE", "ENDIF", "LAB", "GOTO", "WE", "WH", "DO"}; // 操作类型字符串
    SymTable symtbl;                         // 符号表
};

#endif // _PARSER_H
