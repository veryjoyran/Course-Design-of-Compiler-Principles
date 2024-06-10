#include "lexer.h"
#include <iostream>
#include <sstream>
#include <cctype>

Lexer::Lexer(const std::string& input) : sourceCode(input) {}

int Lexer::getId(const std::string& str) {
    for (int i = 0; i < symbolNames.size(); ++i) {
        if (symbolNames[i] == str) {
            return i;
        }
    }
    symbolNames.push_back(str);
    return symbolNames.size() - 1;
}

int Lexer::getPos(int category, int id) {
    for (int j = 0; j < symbolTable[category].size(); ++j) {
        if (symbolTable[category][j] == id) {
            return j;
        }
    }
    symbolTable[category].push_back(id);
    return symbolTable[category].size() - 1;
}

bool Lexer::is_Char(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool Lexer::is_Dig(char c) {
    return c >= '0' && c <= '9';
}

void Lexer::next_state(char ch) {
    switch (state) {
        case 0:  // 初始状态
            if (is_Char(ch)) {
                state = 1;
                str.push_back(ch);
            } else if (is_Dig(ch)) {
                state = 3;
                str.push_back(ch);
            } else if (ch == '=') {
                state = 8;
                str.push_back(ch);
            } else if (ch == '>') {
                state = 11;
                str.push_back(ch);
            } else if (ch == '<') {
                state = 14;
                str.push_back(ch);
            } else if (ch == '+') {
                state = 17;
                str.push_back(ch);
            } else if (ch == '-') {
                state = 21;
                str.push_back(ch);
            } else if (ch == '*') {
                state = 25;
                str.push_back(ch);
            } else if (ch == '/') {
                state = 28;
                str.push_back(ch);
            } else if (ch == '{') {
                tokens.push_back(Token(2, getPos(2, getId("{")), grammar.Id("{")));
            } else if (ch == '}') {
                tokens.push_back(Token(2, getPos(2, getId("}")), grammar.Id("}")));
            } else if (ch == '(') {
                tokens.push_back(Token(2, getPos(2, getId("(")), grammar.Id("(")));
            } else if (ch == ')') {
                tokens.push_back(Token(2, getPos(2, getId(")")), grammar.Id(")")));
            } else if (ch == '[') {
                tokens.push_back(Token(2, getPos(2, getId("[")), grammar.Id("[")));
            } else if (ch == ']') {
                tokens.push_back(Token(2, getPos(2, getId("]")), grammar.Id("]")));
            } else if (ch == ',') {
                tokens.push_back(Token(2, getPos(2, getId(",")), grammar.Id(",")));
            } else if (ch == ';') {
                tokens.push_back(Token(2, getPos(2, getId(";")), grammar.Id(";")));
            } else if (ch == '\'') {
                state = 39;
                str.push_back(ch);
            } else if (ch == '"') {
                state = 41;
                str.push_back(ch);
            } else if (ch == '#') {  // 处理 # 字符，跳过当前行
                tokens.push_back(Token(2, getPos(2, getId("#")), grammar.Id("#")));
                
            } else if (isspace(ch)) {
                // 忽略空白字符
            } else {
                std::cerr << "Error: Unrecognized character '" << ch << "' at position " << i << std::endl;
                error_flag = true;
            }
            break;

        case 1:  // 标识符状态
            if (is_Char(ch) || is_Dig(ch) || ch == '_') {
                str.push_back(ch);
            } else if (ch == '[') {  // 检查是否为数组
                state = 43;
                str.push_back(ch);
            } else {
                if (keywords.count(str)) {
                    tokens.push_back(Token(1, getPos(1, getId(str)), grammar.Id(str)));
                } else {
                    tokens.push_back(Token(3, getPos(3, getId(str)), grammar.Id("Iden")));
                }
                str.clear();
                state = 0;
                next_state(ch);  // 重新处理当前字符
            }
            break;

        case 3:  // 数字状态
            if (is_Dig(ch)) {
                str.push_back(ch);
            } else if (ch == '.') {
                state = 4;
                str.push_back(ch);
            } else {
                tokens.push_back(Token(0, getPos(0, getId(str)), grammar.Id("Num")));
                str.clear();
                state = 0;
                next_state(ch);  // 重新处理当前字符
            }
            break;

        case 4:  // 处理浮点数的小数点后部分
            if (is_Dig(ch)) {
                state = 5;
                str.push_back(ch);
            } else {
                std::cerr << "Error: Malformed floating point number at position " << i << std::endl;
                error_flag = true;
            }
            break;

        case 5:  // 继续识别浮点数的小数部分
            if (is_Dig(ch)) {
                str.push_back(ch);
            } else {
                tokens.push_back(Token(0, getPos(0, getId(str)), grammar.Id("Num")));
                str.clear();
                state = 0;
                next_state(ch);  // 重新处理当前字符
            }
            break;

        case 8:  // 处理双字符运算符'=='
            if (ch == '=') {
                str.push_back(ch);
                tokens.push_back(Token(2, getPos(2, getId(str)), grammar.Id(str)));
                str.clear();
                state = 0;
            } else {
                tokens.push_back(Token(2, getPos(2, getId(str)), grammar.Id(str)));
                str.clear();
                state = 0;
                next_state(ch);  // 重新处理当前字符
            }
            break;

        case 11:  // 处理双字符运算符'>='
            if (ch == '=') {
                str.push_back(ch);
                tokens.push_back(Token(2, getPos(2, getId(str)), grammar.Id(str)));
                str.clear();
                state = 0;
            } else {
                tokens.push_back(Token(2, getPos(2, getId(str)), grammar.Id(str)));
                str.clear();
                state = 0;
                next_state(ch);  // 重新处理当前字符
            }
            break;

        case 14:  // 处理双字符运算符'<='
            if (ch == '=') {
                str.push_back(ch);
                tokens.push_back(Token(2, getPos(2, getId(str)), grammar.Id(str)));
                str.clear();
                state = 0;
            } else {
                tokens.push_back(Token(2, getPos(2, getId(str)), grammar.Id(str)));
                str.clear();
                state = 0;
                next_state(ch);  // 重新处理当前字符
            }
            break;

        case 17:  // 处理双字符运算符'++'
            if (ch == '+') {
                str.push_back(ch);
                tokens.push_back(Token(2, getPos(2, getId(str)), grammar.Id(str)));
                str.clear();
                state = 0;
            } else if (ch == '=') {
                str.push_back(ch);
                tokens.push_back(Token(2, getPos(2, getId(str)), grammar.Id(str)));
                str.clear();
                state = 0;
            } else {
                tokens.push_back(Token(2, getPos(2, getId(str)), grammar.Id(str)));
                str.clear();
                state = 0;
                next_state(ch);  // 重新处理当前字符
            }
            break;

        case 21:  // 处理双字符运算符'--'
            if (ch == '-') {
                str.push_back(ch);
                tokens.push_back(Token(2, getPos(2, getId(str)), grammar.Id(str)));
                str.clear();
                state = 0;
            } else if (ch == '=') {
                str.push_back(ch);
                tokens.push_back(Token(2, getPos(2, getId(str)), grammar.Id(str)));
                str.clear();
                state = 0;
            } else {
                tokens.push_back(Token(2, getPos(2, getId(str)), grammar.Id(str)));
                str.clear();
                state = 0;
                next_state(ch);  // 重新处理当前字符
            }
            break;

        case 25:  // 处理运算符'*='
            if (ch == '=') {
                str.push_back(ch);
                tokens.push_back(Token(2, getPos(2, getId(str)), grammar.Id(str)));
                str.clear();
                state = 0;
            } else {
                tokens.push_back(Token(2, getPos(2, getId(str)), grammar.Id(str)));
                str.clear();
                state = 0;
                next_state(ch);  // 重新处理当前字符
            }
            break;

        case 28:  // 处理运算符'/='
            if (ch == '=') {
                str.push_back(ch);
                tokens.push_back(Token(2, getPos(2, getId(str)), grammar.Id(str)));
                str.clear();
                state = 0;
            } else {
                tokens.push_back(Token(2, getPos(2, getId(str)), grammar.Id(str)));
                str.clear();
                state = 0;
                next_state(ch);  // 重新处理当前字符
            }
            break;

        case 39:  // 处理字符常量
            if (ch == '\'') {
                str.push_back(ch);
                tokens.push_back(Token(2, getPos(2, getId(str)), grammar.Id("Charc")));
                str.clear();
                state = 0;
            } else {
                str.push_back(ch);
            }
            break;

        case 41:  // 处理字符串常量
            if (ch == '"') {
                str.push_back(ch);
                tokens.push_back(Token(2, getPos(2, getId(str)), grammar.Id("Strc")));
                str.clear();
                state = 0;
            } else {
                str.push_back(ch);
            }
            break;

        case 43:  // 处理数组标识符
            if (is_Dig(ch)) {
                str.push_back(ch);
            } else if (ch == ']') {
                str.push_back(ch);
                tokens.push_back(Token(1, getPos(1, getId(str)), grammar.Id("Iden")));
                str.clear();
                state = 0;
            } else {
                std::cerr << "Error: Invalid array declaration at position " << i << std::endl;
                error_flag = true;
            }
            break;

        default:
            std::cerr << "Error: Unhandled state " << state << " at position " << i << std::endl;
            error_flag = true;
            break;
    }
}

void Lexer::getTokens() {
    i = 0;
    state = 0;
    str.clear();
    error_flag = false;

    while (i < sourceCode.size()) {
        char ch = sourceCode[i];
        next_state(ch);
        if (error_flag) {
            std::cerr << "词法分析中遇到错误" << std::endl;
            return;
        }
        i++;
    }

    // 处理结束时可能存在的最后一个 token
    if (!str.empty() && !error_flag) {
        if (isdigit(str[0])) {
            tokens.push_back(Token(0, getPos(0, getId(str)), grammar.Id("Num")));
        } else if (keywords.count(str)) {
            tokens.push_back(Token(1, getPos(1, getId(str)), grammar.Id(str)));
        } else if(delimiters.count(str)){
            tokens.push_back(Token(2,getPos(2,getId(str)),grammar.Id(str)));
        }else {
            tokens.push_back(Token(3, getPos(3, getId(str)), grammar.Id("Iden")));
            identifiers.insert(str);
        }
    }
}
