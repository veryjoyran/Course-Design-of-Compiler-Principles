#ifndef _GRAMMAR_H
#define _GRAMMAR_H

#include <vector>
#include <set>
#include <map>
#include <string>
#include <iostream>
#include <algorithm>
#include <utility>
#include <fstream>

class Grammar {
public:
    Grammar();

    int Id(const std::string& str);
    void Get_Set(std::set<int>& set);
    void Get_Vn();
    void Get_Vt();
    void Get_Gram();
    void Get_Vnull();
    bool Check_Vnull(const std::vector<int>& R);
    void Get_First();
    void Get_Follow();
    void Calc_Select(int i);
    void Get_Select();
   

    void Print_Follow() const;
    void Print_First() const;
    void Print_Select() const;
    void Print_Vnull() const;


    std::vector<std::string> names;
    std::set<int> Vt, Vn, Vnull;
    std::vector<std::pair<int, std::vector<int>>> Gram;
    std::map<int, std::set<int>> First, Follow, Select;
    int Start;
};

#endif // _GRAMMAR_H
