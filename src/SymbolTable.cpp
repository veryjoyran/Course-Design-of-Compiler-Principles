#include "symbolTable.h"

using namespace std;

// 创建新的记录并根据类型初始化，并添加到符号表和 synbl 中
void SymTable::Get_NewRecord(const string& type, int range, vector<Record>& S, int line, int name) {
    Record rcd;
    rcd.range = range;
    if (type == "int") {
        rcd.type = INT;
    } else if (type == "double") {
        rcd.type = DOUBLE;
    } else if (type == "bool") {
        rcd.type = BOOL;
    } else if (type == "char") {
        rcd.type = CHAR;
    }
    rcd.line = line;
    rcd.name = name;
    S.push_back(rcd);
    synbl.push_back(rcd);
}

// 弹出指定范围的记录
void SymTable::Pop_Record(int range, map<int, vector<Record>>& InfoTable) {
    for (auto& p : InfoTable) {
        vector<Record>& S = p.second;
        if (!S.empty() && S.back().range == range) {
            S.pop_back();
        }
    }
}

// 打印符号表中的记录
void SymTable::Print_Table(const map<int, vector<Record>>& InfoTable) {
    for (const auto& p : InfoTable) {
        printf("%s : \n", lex.symbolNames[p.first].c_str());
        vector<Record> S = p.second;
        reverse(S.begin(), S.end());
        for (const auto& i : S) {
            printf("type = %d, range = %d, line = %d\n", i.type, i.range, i.line);
        }
    }
    cout << endl;
}

// 判断字符串是否为数组定义，并解析数组名称和大小
bool IsArr(const string& str, string& name, int& num) {
    bool ok = false;
    string x;
    for (char ch : str) {
        if (!ok) name.push_back(ch);
        if (ch == '[') {
            ok = true;
        } else if (ch == ']') break;
        else if (ok) {
            x.push_back(ch);
        }
    }
    name.pop_back();
    sscanf(x.c_str(), "%d", &num);
    return ok;
}

// 获取字符串对应的类型枚举值
int get_type(const string& s) {
    if (s == "int") return INT;
    else if (s == "double") return DOUBLE;
    else if (s == "char") return CHAR;
    else if (s == "bool") return BOOL;
    return -1; // 返回 -1 表示未知类型
}

// 获取类型对应的大小（字节数）
int get_size(int i) {
    if (i == CHAR) return 1;
    if (i == DOUBLE) return 8;
    return 4;
}

// 打印标准符号表，包括符号表、类型表和数组信息表
void SymTable::Print_RegularTable() {
    std::cout << "SYNBL\n";
    std::cout << std::setw(8) << "NAME" << std::setw(8) << "TYP" << std::setw(8) << "CAT" << std::setw(8) << "ADDR" << "\n";
    int d = 0; // 地址
    for (auto& r : synbl) { // 确保 r 是非 const 的
        std::cout << std::setw(8) << lex.symbolNames[r.name];
        switch (r.type) {
            case INT: std::cout << std::setw(8) << "int"; break;
            case DOUBLE: std::cout << std::setw(8) << "double"; break;
            case CHAR: std::cout << std::setw(8) << "char"; break;
            case BOOL: std::cout << std::setw(8) << "bool"; break;
            case ARRAY: std::cout << std::setw(8) << "AINFL"; break;
            case FUNCTION: std::cout << std::setw(8) << "PFINFL"; break;
        }
        switch (r.type) {
            case INT:
            case DOUBLE:
            case CHAR:
            case BOOL: std::cout << std::setw(8) << "v"; break;
            case ARRAY: std::cout << std::setw(8) << "t"; break;
            case FUNCTION: std::cout << std::setw(8) << "f"; break;
        }
        if (0 <= r.type && r.type < 4) {
            char addr[10];
            sprintf(addr, "(%d,%d)", r.range, d);
            d += get_size(r.type);
            std::cout << std::setw(8) << addr << "\n";
        } else if (r.type == ARRAY) {
            std::cout << std::setw(8) << "AINFL, 1\n";
            r.low = d;  // 确保这是可修改的
            r.up = d + r.clen;  // 确保这是可修改的
            d += r.clen;
            arr.push_back(r);
        } else if (r.type == FUNCTION) {
            std::cout << std::setw(8) << "PFINFL\n";
        }
    }

    std::cout << "\n";
    // 类型表
    std::cout << "TYPEL\n";
    std::cout << std::setw(8) << "NAME" << std::setw(8) << "TYP" << "\n";
    for (const auto& r : arr) {
        std::cout << std::setw(8) << lex.symbolNames[r.name];
        std::cout << std::setw(8) << "AINFL\n";
    }
    std::cout << "\n";
    // 数组信息表
    std::cout << "AINFL\n";
    std::cout << std::setw(8) << "NAME" << std::setw(8) << "LOW" << std::setw(8) << "UP" << std::setw(8) << "CTP" << std::setw(8) << "CLEN" << "\n";
    for (const auto& r : arr) {
        std::cout << std::setw(8) << lex.symbolNames[r.name];
        std::cout << std::setw(8) << r.low << std::setw(8) << r.up;
        switch (r.ctp) {
            case INT: std::cout << std::setw(8) << "int"; break;
            case DOUBLE: std::cout << std::setw(8) << "double"; break;
            case CHAR: std::cout << std::setw(8) << "char"; break;
            case BOOL: std::cout << std::setw(8) << "bool"; break;
        }
        std::cout << std::setw(8) << r.clen << "\n";
    }
    std::cout << "\n";
    // 函数信息表
    std::cout << "PFINFL\n";
    std::cout << std::setw(8) << "LEVEL" << std::setw(8) << "OFF" << std::setw(8) << "FN" << std::setw(8) << "ENTRY" << std::setw(8) << "PARAM" << "\n";
    std::cout << std::setw(8) << "0" << std::setw(8) << "0" << std::setw(8) << "0" << std::setw(8) << "0" << std::setw(8) << "0" << "\n";
    std::cout << "\n";
}

// 获取符号表信息
void SymTable::Get_SymTable() {
    map<int, vector<Record>> CurTable;
    int range = 0; // 计数级别
    int d = 0;

    for (const auto& line : lines) {
        Infos.push_back(CurTable);
        vector<string> vs;
        stringstream ss(line);
        string str;
        while (ss >> str) vs.push_back(str);
        for (int j = 0; j < vs.size(); ++j, ++d) {
            lex.tokens[d].level = &line - &lines[0];
        }

        if (line == lines.front()) {
            vector<Record>& S = CurTable[lex.getId("main")];
            Record funrec;
            funrec.range = 0;
            funrec.Init_Fun(0, 0, 0, 0);
            funrec.name = lex.getId("main");
            S.push_back(funrec);
            synbl.push_back(funrec);
        } else if (Def.count(vs[0])) { // 定义
            for (size_t j = 1; j < vs.size(); ++j) {
                string name;
                int num;
                if (IsArr(vs[j], name, num)) {
                    vector<Record>& S = CurTable[lex.getId(name)];
                    Record arrrec;
                    arrrec.ctp = get_type(vs[j]);
                    arrrec.line = &line - &lines[0];
                    arrrec.range = range;
                    arrrec.Init_Arr(0, num, get_type(vs[0]), num * get_size(get_type(vs[0])));
                    arrrec.name = lex.getId(name);
                    S.push_back(arrrec);
                    synbl.push_back(arrrec);
                    continue;
                }
                if (vs[j] == "," || vs[j] == ";") continue;
                if (lex.keywords.count(vs[j]) || lex.delimiters.count(vs[j])) continue;
                vector<Record>& S = CurTable[lex.getId(vs[j])];
                if (S.empty()) {
                    Get_NewRecord(vs[0], range, S, &line - &lines[0] + 1, lex.getId(vs[j]));
                } else {
                    Record rcd = S.back();
                    if (rcd.range == range) {
                        printf("Error(%d): redefine! Last define is at line: (%d)!\n", &line - &lines[0] + 1, rcd.line);
                        exit(0);
                    } else {
                        Get_NewRecord(vs[0], range, S, &line - &lines[0] + 1, lex.getId(vs[j]));
                    }
                }
            }
        } else if (vs[0] == "{") { // 进入新范围
            ++range;
        } else if (vs[0] == "}") { // 退出当前范围
            Pop_Record(range, CurTable);
            --range;
        } else { // 变量的使用
            for (const auto& v : vs) {
                if (lex.identifiers.count(v)) {
                    const vector<Record>& vr = CurTable[lex.getId(v)];
                    if (vr.empty()) {
                        printf("Error(%d): Not defined %s!\n", &line - &lines[0] + 1, v.c_str());
                        exit(0);
                    }
                }
            }
        }
    }
}
