#ifndef _LEXICAL_H
#define _LEXICAL_H

#include <cstdio>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <stack>
#include <iostream>
#include <utility>
#include <map>
#include <iomanip>

class LexicalAnalyzer {
public:
    struct Token {
        int type;
        int index;
        int Vt_id;
        Token(int a = 0, int b = 0, int c = 0) : type(a), index(b), Vt_id(c) {}
    };

    LexicalAnalyzer() : state(0), level(0), fnum(0), array_len(0), Vt_num(4), error_flag(0), file_flag(0), i(0) {
        Vt["Iden"] = 0;
        Vt["Num"] = 1;
        Vt["Strc"] = 2;
        Vt["Charc"] = 3;
    }

   void lexical_analysis(std::ifstream& infile, std::ofstream& outfile, std::vector<std::string>& lines, std::string& S);

private:
    std::vector<std::string> TOKEN_k{"int", "double", "char", "bool","if", "else", "goto", "while"}; // 关键词表
    std::vector<std::string> TOKEN_p{",", ":", ";", "=", "+", "-", "{", "}", "*", "/", "(", ")", "#", ">", "<", "==", ">=", "<=", "!="}; // 界符表
    std::vector<std::string> TOKEN_i; // 标识符表
    std::vector<std::string> TOKEN_c; // 数字常量表
    std::vector<std::string> TOKEN_strc; // 字符串常量表
    std::vector<std::string> TOKEN_charc; // 字符常量表
    std::map<std::string, int> Vt; // 所有终结符类型的映射
    std::vector<Token> tokens;

    int state, level, fnum, array_len, Vt_num;
    int error_flag; // 编译错误标记
    int file_flag; // 文件读取的标记
    unsigned int i;
    std::string str;
    std::string input_str;

    bool is_Char(char c); // 判断是否是字符
    bool is_Dig(char c); // 判断是否是数字
    void next_state(char ch); // 自动机状态转移函数
    bool is_KI(); // 判断是否为关键词或标识符
    bool is_C(); // 判断是否为数字常量
    bool is_CHARC(); // 判断是否为字符常量
    bool is_STR_C(); // 判断是否为字符串常量
    bool is_P(); // 判断是否为界符
};





#endif // _LEXICAL_H
