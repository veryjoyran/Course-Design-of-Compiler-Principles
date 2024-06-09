#include "grammar.h"

// 构造函数初始化起始符号为 -1
Grammar::Grammar() : Start(-1) {}

// 打印 Vnull 集合
void Grammar::Print_Vnull() const {
    std::cout << "Vnull:\n";
    for (auto i : Vnull) {
        std::cout << names[i] << std::endl;
    }
    std::cout << std::endl;
}

// 打印 First 集合
void Grammar::Print_First() const {
    std::cout << "First:\n";
    for (const auto& i : First) {
        std::cout << names[i.first] << " :";
        for (auto j : i.second) {
            std::cout << " " << names[j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// 打印 Follow 集合
void Grammar::Print_Follow() const {
    std::cout << "Follow:\n";
    for (const auto& i : Follow) {
        std::cout << names[i.first] << " :";
        for (auto j : i.second) {
            std::cout << " " << names[j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// 打印 Select 集合
void Grammar::Print_Select() const {
    std::cout << "Select:\n";
    for (const auto& i : Select) {
        const auto& p = Gram[i.first];
        std::cout << names[p.first] << " :";
        for (auto x : p.second) {
            std::cout << " " << names[x];
        }
        std::cout << " [";
        for (auto j : i.second) {
            std::cout << " " << names[j];
        }
        std::cout << " ]" << std::endl;
    }
    std::cout << std::endl;
}

// 返回字符串 str 对应的 id，如果不存在则新增
int Grammar::Id(const std::string& str) {
    for (int i = 0; i < names.size(); ++i) {
        if (names[i] == str) {
            return i;
        }
    }
    names.push_back(str);
    return names.size() - 1;
}

// 从输入中读取集合
void Grammar::Get_Set(std::set<int>& set) {
    int cnt;
    std::cin >> cnt;
    for (int i = 0; i < cnt; ++i) {
        std::string str;
        std::cin >> str;
        set.insert(Id(str));
        if (Start == -1) Start = Id(str);
    }
}

// 获取非终结符集合
void Grammar::Get_Vn() {
    Get_Set(Vn);
}

// 获取终结符集合
void Grammar::Get_Vt() {
    Get_Set(Vt);
}

// 获取语法规则
void Grammar::Get_Gram() {
    int cnt;
    std::cin >> cnt;
    for (int i = 0; i < cnt; ++i) {
        std::string str;
        int L, cnt2;
        std::cin >> str >> cnt2;
        L = Id(str);
        std::vector<int> R;
        for (int j = 0; j < cnt2; ++j) {
            std::cin >> str;
            R.push_back(Id(str));
        }
        Gram.push_back({L, R});
    }
}

// 检查右部是否能推出空
bool Grammar::Check_Vnull(const std::vector<int>& R) {
    for (auto i : R) {
        if (!Vnull.count(i)) return false;
    }
    return true;
}

// 计算 Vnull 集合
void Grammar::Get_Vnull() {
    bool changing = true;
    while (changing) {
        changing = false;
        for (const auto& p : Gram) {
            const auto& R = p.second;
            if ((names[R[0]] == "~" || Check_Vnull(R)) && !Vnull.count(p.first)) {
                Vnull.insert(p.first);
                changing = true;
            }
        }
    }
}

// 计算 First 集合
void Grammar::Get_First() {
    bool changing = true;
    while (changing) {
        changing = false;
        for (const auto& p : Gram) {
            const auto& R = p.second;
            for (auto i : R) {
                if (Vt.count(i)) {
                    if (!First[p.first].count(i)) {
                        First[p.first].insert(i);
                        changing = true;
                    }
                    break;
                } else if (Vn.count(i)) {
                    for (auto vt : First[i]) {
                        if (!First[p.first].count(vt)) {
                            First[p.first].insert(vt);
                            changing = true;
                        }
                    }
                    if (!Vnull.count(i)) break;
                }
            }
        }
    }
}

// 计算 Follow 集合
void Grammar::Get_Follow() {
    bool changing = true;
    while (changing) {
        changing = false;
        for (const auto& p : Gram) {
            std::set<int> temp_f = Follow[p.first];
            auto R = p.second;
            std::reverse(R.begin(), R.end());
            for (auto i : R) {
                if (Vt.count(i)) {
                    temp_f.clear();
                    temp_f.insert(i);
                } else if (Vn.count(i)) {
                    for (auto vt : temp_f) {
                        if (!Follow[i].count(vt)) {
                            Follow[i].insert(vt);
                            changing = true;
                        }
                    }
                    if (!Vnull.count(i)) temp_f = First[i];
                    else {
                        for (auto vt : First[i]) {
                            temp_f.insert(vt);
                        }
                    }
                }
            }
        }
    }
}

// 计算 Select 集合
void Grammar::Calc_Select(int i) {
    const auto& p = Gram[i];
    const auto& R = p.second;
    for (auto j : R) {
        if (Vt.count(j)) {
            Select[i].insert(j);
            return;
        } else if (Vn.count(j)) {
            for (auto f : First[j]) {
                Select[i].insert(f);
            }
            if (!Vnull.count(j)) return;
        }
    }
    for (auto f : Follow[p.first]) {
        Select[i].insert(f);
    }
}

// 获取 Select 集合
void Grammar::Get_Select() {
    for (int i = 0; i < Gram.size(); ++i) {
        Calc_Select(i);
    }
}

