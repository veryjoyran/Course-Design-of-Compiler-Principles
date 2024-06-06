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

    void lexical_analysis(std::ifstream& infile, std::vector<std::string>& lines, std::string& S) ;

private:
    std::vector<std::string> TOKEN_k; // 关键词表
    std::vector<std::string> TOKEN_p; // 界符表
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

bool LexicalAnalyzer::is_Char(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool LexicalAnalyzer::is_Dig(char c) {
    return c >= '0' && c <= '9';
}

void LexicalAnalyzer::next_state(char ch) {
    if (state == 0) {
        if (is_Char(ch)) { state = 1; str.push_back(ch); }
        else if (is_Dig(ch)) { state = 3; str.push_back(ch); }
        else if (ch == '=') { state = 8; str.push_back(ch); }
        else if (ch == '>') { state = 11; str.push_back(ch); }
        else if (ch == '<') { state = 14; str.push_back(ch); }
        else if (ch == '+') { state = 17; str.push_back(ch); }
        else if (ch == '-') { state = 21; str.push_back(ch); }
        else if (ch == '*') { state = 25; str.push_back(ch); }
        else if (ch == '/') { state = 28; str.push_back(ch); }
        else if (ch == '{') { state = 31; str.push_back(ch); }
        else if (ch == '}') { state = 32; str.push_back(ch); }
        else if (ch == '(') { state = 33; str.push_back(ch); }
        else if (ch == ')') { state = 34; str.push_back(ch); }
        else if (ch == '[') { state = 35; str.push_back(ch); }
        else if (ch == ']') { state = 36; str.push_back(ch); }
        else if (ch == ',') { state = 37; str.push_back(ch); }
        else if (ch == ';') { state = 38; str.push_back(ch); }
        else if (ch == 39) { state = 39; str.push_back(ch); }
        else if (ch == 34) { state = 41; str.push_back(ch); }
    } else if (state == 1) {
        if (is_Char(ch) || is_Dig(ch) || ch == '_') { state = 1; str.push_back(ch); }
        else if (ch == '[') {
            state = 43; str.push_back(ch);
        } else {
            state = 2; i--;
        }
    } else if (state == 3) {
        if (is_Dig(ch)) { state = 3; str.push_back(ch); }
        else if (ch == '.') { state = 4; str.push_back(ch); }
        else {
            state = 7; i--;
        }
    } else if (state == 4) {
        if (is_Dig(ch)) { state = 5; str.push_back(ch); }
        else {
            error_flag = 1;
        }
    } else if (state == 5) {
        if (is_Dig(ch)) { state = 5; str.push_back(ch); }
        else {
            state = 6; i--;
        }
    } else if (state == 8) {
        if (ch == '=') { state = 9; str.push_back(ch); }
        else {
            state = 10; i--;
        }
    } else if (state == 11) {
        if (ch == '=') { state = 12; str.push_back(ch); }
        else {
            state = 13; i--;
        }
    } else if (state == 14) {
        if (ch == '=') { state = 15; str.push_back(ch); }
        else {
            state = 16; i--;
        }
    } else if (state == 17) {
        if (ch == '+') { state = 18; str.push_back(ch); }
        else if (ch == '=') { state = 19; str.push_back(ch); }
        else {
            state = 20; i--;
        }
    } else if (state == 21) {
        if (ch == '-') { state = 22; str.push_back(ch); }
        else if (ch == '=') { state = 23; str.push_back(ch); }
        else {
            state = 24; i--;
        }
    } else if (state == 25) {
        if (ch == '=') { state = 26; str.push_back(ch); }
        else {
            state = 27; i--;
        }
    } else if (state == 28) {
        if (ch == '=') { state = 29; str.push_back(ch); }
        else {
            state = 30; i--;
        }
    } else if (state == 39) {
        if (ch == 39) { state = 40; str.push_back(ch); }
        else {
            state = 39; str.push_back(ch);
        }
    } else if (state == 41) {
        if (ch == 34) { state = 42; str.push_back(ch); }
        else {
            state = 41; str.push_back(ch);
        }
    } else if (state == 43) {
        if (is_Dig(ch)) { state = 43; str.push_back(ch); }
        else if (ch == ']') { state = 2; str.push_back(ch); }
        else {
            error_flag = 1;
        }
    }
}

bool LexicalAnalyzer::is_KI() {
    return state == 2;
}

bool LexicalAnalyzer::is_C() {
    return state == 6 || state == 7;
}

bool LexicalAnalyzer::is_CHARC() {
    return state == 40;
}

bool LexicalAnalyzer::is_STR_C() {
    return state == 42;
}

bool LexicalAnalyzer::is_P() {
    return state == 9 || state == 10 || state == 12 || state == 13 || state == 15 || state == 16 ||
           (state >= 18 && state <= 20) || (state >= 22 && state <= 24) || state == 26 || state == 27 ||
           (state >= 29 && state <= 38);
}

void LexicalAnalyzer::lexical_analysis(std::ifstream& infile, std::vector<std::string>& lines, std::string& S) {
    state = 0;
    error_flag = 0;
    file_flag = 0;
    Vt_num = 4;
    Vt["Iden"] = 0;
    Vt["Num"] = 1;
    Vt["Strc"] = 2;
    Vt["Charc"] = 3;
    int line = 1;

    while (std::getline(infile, input_str)) {
        if (input_str.empty()) {
            continue;
        }
        printf("line %d: ", line++);

        std::vector<std::string> strs;

        if (input_str != "#") {
            i = 0;
            while (i < input_str.size()) {
                error_flag = 0;
                next_state(input_str[i]);
                if (error_flag == 1) {
                    break;
                }
                if (is_KI()) {
                    strs.push_back(str);
                    std::cout << str << " ";
                    bool flag_k = false, flag_i = false;
                    for (size_t j = 0; j < TOKEN_k.size(); ++j) {
                        if (str == TOKEN_k[j]) {
                            Token token(0, j, Vt[TOKEN_k[j]]);
                            flag_k = true;
                            tokens.push_back(token);
                        }
                    }
                    if (!flag_k) {
                        for (size_t j = 0; j < TOKEN_i.size(); ++j) {
                            if (str == TOKEN_i[j]) {
                                Token token(1, j, Vt["Iden"]);
                                flag_i = true;
                                tokens.push_back(token);
                            }
                        }
                        if (!flag_i) {
                            TOKEN_i.push_back(str);
                            Token token(1, TOKEN_i.size() - 1, Vt["Iden"]);
                            tokens.push_back(token);
                        }
                    }
                    state = 0;
                    str.clear();
                } else if (is_P()) {
                    strs.push_back(str);
                    std::cout << str << " ";
                    for (size_t j = 0; j < TOKEN_p.size(); ++j) {
                        if (str == TOKEN_p[j]) {
                            Token token(2, j, Vt[TOKEN_p[j]]);
                            tokens.push_back(token);
                        }
                    }
                    state = 0;
                    str.clear();
                } else if (is_C()) {
                    strs.push_back(str);
                    std::cout << str << " ";
                    bool flag_c = false;
                    for (size_t j = 0; j < TOKEN_c.size(); ++j) {
                        if (str == TOKEN_c[j]) {
                            Token token(3, j, Vt["Num"]);
                            flag_c = true;
                            tokens.push_back(token);
                        }
                    }
                    if (!flag_c) {
                        TOKEN_c.push_back(str);
                        Token token(3, TOKEN_c.size() - 1, Vt["Num"]);
                        tokens.push_back(token);
                    }
                    state = 0;
                    str.clear();
                } else if (is_STR_C()) {
                    strs.push_back(str);
                    std::cout << str << " ";
                    bool flag_strc = false;
                    for (size_t j = 0; j < TOKEN_strc.size(); ++j) {
                        if (str == TOKEN_strc[j]) {
                            Token token(4, j, Vt["Strc"]);
                            flag_strc = true;
                            tokens.push_back(token);
                        }
                    }
                    if (!flag_strc) {
                        TOKEN_strc.push_back(str);
                        Token token(4, TOKEN_strc.size() - 1, Vt["Strc"]);
                        tokens.push_back(token);
                    }
                    state = 0;
                    str.clear();
                } else if (is_CHARC()) {
                    strs.push_back(str);
                    std::cout << str << " ";
                    bool flag_charc = false;
                    for (size_t j = 0; j < TOKEN_charc.size(); ++j) {
                        if (str == TOKEN_charc[j]) {
                            Token token(5, j, Vt["Charc"]);
                            flag_charc = true;
                            tokens.push_back(token);
                        }
                    }
                    if (!flag_charc) {
                        TOKEN_charc.push_back(str);
                        Token token(5, TOKEN_charc.size() - 1, Vt["Charc"]);
                        tokens.push_back(token);
                    }
                    state = 0;
                    str.clear();
                }
                i++;
            }
            std::cout << std::endl;
            if (error_flag == 1) {
                printf("编译程序出错!!\n");
                break;
            }
        } else {
            // 如果输入的行是"#"，表示输入结束
            break;
        }

        std::string temp;
        for (auto& x : strs) {
            temp += x + " ";
            S += x + " ";
        }
        if (temp.empty()) {
            continue;
        }
        lines.push_back(temp);
    }

    for (size_t j = 0; j < tokens.size(); ++j) {
        printf("(%d , %d , %d)\n", tokens[j].type, tokens[j].index, tokens[j].Vt_id);
    }
}





#endif // _LEXICAL_H
