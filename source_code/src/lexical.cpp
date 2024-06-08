#include <iostream>
#include <fstream>
#include "lexical.h"

int main() {
    std::ifstream infile("test_code.txt"); 
    if (!infile) {
        std::cerr << "无法打开文件" << std::endl;
        return 1;
    }

    LexicalAnalyzer lexer;
    std::vector<std::string> lines;
    std::string S;

    lexer.lexical_analysis(infile, lines, S);

    infile.close();
    return 0;
}
