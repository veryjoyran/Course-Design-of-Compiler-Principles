#include "lexical.h"
#include <bits/stdc++.h>


bool LexicalAnalyzer::is_Char(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool LexicalAnalyzer::is_Dig(char c) {
    return c >= '0' && c <= '9';
}

void LexicalAnalyzer::next_state(char ch) {
    // 初始状态，判断字符类型并进行相应状态转移
    if (state == 0) {
        // 如果是字母，进入标识符状态
        if (is_Char(ch)) { state = 1; str.push_back(ch); }
        // 如果是数字，进入数字状态
        else if (is_Dig(ch)) { state = 3; str.push_back(ch); }
        // 处理各种单运算符
        else if (ch == '=') { state = 8; str.push_back(ch); }
        else if (ch == '>') { state = 11; str.push_back(ch); }
        else if (ch == '<') { state = 14; str.push_back(ch); }
        else if (ch == '+') { state = 17; str.push_back(ch); }
        else if (ch == '-') { state = 21; str.push_back(ch); }
        else if (ch == '*') { state = 25; str.push_back(ch); }
        else if (ch == '/') { state = 28; str.push_back(ch); }
        // 处理各种界符
        else if (ch == '{') { state = 31; str.push_back(ch); }
        else if (ch == '}') { state = 32; str.push_back(ch); }
        else if (ch == '(') { state = 33; str.push_back(ch); }
        else if (ch == ')') { state = 34; str.push_back(ch); }
        else if (ch == '[') { state = 35; str.push_back(ch); }
        else if (ch == ']') { state = 36; str.push_back(ch); }
        else if (ch == ',') { state = 37; str.push_back(ch); }
        else if (ch == ';') { state = 38; str.push_back(ch); }
        // 处理字符常量
        else if (ch == 39) { state = 39; str.push_back(ch); }
        // 处理字符串常量
        else if (ch == 34) { state = 41; str.push_back(ch); }
    } 
    // 标识符状态，继续识别字母、数字或下划线
    else if (state == 1) {
        if (is_Char(ch) || is_Dig(ch) || ch == '_') { state = 1; str.push_back(ch); }
        else if (ch == '[') {
            state = 43; str.push_back(ch);
        } else {
            state = 2; i--; // 退回一个字符，标识符识别结束
        }
    } 
    // 数字状态，继续识别数字
    else if (state == 3) {
        if (is_Dig(ch)) { state = 3; str.push_back(ch); }
        else if (ch == '.') { state = 4; str.push_back(ch); } // 可能是浮点数
        else {
            state = 7; i--; // 退回一个字符，数字识别结束
        }
    } 
    // 处理浮点数的小数点后部分
    else if (state == 4) {
        if (is_Dig(ch)) { state = 5; str.push_back(ch); }
        else {
            error_flag = 1; // 浮点数格式错误
        }
    } 
    // 继续识别浮点数的小数部分
    else if (state == 5) {
        if (is_Dig(ch)) { state = 5; str.push_back(ch); }
        else {
            state = 6; i--; // 退回一个字符，浮点数识别结束
        }
    } 
    // 处理双字符运算符'=='
    else if (state == 8) {
        if (ch == '=') { state = 9; str.push_back(ch); }
        else {
            state = 10; i--; // 退回一个字符，运算符识别结束
        }
    } 
    // 处理双字符运算符'>='
    else if (state == 11) {
        if (ch == '=') { state = 12; str.push_back(ch); }
        else {
            state = 13; i--; // 退回一个字符，运算符识别结束
        }
    } 
    // 处理双字符运算符'<='
    else if (state == 14) {
        if (ch == '=') { state = 15; str.push_back(ch); }
        else {
            state = 16; i--; // 退回一个字符，运算符识别结束
        }
    } 
    // 处理双字符运算符'++'
    else if (state == 17) {
        if (ch == '+') { state = 18; str.push_back(ch); }
        else if (ch == '=') { state = 19; str.push_back(ch); }
        else {
            state = 20; i--; // 退回一个字符，运算符识别结束
        }
    } 
    // 处理双字符运算符'--'
    else if (state == 21) {
        if (ch == '-') { state = 22; str.push_back(ch); }
        else if (ch == '=') { state = 23; str.push_back(ch); }
        else {
            state = 24; i--; // 退回一个字符，运算符识别结束
        }
    } 
    // 处理运算符'*='
    else if (state == 25) {
        if (ch == '=') { state = 26; str.push_back(ch); }
        else {
            state = 27; i--; // 退回一个字符，运算符识别结束
        }
    } 
    // 处理运算符'/='
    else if (state == 28) {
        if (ch == '=') { state = 29; str.push_back(ch); }
        else {
            state = 30; i--; // 退回一个字符，运算符识别结束
        }
    } 
    // 处理字符常量，状态39表示字符常量起始
    else if (state == 39) {
        if (ch == 39) { state = 40; str.push_back(ch); }
        else {
            state = 39; str.push_back(ch);
        }
    } 
    // 处理字符串常量，状态41表示字符串常量起始
    else if (state == 41) {
        if (ch == 34) { state = 42; str.push_back(ch); }
        else {
            state = 41; str.push_back(ch);
        }
    } 
    // 处理数组下标，状态43表示数组下标起始
    else if (state == 43) {
        if (is_Dig(ch)) { state = 43; str.push_back(ch); }
        else if (ch == ']') { state = 2; str.push_back(ch); }
        else {
            error_flag = 1; // 数组下标格式错误
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

void LexicalAnalyzer::lexical_analysis(std::ifstream& infile, std::ofstream& outfile, std::vector<std::string>& lines, std::string& S) {
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
        outfile << "line " << line++ << ": ";

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
                    outfile << str << " ";
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
                    outfile << str << " ";
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
                    outfile << str << " ";
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
                    outfile << str << " ";
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
                    outfile << str << " ";
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
            outfile << std::endl;
            if (error_flag == 1) {
                outfile << "编译程序出错!!" << std::endl;
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
        outfile << "(" << tokens[j].type << " , " << tokens[j].index << " , " << tokens[j].Vt_id << ")" << std::endl;
    }
}
