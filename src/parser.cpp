// #include <bits/stdc++.h>
// #include "parser.h"

// void Parser::Get_LL1Table() {
//     for (auto i : gram.Select) {
//         auto p = gram.Gram[i.first];
//         for (auto j : i.second) {
//             LL1Table[{p.first, j}] = i.first;
//         }
//     }
// }

// void Parser::Debug_LL1(std::stack<int> S, int j, int d) {
//     std::vector<int> v;
//     while (!S.empty()) {
//         v.push_back(S.top());
//         S.pop();
//     }
//     std::reverse(v.begin(), v.end());
//     printf("sta: ");
//     for (auto x : v) {
//         printf(" %s", gram.names[x].c_str());
//     }
//     std::cout << std::endl;
//     int r = tokens[d].i, c = tokens[d].j;
//     printf("%s", lex.names[lex.Table[r][c]].c_str());
//     std::cout << std::endl;
// }

// void Parser::LL1() {
//     Get_LL1Table();
//     std::stack<int> S;
//     S.push(gram.Start);
//     int d = 0;
//     while (!S.empty()) {
//         int i = S.top(), j = tokens[d].Vt_id;
//         if (gram.Vt.count(i)) {
//             if (S.top() == j) {
//                 d++;
//                 S.pop();
//             } else {
//                 std::puts("Not Match!");
//                 std::exit(0);
//             }
//         } else if (gram.Vn.count(i)) {
//             S.pop();
//             if (LL1Table.count({i, j})) {
//                 std::vector<int> rev;
//                 for (auto x : gram.Gram[LL1Table[{i, j}]].second) {
//                     if (gram.names[x] == "~") continue;
//                     rev.push_back(x);
//                 }
//                 std::reverse(rev.begin(), rev.end());
//                 for (auto x : rev) S.push(x);
//             } else {
//                 std::puts("Don't have LL1Table!");
//                 std::exit(0);
//             }
//         }
//     }
//     std::puts("exp is right!");
// }

// void Parser::Print_Quat(Quat q) {
//     std::string op, a, b, res;
//     op = lex.names[q.op];
//     a = (q.a == -1) ? "_" : lex.names[q.a];
//     b = (q.b == -1) ? "_" : lex.names[q.b];
//     res = (q.res == -1) ? "_" : lex.names[q.res];
//     printf("(%s, %s, %s, %s)\n", op.c_str(), a.c_str(), b.c_str(), res.c_str());
// }

// int Parser::Get_Type(int id, int tid) {
//     if (tokens[tid].type != -1) return tokens[tid].type;
//     std::vector<Record> vr = symtbl.Infos[tokens[tid].level][id];
//     return tokens[tid].type = vr.back().type;
// }

// void Parser::GEQ(int op, int tokenid) {
//     int tr = SToken.top(); SToken.pop();
//     int tl = SToken.top(); SToken.pop();
//     int rop = SEM.top(); SEM.pop();
//     int lop = SEM.top(); SEM.pop();
//     int typer = Get_Type(lop, tl), typel = Get_Type(rop, tr);
//     if (typer != typel) {
//         printf("Error(%d): Type not match : (%s, %s)!\n", tokens[tl].level + 1, lex.names[lop].c_str(), lex.names[rop].c_str());
//         std::exit(0);
//     }
//     std::string t = "t" + std::to_string(cnt_t++);
//     quats.push_back(Quat(op, lop, rop, lex.Get_Id(t)));
//     SEM.push(lex.Get_Id(t));
//     Token tt = {tokens[tl].type, -1, tokens[tl].Vt_id};
//     tt.level = tokens[tl].level;
//     tokens.push_back(tt);
//     SToken.push(tokens.size() - 1);
//     Print_Quat(quats.back());
// }

// void Parser::PUSH(int x, int tokenid) {
//     SEM.push(x);
//     SToken.push(tokenid);
// }

// void Parser::Get_Action() {
//     int cnt;
//     std::cin >> cnt;
//     for (int i = 0; i < cnt; ++i) {
//         int idx, type, pos;
//         std::cin >> idx >> type >> pos;
//         --idx;
//         acts[idx].type = type;
//         acts[idx].pos = pos;
//     }
// }

// void Parser::Debug_Quats(std::stack<int> S, std::stack<Action> SACT, int j, int d) {
//     std::vector<int> v;
//     while (!S.empty()) {
//         v.push_back(S.top());
//         S.pop();
//     }
//     std::reverse(v.begin(), v.end());
//     printf("S: ");
//     for (auto x : v) {
//         if (x == -1) printf(" -1");
//         else printf(" %s", gram.names[x].c_str());
//     }
//     std::cout << std::endl;
//     int r = tokens[d].i, c = tokens[d].j;
//     printf("%s", lex.names[lex.Table[r][c]].c_str());
//     std::cout << std::endl;
// }

// void Parser::ASSI() {
//     int lop = SEM.top(); SEM.pop();
//     int res = SEM.top(); SEM.pop();
//     quats.push_back(Quat(lex.Get_Id("="), lop, -1, res));
//     Print_Quat(quats.back());
// }

// void Parser::IF() {
//     int lop = SEM.top(); SEM.pop();
//     quats.push_back(Quat(lex.Get_Id("if"), lop, -1, -1));
//     Print_Quat(quats.back());
// }

// void Parser::ELSE() {
//     quats.push_back(Quat(lex.Get_Id("el"), -1, -1, -1));
//     Print_Quat(quats.back());
// }

// void Parser::ENDIF() {
//     quats.push_back(Quat(lex.Get_Id("ie"), -1, -1, -1));
//     Print_Quat(quats.back());
// }

// void Parser::LAB() {
//     int res = SEM.top(); SEM.pop();
//     quats.push_back(Quat(lex.Get_Id("lb"), -1, -1, res));
//     SEM.push(res);
//     Print_Quat(quats.back());
// }

// void Parser::GOTO() {
//     int res = SEM.top(); SEM.pop();
//     quats.push_back(Quat(lex.Get_Id("gt"), -1, -1, res));
//     Print_Quat(quats.back());
// }

// void Parser::WE() {
//     quats.push_back(Quat(lex.Get_Id("we"), -1, -1, -1));
//     Print_Quat(quats.back());
// }

// void Parser::WH() {
//     quats.push_back(Quat(lex.Get_Id("wh"), -1, -1, -1));
//     Print_Quat(quats.back());
// }

// void Parser::DO() {
//     int lop = SEM.top(); SEM.pop();
//     quats.push_back(Quat(lex.Get_Id("do"), lop, -1, -1));
//     Print_Quat(quats.back());
// }

// void Parser::Print_Act(Action a) {
//     printf("obj = %d, pos = %d, type = %d\n", a.object, a.pos, a.type);
// }

// void Parser::Get_Quats() {
//     std::stack<int> S;
//     std::stack<Action> SACT;
//     S.push(gram.Start);
//     int d = 0;
//     while (!S.empty()) {
//         int i = S.top(), j = tokens[d].Vt_id;
//         if (i == -1) {
//             S.pop();
//             if (SACT.empty()) { std::puts("SACT is null!"); std::exit(0); }
//             Action act = SACT.top(); SACT.pop();
//             switch (act.type) {
//                 case 1: GEQ(act.object, act.tokenid); break;
//                 case 2: PUSH(act.object, act.tokenid); break;
//                 case 3: ASSI(); break;
//                 case 4: IF(); break;
//                 case 5: ELSE(); break;
//                 case 6: ENDIF(); break;
//                 case 7: LAB(); break;
//                 case 8: GOTO(); break;
//                 case 9: WE(); break;
//                 case 10: WH(); break;
//                 case 11: DO(); break;
//                 default: break;
//             }
//         } else if (gram.Vt.count(i)) {
//             if (S.top() == j) {
//                 d++;
//                 S.pop();
//             }
//         } else if (gram.Vn.count(i)) {
//             S.pop();
//             if (LL1Table.count({i, j})) {
//                 std::vector<int> rev;
//                 int k = 0;
//                 int t = LL1Table[{i, j}];
//                 for (auto x : gram.Gram[t].second) {
//                     if (gram.names[x] == "~") continue;
//                     rev.push_back(x);
//                     if (acts[t].type == 0) continue;
//                     if (k == acts[t].pos) {
//                         rev.push_back(-1);
//                         int r = tokens[d].i, c = tokens[d].j;
//                         acts[t].object = lex.Table[r][c];
//                         acts[t].tokenid = d;
//                         SACT.push(acts[t]);
//                     }
//                     ++k;
//                 }
//                 std::reverse(rev.begin(), rev.end());
//                 for (auto x : rev) S.push(x);
//             }
//         }
//     }
// }
