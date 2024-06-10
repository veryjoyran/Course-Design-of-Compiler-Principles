#include "complier.h"

std::ifstream infile_lexical("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/lexical_in.txt");
std::ofstream outfile_lexical("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/lexical_out.txt");

std::ifstream infile_lexer("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/lexer_in.txt");
std::ofstream outfile_lexer("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/lexer_out.txt");

std::ifstream infile_grammar("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/grammar_in.txt");  // 输入文件
std::ofstream outfile_grammar("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/grammar_out.txt");  // 输出文件

std::ifstream infile_parser("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/parser_in.txt");
std::ofstream outfile_parser("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/parser_out.txt");


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
    if (!infile_lexer.is_open() || !outfile_lexer.is_open()) {
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



void parser_test(Parser &parser,Lexer &lexer) {
    

    if (!infile_parser || !outfile_parser) {
        std::cerr << "无法打开输入或输出文件(parser)" << std::endl;
        return;
    }

    std::cout<<"LL(1) start"<<"\n";
    parser.LL1();  // 执行LL1解析

    std::cout<<"LL(1) completed"<<"\n";
    parser.Get_Quats();

    for (const auto& quat : parser.quats) {
        outfile_parser << "(" << lexer.symbolNames[quat.op] << ", "
                << (quat.a == -1 ? "_" : lexer.symbolNames[quat.a]) << ", "
                << (quat.b == -1 ? "_" : lexer.symbolNames[quat.b]) << ", "
                << (quat.res == -1 ? "_" : lexer.symbolNames[quat.res]) << ")\n";
    }

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
    if (grammar.Vt.empty()) {
        std::cout << "grammar.Vt is empty after grammar_test()" << std::endl;
    } else {
        for (auto& i : grammar.Vt) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }
    
    std::string input((std::istreambuf_iterator<char>(infile_lexer)), std::istreambuf_iterator<char>());
    Lexer lexer(input);
    lexer.grammar=grammar;
    lexer_test(lexer);

    SymTable symtbl(lexer);  
    Parser parser(lexer.tokens, grammar, lexer, symtbl);
    parser_test(parser,lexer);

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
