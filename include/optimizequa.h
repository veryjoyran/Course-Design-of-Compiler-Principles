#ifndef _OPTIMIZEQUA_H
#define _OPTIMIZEQUA_H

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stack>
#include <string>
#include <map>
#include <iomanip>
#include <algorithm>

const int maxn = 1e2 + 5;

class Opt {
public:
    struct Token {
        std::string name;
        std::string type; // 1为变量，2为临时变量，3为常量
    };

    struct Quaternion {
        int block; // 块号
        int type;  // 操作类型 - 0, 1, 2, 3
        std::string op; // 操作
        Token num1; // 操作数1
        Token num2; // 操作数2
        Token ans;  // 结果
    };

    struct ACT_INF {
        std::string num1;
        std::string num2;
        std::string ans;
        ACT_INF(std::string a = "", std::string b = "", std::string c = "") : num1(a), num2(b), ans(c) {}
    };

    struct ACT_SYNBL {
        std::string name;
        std::string act_inf;
        ACT_SYNBL(std::string a = "", std::string b = "") : name(a), act_inf(b) {}
    };

    std::vector<std::vector<ACT_INF>> Act_inf;
    std::vector<std::vector<ACT_SYNBL>> Act_synbl;
    std::vector<std::string> objcode;
    std::stack<std::string> SEM; // 存放临时地址的栈

    Quaternion qua[maxn]; // 四元式数组
    std::vector<Quaternion> Block[15];
    std::vector<std::vector<Quaternion>> ToTarget;

    struct Node {
        int left;
        int right;
        int id;
        std::string op;
        Token m_sign;
        Token sign[maxn];
    };

    struct DAG {
        int num = 0;
        Node node[maxn];
    } dag;

    void init();     // 初始化四元式结构数组
    void addition(int n, std::string A, std::string typ);   // 将变量A添加到节点n上，同时删除变量A的前一个节点的标识
    int getnode(std::string B);   // 获取标识为B的节点id
    void makeleaf(std::string B, std::string typ);   // 创建叶子节点
    void delnode(int n);     // 删除id为n的节点
    int findnode(std::string opl, std::string B, std::string C);   // 在DAG中查找或创建A=B op C的节点
    int makenode(std::string opl, std::string B, std::string C);  // 创建中间节点
    std::string coutvalue(std::string opl, std::string B, std::string C);   // 计算两个常量的值
    void makequa(int block);   // 生成block块中的优化后的四元式
    void deletedag();  // 清空dag图
    void optimizequa(int block_num);    // 优化四元式
    int divide();             // 划分基本块
    void run();

    int to_int(std::string str) {
        int res = 0;
        for (char ch : str) {
            res = res * 10 + (ch - '0');
        }
        return res;
    }

    bool is_inActsynbl(Token token1, std::vector<ACT_SYNBL> act_synbl) {
        for (const auto& act : act_synbl) {
            if (token1.name == act.name) return true;
        }
        return false;
    }

    void get_act_inf(std::vector<std::vector<Quaternion>> qua) {
        for (const auto& q : qua) {
            std::vector<ACT_SYNBL> act_synbl;
            for (const auto& qi : q) {
                if (qi.num1.name != "" && qi.num1.type != "3" && !is_inActsynbl(qi.num1, act_synbl)) {
                    act_synbl.emplace_back(qi.num1.name, qi.num1.type == "1" ? "Y" : "N");
                }
                if (qi.num2.name != "" && qi.num2.type != "3" && !is_inActsynbl(qi.num2, act_synbl)) {
                    act_synbl.emplace_back(qi.num2.name, qi.num2.type == "1" ? "Y" : "N");
                }
                if (qi.ans.name != "" && qi.ans.type != "3" && !is_inActsynbl(qi.ans, act_synbl)) {
                    act_synbl.emplace_back(qi.ans.name, qi.ans.type == "1" ? "Y" : "N");
                }
            }
            Act_synbl.push_back(act_synbl);
        }

        for (const auto& q : qua) {
            std::vector<ACT_INF> act_inf;
            for (int j = q.size() - 1; j >= 0; --j) {
                ACT_INF tmp;
                for (auto& act : Act_synbl[&q - &qua[0]]) {
                    if (q[j].num1.name == act.name) {
                        tmp.num1 = act.act_inf;
                        act.act_inf = "Y";
                    }
                    if (q[j].num2.name == act.name) {
                        tmp.num2 = act.act_inf;
                        act.act_inf = "Y";
                    }
                    if (q[j].ans.name == act.name) {
                        tmp.ans = act.act_inf;
                        act.act_inf = "N";
                    }
                }
                act_inf.push_back(tmp);
            }
            std::reverse(act_inf.begin(), act_inf.end());
            Act_inf.push_back(act_inf);
        }
    }

    void to_objcode(std::vector<std::vector<Quaternion>> qua, std::vector<std::vector<ACT_INF>> Act_inf) {
        for (const auto& q : qua) {
            ACT_SYNBL Rdl("", "");
            for (int j = 0; j < q.size(); ++j) {
                if (q[j].op == "-" || q[j].op == "/" || q[j].op == "!=" || q[j].op == ">=" || q[j].op == "<=" || q[j].op == ">" || q[j].op == "<") {
                    if (Rdl.name == "") {
                        objcode.push_back("LD R," + q[j].num1.name);
                    } else if (Rdl.name != q[j].num1.name) {
                        if (Rdl.act_inf == "Y") {
                            objcode.push_back("ST R," + Rdl.name);
                        }
                        objcode.push_back("LD R," + q[j].num1.name);
                    }
                    Rdl.act_inf = Act_inf[&q - &qua[0]][j].num1;
                    objcode.push_back(q[j].op + " R," + q[j].num2.name);
                    Rdl.name = q[j].ans.name;
                    Rdl.act_inf = Act_inf[&q - &qua[0]][j].ans;
                } else if (q[j].op == "+" || q[j].op == "*") {
                    if (Rdl.name != "" && Rdl.name != q[j].num1.name && Rdl.name != q[j].num2.name) {
                        if (Rdl.act_inf == "Y") {
                            objcode.push_back("ST R," + Rdl.name);
                        }
                    }
                    if (Rdl.name == q[j].num1.name || Rdl.name == q[j].num2.name) {
                        Rdl.act_inf = Act_inf[&q - &qua[0]][j].num1;
                        objcode.push_back((q[j].op == "*" ? "MUL R," : "ADD R,") + (Rdl.name == q[j].num1.name ? q[j].num2.name : q[j].num1.name));
                    } else {
                        objcode.push_back("LD R," + q[j].num1.name);
                        objcode.push_back((q[j].op == "*" ? "MUL R," : "ADD R,") + q[j].num2.name);
                    }
                    Rdl.name = q[j].ans.name;
                    Rdl.act_inf = Act_inf[&q - &qua[0]][j].ans;
                } else if (q[j].op == "if" || q[j].op == "el" || q[j].op == "ie") {
                    if (q[j].op == "if") Rdl.act_inf = Act_inf[&q - &qua[0]][j].num1;
                    if (Rdl.act_inf == "Y") objcode.push_back("ST R," + Rdl.name);
                    objcode.push_back(q[j].op == "if" ? "FJ R," : "JMP ");
                    if (!SEM.empty()) {
                        objcode[to_int(SEM.top())] += std::to_string(objcode.size());
                        SEM.pop();
                    }
                    if (q[j].op != "ie") SEM.push(std::to_string(objcode.size() - 1));
                } else if (q[j].op == "wh" || q[j].op == "do" || q[j].op == "we") {
                    if (q[j].op == "do") Rdl.act_inf = Act_inf[&q - &qua[0]][j].num1;
                    if (Rdl.act_inf == "Y") objcode.push_back("ST R," + Rdl.name);
                    if (q[j].op == "wh") {
                        SEM.push(std::to_string(objcode.size()));
                    } else if (q[j].op == "do") {
                        objcode.push_back("FJ R,");
                    } else if (q[j].op == "we") {
                        objcode.push_back("JMP " + std::to_string(to_int(SEM.top())));
                        SEM.pop();
                    }
                } else if (q[j].op == "lb" || q[j].op == "gt") {
                    if (q[j].op == "lb") {
                        objcode.push_back(q[j].ans.name + ":");
                    } else if (q[j].op == "gt") {
                        objcode.push_back("JMP " + q[j].ans.name);
                    }
                } else if (q[j].op == "=") {
                    if (Rdl.name != q[j].num1.name) {
                        if (Rdl.act_inf == "Y") {
                            objcode.push_back("ST R," + Rdl.name);
                        }
                        objcode.push_back("LD R," + q[j].num1.name);
                    }
                    Rdl.name = q[j].ans.name;
                    Rdl.act_inf = Act_inf[&q - &qua[0]][j].ans;
                }
            }
            if (Rdl.act_inf == "Y") {
                objcode.push_back("ST R," + Rdl.name);
            }
        }

        for (int i = 0; i < objcode.size(); ++i) {
            std::cout << std::left << std::setw(10) << i << std::left << std::setw(10) << objcode[i] << std::endl;
        }
    }

    void get_objcode() {
        get_act_inf(ToTarget);
        to_objcode(ToTarget, Act_inf);
    }
};

#endif // _OPTIMIZEQUA_H
