#include "complier.h"

void lexical_test() {
    std::ifstream infile("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/lexical_in.txt");
    std::ofstream outfile("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/lexical_out.txt");

    if (!infile || !outfile) {
        std::cerr << "无法打开输入或输出文件" << std::endl;
        return;
    }

    LexicalAnalyzer lexer;
    std::vector<std::string> lines;
    std::string S;

    lexer.lexical_analysis(infile, outfile,lines, S);  // 传入outfile

    infile.close();
    outfile.close();
    std::cout << "lexical_test success!!!" << std::endl;
}

void grammar_test(){

    std::ifstream infile("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/grammar_in.txt");  // 输入文件
    std::ofstream outfile("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/grammar_out.txt");  // 输出文件

    // 检查文件是否成功打开
    if (!infile.is_open() || !outfile.is_open()) {
        std::cerr << "Error opening files!" << std::endl;
    }

    // 重定向标准输入和输出到文件
    std::streambuf* cinbuf = std::cin.rdbuf();
    std::cin.rdbuf(infile.rdbuf());

    std::streambuf* coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(outfile.rdbuf());

    // 创建语法分析对象
    Grammar grammar;

    // 读取并处理数据
    grammar.Get_Vn();
    grammar.Get_Vt();
    grammar.Get_Gram();
    grammar.Get_Vnull();
    grammar.Get_First();
    grammar.Get_Follow();
    grammar.Get_Select();

    // 打印结果
    grammar.Print_Vnull();
    grammar.Print_First();
    grammar.Print_Follow();
    grammar.Print_Select();

    // 恢复标准输入和输出
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);

    std::cout<<"grammmar test Success!!!"<<endl;

    // 关闭文件
    infile.close();
    outfile.close();
}

void lexer_test() {
    std::ifstream infile("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/lexer_in.txt");
    std::ofstream outfile("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/lexer_out.txt");

    if (!infile || !outfile) {
        std::cerr << "无法打开输入或输出文件" << std::endl;
        return;
    }

    std::string input((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
    Lexer lexer(input);
    lexer.getTokens();

    for (const auto& token : lexer.tokens) {
        outfile << "i: " << token.i << ", j: " << token.j << ", Vt_id: " << token.Vt_id << ", type: " << token.type << ", level: " << token.level << "\n";
    }

    infile.close();
    outfile.close();
    std::cout << "lexer_test success!!!" << std::endl;
}

