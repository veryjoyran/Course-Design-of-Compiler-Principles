#include <bits/stdc++.h>
#include "parser.h"

// 生成 LL1 分析表
void Parser::Get_LL1Table() {
    for (auto i : gram.Select) {
        auto p = gram.Gram[i.first];
        for (auto j : i.second) {
            LL1Table[{p.first, j}] = i.first;
        }
    }
}

// 调试 LL1 解析过程
void Parser::Debug_LL1(std::stack<int> S, int j, int d) {
    std::vector<int> v;
    while (!S.empty()) {
        v.push_back(S.top());
        S.pop();
    }
    std::reverse(v.begin(), v.end());
    printf("sta: ");
    for (auto x : v) {
        printf(" %s", gram.names[x].c_str());
    }
    std::cout << std::endl;
    int r = tokens[d].i, c = tokens[d].j;
    printf("%s", lex.symbolNames[lex.symbolTable[r][c]].c_str());
    std::cout << std::endl;
}

// LL1 解析
void Parser::LL1() {
    Get_LL1Table();  // 生成 LL1 分析表
    // Print_LL1Table();
    std::stack<int> S;
    S.push(gram.Start);  // 将起始符号压入栈
    int d = 0;
    while (!S.empty()) {
        int i = S.top();
        int j = tokens[d].Vt_id;
        if (gram.Vt.count(i)) {  // 如果是终结符
            if (S.top() == j) {
                d++;
                S.pop();
            } else {
                std::cout<<"Not Match!\n";
                std::exit(0);
            }
        } else if (gram.Vn.count(i)) {  // 如果是非终结符
            S.pop();
            if (LL1Table.count({i, j})) {
                std::vector<int> rev;
                for (auto x : gram.Gram[LL1Table[{i, j}]].second) {
                    if (gram.names[x] == "~") continue;
                    rev.push_back(x);
                }
                std::reverse(rev.begin(), rev.end());
                for (auto x : rev) S.push(x);
            } else {
                std::cout<<"Don't have LL1Table!\n";
                std::exit(0);
            }
        }
    }
    std::cout<<"exp is right!\n";
}

// 打印四元式
void Parser::Print_Quat(Quat q) {
    std::string op = lex.symbolNames[q.op];
    std::string a = (q.a == -1) ? "_" : lex.symbolNames[q.a];
    std::string b = (q.b == -1) ? "_" : lex.symbolNames[q.b];
    std::string res = (q.res == -1) ? "_" : lex.symbolNames[q.res];

    std::cout << "(" << op << ", " << a << ", " << b << ", " << res << ")\n";
}

// 获取类型
int Parser::Get_Type(int id, int tid) {
    // 如果 token 已经有类型，则直接返回
    if (tokens[tid].type != -1) return tokens[tid].type;
    
    // 获取符号表中的记录列表
    const vector<Record>& vr = symtbl.Infos[tokens[tid].level][id];
    
    // 检查记录列表是否为空
    // if (vr.empty()) {
    //     cerr << "Error: No records found for id " << id << " at level " << tokens[tid].level << endl;
    //     exit(1);  
    // }
    
    // 返回最后一个记录的类型，并更新 tokens 数组中的类型
    // tokens[tid].type = vr.back().type;
    return 0;
}

// GEQ 操作
void Parser::GEQ(int op, int tokenid) {
    int tr = SToken.top(); SToken.pop();
    int tl = SToken.top(); SToken.pop();
    int rop = SEM.top(); SEM.pop();
    int lop = SEM.top(); SEM.pop();
    int typer = Get_Type(lop, tl), typel = Get_Type(rop, tr);
    if (typer != typel) {
        printf("Error(%d): Type not match : (%s, %s)!\n", tokens[tl].level + 1, lex.symbolNames[lop].c_str(), lex.symbolNames[rop].c_str());
        std::exit(0);
    }
    std::string t = "t" + std::to_string(cnt_t++);
    quats.push_back(Quat(op, lop, rop, lex.getId(t)));
    SEM.push(lex.getId(t));
    Token tt = {tokens[tl].type, -1, tokens[tl].Vt_id};
    tt.level = tokens[tl].level;
    tokens.push_back(tt);
    SToken.push(tokens.size() - 1);
    Print_Quat(quats.back());
}

// PUSH 操作
void Parser::PUSH(int x, int tokenid) {
    SEM.push(x);
    SToken.push(tokenid);
}

// 获取操作
void Parser::Get_Action() {
    int cnt;
    std::cin >> cnt;
    for (int i = 0; i < cnt; ++i) {
        int idx, type, pos;
        std::cin >> idx >> type >> pos;
        --idx;
        acts[idx].type = type;
        acts[idx].pos = pos;
    }
}

// 调试四元式生成过程
void Parser::Debug_Quats(std::stack<int> S, std::stack<Action> SACT, int j, int d) {
    std::vector<int> v;
    while (!S.empty()) {
        v.push_back(S.top());
        S.pop();
    }
    std::reverse(v.begin(), v.end());
    printf("S: ");
    for (auto x : v) {
        if (x == -1) printf(" -1");
        else printf(" %s", gram.names[x].c_str());
    }
    std::cout << std::endl;
    int r = tokens[d].i, c = tokens[d].j;
    printf("%s", lex.symbolNames[lex.symbolTable[r][c]].c_str());
    std::cout << std::endl;
}

// ASSI 操作
void Parser::ASSI() {
    int lop = SEM.top(); SEM.pop();
    int res = SEM.top(); SEM.pop();
    quats.push_back(Quat(lex.getId("="), lop, -1, res));
    Print_Quat(quats.back());
}

// IF 操作
void Parser::IF() {
    int lop = SEM.top(); SEM.pop();
    quats.push_back(Quat(lex.getId("if"), lop, -1, -1));
    Print_Quat(quats.back());
}

// ELSE 操作
void Parser::ELSE() {
    quats.push_back(Quat(lex.getId("el"), -1, -1, -1));
    Print_Quat(quats.back());
}

// ENDIF 操作
void Parser::ENDIF() {
    quats.push_back(Quat(lex.getId("ie"), -1, -1, -1));
    Print_Quat(quats.back());
}

// LAB 操作
void Parser::LAB() {
    int res = SEM.top(); SEM.pop();
    quats.push_back(Quat(lex.getId("lb"), -1, -1, res));
    SEM.push(res);
    Print_Quat(quats.back());
}

// GOTO 操作
void Parser::GOTO() {
    int res = SEM.top(); SEM.pop();
    quats.push_back(Quat(lex.getId("gt"), -1, -1, res));
    Print_Quat(quats.back());
}

// WHILE 结束操作
void Parser::WE() {
    quats.push_back(Quat(lex.getId("we"), -1, -1, -1));
    Print_Quat(quats.back());
}

// WHILE 开始操作
void Parser::WH() {
    quats.push_back(Quat(lex.getId("wh"), -1, -1, -1));
    Print_Quat(quats.back());
}

// DO 操作
void Parser::DO() {
    int lop = SEM.top(); SEM.pop();
    quats.push_back(Quat(lex.getId("do"), lop, -1, -1));
    Print_Quat(quats.back());
}

// 打印操作
void Parser::Print_Act(Action a) {
    printf("obj = %d, pos = %d, type = %d\n", a.object, a.pos, a.type);
}

// 获取四元式
void Parser::Get_Quats() {
    std::stack<int> S;
    std::stack<Action> SACT;
    S.push(gram.Start);  // 将起始符号压入栈
    int d = 0;
    while (!S.empty()) {
        int i = S.top(), j = tokens[d].Vt_id;
        if (i == -1) {
            S.pop();
            if (SACT.empty()) { 
                std::puts("SACT is null!"); 
                std::exit(0); 
            }
            Action act = SACT.top(); SACT.pop();
            switch (act.type) {
                case 1: GEQ(act.object, act.tokenid); break;
                case 2: PUSH(act.object, act.tokenid); break;
                case 3: ASSI(); break;
                case 4: IF(); break;
                case 5: ELSE(); break;
                case 6: ENDIF(); break;
                case 7: LAB(); break;
                case 8: GOTO(); break;
                case 9: WE(); break;
                case 10: WH(); break;
                case 11: DO(); break;
                default: break;
            }
        } else if (gram.Vt.count(i)) {  // 如果是终结符
            if (S.top() == j) {
                d++;
                S.pop();
            }
        } else if (gram.Vn.count(i)) {  // 如果是非终结符
            S.pop();
            if (LL1Table.count({i, j})) {
                std::vector<int> rev;
                int k = 0;
                int t = LL1Table[{i, j}];
                for (auto x : gram.Gram[t].second) {
                    if (gram.names[x] == "~") continue;
                    rev.push_back(x);
                    if (acts[t].type == 0) continue;
                    if (k == acts[t].pos) {
                        rev.push_back(-1);
                        int r = tokens[d].i, c = tokens[d].j;
                        acts[t].object = lex.symbolTable[r][c];
                        acts[t].tokenid = d;
                        SACT.push(acts[t]);
                    }
                    ++k;
                }
                std::reverse(rev.begin(), rev.end());
                for (auto x : rev) S.push(x);
            }
        }
    }
}

void Parser::Print_LL1Table() {
    for (const auto& entry : LL1Table) {
        std::pair<int, int> key = entry.first;
        int value = entry.second;
        std::cout << "LL1Table[(" << key.first << ", " << key.second << ")] = " << value << std::endl;
    }
}
