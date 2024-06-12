#include "complier.h"

std::ifstream infile_lexical("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/lexical_in.txt");
std::ofstream outfile_lexical("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/lexical_out.txt");

std::ifstream infile_lexer("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/lexer_in.txt");
std::ofstream outfile_lexer("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/lexer_out.txt");

std::ifstream infile_grammar("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/grammar_in.txt");  // 输入文件
std::ofstream outfile_grammar("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/grammar_out.txt");  // 输出文件

std::ifstream infile_parser("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/parser_in.txt");
std::ofstream outfile_parser("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/parser_out.txt");

std::ifstream infile_symbolTable("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/symbolTable_in.txt");
std::ofstream outfile_symbolTable("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/symbolTable_out.txt");

void lexical_test(LexicalAnalyzer &lexical) {
    

    if (!infile_lexical || !outfile_lexical) {
        std::cerr << "无法打开输入或输出文件(lexical)" << std::endl;
        return;
    }

   
    std::vector<std::string> lines;
    std::string S;

    lexical.lexical_analysis(infile_lexical, outfile_lexical,lines, S);  // 传入outfile

    infile_lexical.close();
    outfile_lexical.close();
    std::cout << "lexical_test success!!!" << std::endl;
}

void grammar_test(Grammar &grammar){

    // 检查文件是否成功打开
    if (!infile_grammar.is_open() || !outfile_grammar.is_open()) {
        std::cerr << "Error opening files!(grammar)" << std::endl;
    }

    // 重定向标准输入和输出到文件
    std::streambuf* cinbuf = std::cin.rdbuf();
    std::cin.rdbuf(infile_grammar.rdbuf());

    std::streambuf* coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(outfile_grammar.rdbuf());


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
    infile_grammar.close();
    outfile_grammar.close();
}

void lexer_test(Lexer &lexer) {
   

    if (!infile_lexer) {
        std::cerr << "无法打开输入文件(lexer)" << std::endl;
        return;
    }

    if (!outfile_lexer) {
        std::cerr << "无法打开输出文件(lexer)" << std::endl;
        return;
    }

  
    lexer.getTokens();

    for (const auto& token : lexer.tokens) {
        outfile_lexer << "i: " << token.i << ", j: " << token.j << ", Vt_id: " << token.Vt_id << ", type: " << token.type << ", level: " << token.level << "\n";
    }

    infile_lexer.close();
    outfile_lexer.close();
    std::cout << "lexer_test success!!!" << std::endl;
}


void symbolTable_test(SymTable &symTable, Lexer &lexer)
{
    // 检查文件是否成功打开
    if (!infile_symbolTable.is_open() || !outfile_symbolTable.is_open()) {
        std::cerr << "Error opening files!(symbolTable)" << std::endl;
        return;  // 终止函数执行
    }

    
    // 重定向标准输出到文件
    std::streambuf* coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(outfile_symbolTable.rdbuf());

    std::string line;
    while (std::getline(infile_symbolTable, line)) {
        symTable.lines.push_back(line);
    }

    symTable.lex = lexer;
    symTable.Get_SymTable();
    symTable.Print_RegularTable();
    lexer.tokens = symTable.lex.tokens;

    // 恢复标准输出
    std::cout.flush();  // 确保所有缓冲的输出都写入文件
    std::cout.rdbuf(coutbuf);  // 恢复原来的缓冲

    // 输出成功信息
    std::cout << "symbolTable_test success!!!" << std::endl;

    infile_symbolTable.close();
    outfile_symbolTable.close();
}


void parser_test(Parser &parser,Lexer &lexer,Grammar& grammar,SymTable &symTable) {
    

    if (!infile_parser || !outfile_parser) {
        std::cerr << "无法打开输入或输出文件(parser)" << std::endl;
        return;
    }

    // 重定向标准输出到文件
    std::streambuf* coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(outfile_parser.rdbuf());

    // 重定向标准输入到文件
    std::streambuf* cinbuf = std::cin.rdbuf();
    std::cin.rdbuf(infile_parser.rdbuf());

    parser.Get_Action();

    parser.tokens=lexer.tokens;
    parser.gram=grammar;
    parser.lex=lexer;

    std::cout<<"LL(1) start"<<"\n";
    parser.LL1();  // 执行LL1解析

    std::cout<<"LL(1) completed"<<"\n";

    parser.symtbl=symTable;
    parser.tokens=symTable.lex.tokens;
    
    // std::cout<<"四元式开始生成"<<"\n";
    parser.Get_Quats();
    // std::cout<<"四元式生成结束"<<"\n";
    lexer=parser.lex;
    

//  for (const Quat& quat : parser.quats) {
//         parser.Print_Quat(quat);
//     }

    // 恢复标准输入输出
    std::cout.flush();  // 确保所有缓冲的输出都写入文件
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);  

    infile_parser.close();
    outfile_parser.close();



    std::cout << "parser_test success!!!" << std::endl;
}

void run()
{

    LexicalAnalyzer lexicalanalyzer;
    lexical_test(lexicalanalyzer);

    Grammar grammar;
    grammar_test(grammar);
    // if (grammar.Vt.empty()) {
    //     std::cout << "grammar.Vt is empty after grammar_test()" << std::endl;
    // } else {
    //     for (auto& i : grammar.Vt) {
    //         std::cout << i << " ";
    //     }
    //     std::cout << std::endl;
    // }
    
    std::string input((std::istreambuf_iterator<char>(infile_lexer)), std::istreambuf_iterator<char>());
    Lexer lexer(input);
    lexer.grammar=grammar;
    lexer_test(lexer);

    SymTable symtbl(lexer);  
    symbolTable_test(symtbl,lexer);
   
  
    Parser parser(lexer.tokens, grammar, lexer, symtbl);
    parser_test(parser,lexer,grammar,symtbl);
    // if (parser.quats.empty()) {
    //     std::cout << "parser.quats is empty after parser_test()" << std::endl;
    // } else {
    //      for (const Quat& quat : parser.quats) {
    //     parser.Print_Quat(quat);
    // }
    //     std::cout << std::endl;
    // }

    //   for (const auto& token : parser.tokens) {
    //     cout << "i: " << token.i << ", j: " << token.j << ", Vt_id: " << token.Vt_id << ", type: " << token.type << ", level: " << token.level << "\n";
    // }

    

}





// void Complier::Init_Gram() {
//     grammar.Get_Vn();
// 	grammar.Get_Vt();
//     grammar.Get_Gram();
// 	grammar.Get_Vnull();
//     grammar.Get_First();
//     grammar.Get_Follow();
//     grammar.Get_Select();

// //    gram.Print_Vnull();
// //    gram.Print_First();
// //    gram.Print_Follow();
// //    gram.Print_Select();
// }

// void Complier::Lexical_Analysis() {
//     lexer.sourceCode = origin;
//     lexer.grammar = grammar;

//     lexer.getTokens();

//     tokens = lexer.tokens;
// //    puts("token:");
// //    for(auto i : tokens) {
// //        printf("i = %d, j = %d, id = %d\n",
// //               i.i, i.j, i.Vt_id);
// //        cout << gram.names[i.Vt_id] << endl;
// //        cout << lex.names[lex.Table[i.i][i.j]] << endl;
// //    }puts("");
// }

// void Complier::Grammatical_Analysis() {
//     parser.tokens = tokens;
//     parser.gram = grammar;
//     parser.lex = lexer;

//     parser.LL1();
// }

// void Complier::Init_SymTable() {
//     symtbl.lines = lines;
//     symtbl.lex = lexer;
//     symtbl.Get_SymTable();
//     tokens = symtbl.lex.tokens;
// }

// void Complier::Get_Quats() {
//     parser.symtbl = symtbl;
//     parser.tokens = tokens;
//     parser.Get_Quats();

//     lexer = parser.lex;
//     quats = parser.quats;
//     tokens = parser.tokens;
// }
