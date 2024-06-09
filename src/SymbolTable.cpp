#include "symbolTable.h"

using namespace std;

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

void SymTable::Pop_Record(int range, map<int, vector<Record>>& InfoTable) {
    for (auto& p : InfoTable) {
        vector<Record>& S = p.second;
        if (!S.empty() && S.back().range == range) {
            S.pop_back();
        }
    }
}

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

int get_type(const string& s) {
    if (s == "int") return INT;
    else if (s == "double") return DOUBLE;
    else if (s == "char") return CHAR;
    else if (s == "bool") return BOOL;
    return -1; // Return -1 for unknown types
}

int get_size(int i) {
    if (i == CHAR) return 1;
    if (i == DOUBLE) return 8;
    return 4;
}

void SymTable::Print_RegularTable() {
    puts("SYNBL");
    printf("%-8s%-8s%-8s%-8s\n", "NAME", "TYP", "CAT", "ADDR");
    int d = 0; // the address
    for (auto& r : synbl) { // Ensure r is not const
        printf("%-8s", lex.symbolNames[r.name].c_str());
        switch (r.type) {
            case INT: printf("%-8s", "int"); break;
            case DOUBLE: printf("%-8s", "double"); break;
            case CHAR: printf("%-8s", "char"); break;
            case BOOL: printf("%-8s", "bool"); break;
            case ARRAY: printf("%-8s", "AINFL"); break;
            case FUNCTION: printf("%-8s", "PFINFL"); break;
        }
        switch (r.type) {
            case INT:
            case DOUBLE:
            case CHAR:
            case BOOL: printf("%-8s", "v"); break;
            case ARRAY: printf("%-8s", "t"); break;
            case FUNCTION: printf("%-8s", "f"); break;
        }
        if (0 <= r.type && r.type < 4) {
            char addr[10];
            sprintf(addr, "(%d,%d)", r.range, d);
            d += get_size(r.type);
            printf("%-8s\n", addr);
        } else if (r.type == ARRAY) {
            printf("%-8s\n", "AINFL, 1");
            r.low = d;  // Ensure this is modifiable
            r.up = d + r.clen;  // Ensure this is modifiable
            d += r.clen;
            arr.push_back(r);
        } else if (r.type == FUNCTION) {
            printf("%-8s\n", "PFINFL");
        }
    }

    cout << endl;
    // Type Table
    puts("TYPEL");
    printf("%-8s%-8s\n", "NAME", "TYP");
    for (const auto& r : arr) {
        printf("%-8s", lex.symbolNames[r.name].c_str());
        printf("%-8s\n", "AINFL");
    }
    cout << endl;
    puts("AINFL");
    printf("%-8s%-8s%-8s%-8s%-8s\n", "NAME", "LOW", "UP", "CTP", "CLEN");
    for (const auto& r : arr) {
        printf("%-8s", lex.symbolNames[r.name].c_str());
        printf("%-8d%-8d", r.low, r.up);
        switch (r.ctp) {
            case INT: printf("%-8s", "int"); break;
            case DOUBLE: printf("%-8s", "double"); break;
            case CHAR: printf("%-8s", "char"); break;
            case BOOL: printf("%-8s", "bool"); break;
        }
        printf("%-8d\n", r.clen);
    }
    cout << endl;
    puts("PFINFL");
    printf("%-8s%-8s%-8s%-8s%-8s\n", "LEVEL", "OFF", "FN", "ENTRY", "PARAM");
    printf("%-8s%-8s%-8s%-8s%-8s\n", "0", "0", "0", "0", "0");
    cout << endl;
}


void SymTable::Get_SymTable() {
    map<int, vector<Record>> CurTable;
    int range = 0; // count level;
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
        } else if (Def.count(vs[0])) { // define
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
        } else if (vs[0] == "{") { // get into new range
            ++range;
        } else if (vs[0] == "}") { // get out of a range
            Pop_Record(range, CurTable);
            --range;
        } else { // use of a var
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
