#include <bits/stdc++.h>
#include "grammar.h"
#include "lexer.h"
#include "parser.h"
#include "symbolTable.h"
#include "optimizequa.h"
#include "lexical.h"
#include "complier.h"

std::ifstream infile_symTable("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/symbolTable_in.txt");

void test();

int main() {
    test();
    // lexical_test();
    // grammar_test();
    // lexer_test();
    // parser_test();
     //run();


    return 0;
}



void test()
{
    std::ifstream infile("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/in.txt");
    std::ofstream outfile("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/out.txt");

    // 检查文件是否成功打开
    if (!infile.is_open() || !outfile.is_open()) {
        std::cerr << "Error opening files!" << std::endl;
    }

     // 重定向标准输入和输出到文件
    std::streambuf* cinbuf = std::cin.rdbuf();
    std::cin.rdbuf(infile.rdbuf());

     std::streambuf* coutbuf = std::cout.rdbuf();
     std::cout.rdbuf(outfile.rdbuf());

    Complier c;
    c.Init_Gram();
    std::cout<<("Grammar Init Succsess\n");
    std::cout<<"\n\n";

    c.parser.Get_Action();
    std::cout<<"Action_Init Success\n";
    std::cout<<"\n\n";
    string str;
    
    
    // 读取输入文件中的源码样例并赋值给origin
    std::string sourceCode((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
    c.origin = sourceCode;

    // // 将sourceCode的内容按行分割并赋值给c.lines
    // std::istringstream originStream(c.origin);
    // std::string lineContent;
    // while (std::getline(originStream, lineContent)) {
    //     c.lines.push_back(lineContent);
    // }

    // // 逐行打印c.lines的内容
    // std::cout << "Content of c.lines:";
    // for (const auto& line : c.lines) {
    //     std::cout << line << std::endl;
    // }

    // //逐行打印origin的内容
    // std::istringstream originStream_sourceCode(sourceCode);
    // std::string line;
    // while (std::getline(originStream_sourceCode, line)) {
    //     std::cout << line << std::endl;
    // }
    // std::cout<<"\n\n";

    
    //恢复标准输入
    std::cin.rdbuf(cinbuf);
    std::string line;
    while (std::getline(infile_symTable, line)) {
        c.symtbl.lines.push_back(line);
    }
    c.lines=c.symtbl.lines;

    
    c.Lexical_Analysis();
    std::cout<<"Lexer Succsess\n";
    std::cout<<"\n\n";

    c.Grammatical_Analysis();
    std::cout<<"LL(1) Success";
    std::cout<<"\n\n";


    c.Init_SymTable();
    std::cout<<"SymbolTable Success\n";
    std::cout<<"\n\n";

    c.Get_Quats();
    cout<<"Quats Success\n";
    std::cout<<"\n\n";


    c.Init_opt();

    c.opt.run();

    //恢复标准输出
    std::cout.rdbuf(coutbuf);

    std::cout<<"All tasks comoleted!!!"<<endl;
    
    infile.close();
    outfile.close();
}