#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "optimizequa.h"

void Opt::init() // 初始化四元式结构数组
{
    qua[0].type = 2; qua[0].op = ">"; qua[0].num1.name = "a"; qua[0].num1.type = "1"; qua[0].num2.name = "b"; qua[0].num2.type = "1"; qua[0].ans.name = "t1"; qua[0].ans.type = "2";
    qua[1].type = 3; qua[1].op = "if"; qua[1].num1.name = "t1"; qua[1].num1.type = "2"; qua[1].num2.name = ""; qua[1].num2.type = ""; qua[1].ans.name = ""; qua[1].ans.type = "";
    qua[2].type = 2; qua[2].op = "+"; qua[2].num1.name = "a"; qua[2].num1.type = "1"; qua[2].num2.name = "b"; qua[2].num2.type = "1"; qua[2].ans.name = "t1"; qua[2].ans.type = "2";
    qua[3].type = 2; qua[3].op = "+"; qua[3].num1.name = "a"; qua[3].num1.type = "1"; qua[3].num2.name = "b"; qua[3].num2.type = "1"; qua[3].ans.name = "t2"; qua[3].ans.type = "2";
    qua[4].type = 2; qua[4].op = "*"; qua[4].num1.name = "t1"; qua[4].num1.type = "2"; qua[4].num2.name = "t2"; qua[4].num2.type = "2"; qua[4].ans.name = "t3"; qua[4].ans.type = "2";
    qua[5].type = 0; qua[5].op = "="; qua[5].num1.name = "t3"; qua[5].num1.type = "2"; qua[5].num2.name = ""; qua[5].num2.type = ""; qua[5].ans.name = "x"; qua[5].ans.type = "1";
    qua[6].type = 3; qua[6].op = "el"; qua[6].num1.name = ""; qua[6].num1.type = ""; qua[6].num2.name = ""; qua[6].num2.type = ""; qua[6].ans.name = ""; qua[6].ans.type = "";
    qua[7].type = 2; qua[7].op = "+"; qua[7].num1.name = "5"; qua[7].num1.type = "3"; qua[7].num2.name = "a"; qua[7].num2.type = "1"; qua[7].ans.name = "t1"; qua[7].ans.type = "2";
    qua[8].type = 0; qua[8].op = "="; qua[8].num1.name = "t1"; qua[8].num1.type = "2"; qua[8].num2.name = ""; qua[8].num2.type = ""; qua[8].ans.name = "x"; qua[8].ans.type = "1";
    qua[9].type = 3; qua[9].op = "ie"; qua[9].num1.name = ""; qua[9].num1.type = ""; qua[9].num2.name = ""; qua[9].num2.type = ""; qua[9].ans.name = ""; qua[9].ans.type = "";
}

void Opt::addition(int n, std::string A, std::string typ) // 将变量A添加到节点n上，同时删除变量A的前一个节点的标识
{
    int j, k;
    k = getnode(A);
    if (k != 0) // 如果getnode(A)不为空
    {
        for (j = 0; j < maxn; j++) // 在节点getnode(A)上删除变量A的标识
        {
            if (dag.node[k - 1].sign[j].name == A)
            {
                dag.node[k - 1].sign[j].name = "";
                dag.node[k - 1].sign[j].type = "";
            }
        }
    }
    if (dag.node[n - 1].m_sign.name != "")
    {
        for (j = 0; j < maxn; j++) // 在节点n上添加变量A的标识
        {
            if (dag.node[n - 1].sign[j].name == "")
            {
                dag.node[n - 1].sign[j].name = A;
                dag.node[n - 1].sign[j].type = typ;
                break;
            }
        }
    }
    else
    {
        dag.node[n - 1].m_sign.name = A;
        dag.node[n - 1].m_sign.type = typ;
    }

    if (dag.node[n - 1].m_sign.type == "2") // 如果主标记是临时变量，并且子标记是变量时，进行交换
    {
        for (j = 0; j < maxn; j++)
        {
            if (dag.node[n - 1].sign[j].type == "1")
            {
                std::string a;
                a = dag.node[n - 1].sign[j].name;
                dag.node[n - 1].sign[j].name = dag.node[n - 1].m_sign.name;
                dag.node[n - 1].m_sign.name = a;
                dag.node[n - 1].sign[j].type = "2";
                dag.node[n - 1].m_sign.type = "1";
            }
        }
    }
}

int Opt::getnode(std::string B) // 获取标识为B的节点id
{
    int n = 0; // 节点id
    for (int i = 0; i < dag.num; i++)
    {
        if (dag.node[i].m_sign.name == B) n = i + 1;
        for (int j = 0; j < maxn; j++)
        {
            if (dag.node[i].sign[j].name == B) n = i + 1;
        }
    }
    return n;
}

void Opt::makeleaf(std::string B, std::string typ) // 创建叶子节点
{
    dag.num++;
    dag.node[dag.num - 1].m_sign.name = B;
    dag.node[dag.num - 1].m_sign.type = typ;
    dag.node[dag.num - 1].id = dag.num;
}

void Opt::delnode(int n) // 删除id为n的节点
{
    dag.node[n - 1].m_sign.name = "";
    dag.node[n - 1].id = 0;
    for (int i = n - 1; i < dag.num - 1; i++)
    {
        dag.node[i].id = dag.node[i + 1].id;
        dag.node[i].left = dag.node[i + 1].left;
        dag.node[i].m_sign.name = dag.node[i + 1].m_sign.name;
        dag.node[i].m_sign.type = dag.node[i + 1].m_sign.type;
        dag.node[i].op = dag.node[i + 1].op;
        dag.node[i].right = dag.node[i + 1].right;
        for (int j = 0; j < maxn; j++)
        {
            dag.node[i].sign[j].name = dag.node[i + 1].sign[j].name;
            dag.node[i].sign[j].type = dag.node[i + 1].sign[j].type;
        }
    }
    dag.node[dag.num - 1].id = 0;
    dag.node[dag.num - 1].left = 0;
    dag.node[dag.num - 1].m_sign.name = "";
    dag.node[dag.num - 1].m_sign.type = "";
    dag.node[dag.num - 1].op = "";
    dag.node[dag.num - 1].right = 0;
    for (int j = 0; j < maxn; j++)
    {
        dag.node[dag.num - 1].sign[j].name = "";
        dag.node[dag.num - 1].sign[j].type = "";
    }
    dag.num--;
}

int Opt::findnode(std::string opl, std::string B, std::string C) // 在DAG中查找或创建A=B op C的节点
{
    int n = 0;
    int l, r, k = 0;
    l = getnode(B);
    r = getnode(C);
    for (int i = 0; i < dag.num; i++)
    {
        k = 0;
        if (dag.node[i].op == opl) // op相同
        {
            l = dag.node[i].left;
            for (int j = 0; j < maxn; j++)
            {
                if (dag.node[l - 1].sign[j].name == B) k = 1;
            }
            if ((dag.node[l - 1].m_sign.name == B) || k == 1) // B相同
            {
                k = 0;
                r = dag.node[i].right;
                for (int j = 0; j < maxn; j++)
                {
                    if (dag.node[r - 1].sign[j].name == C) k = 1;
                }
                if ((dag.node[r - 1].m_sign.name == C) || k == 1) // C相同
                {
                    n = i + 1;
                    break;
                }
            }
        }
    }
    return n;
}

int Opt::makenode(std::string opl, std::string B, std::string C) // 创建中间节点
{
    dag.num++;
    dag.node[dag.num - 1].id = dag.num;
    dag.node[dag.num - 1].left = getnode(B);
    dag.node[dag.num - 1].right = getnode(C);
    dag.node[dag.num - 1].op = opl;
    return dag.num;
}

std::string Opt::coutvalue(std::string opl, std::string B, std::string C) // 计算并返回运算结果
{
    char a;
    char s[maxn];
    float v;
    a = opl[0];
    switch (a)
    {
    case '+':
        v = atof(B.c_str()) + atof(C.c_str());
        break;
    case '-':
        v = atof(B.c_str()) - atof(C.c_str());
        break;
    case '*':
        v = atof(B.c_str()) * atof(C.c_str());
        break;
    case '/':
        v = atof(B.c_str()) / atof(C.c_str());
        break;
    }
    sprintf(s, "%f", v);
    std::string p(s);
    return p;
}

void Opt::makequa(int block) // 生成block的优化后四元式
{
    int i, j, k = 0;
    int numB, l, r;
    for (i = 0; i < dag.num; i++)
    {
        k = 0;
        if (dag.node[i].op == "") // 叶子节点
        {
            for (j = 0; j < maxn; j++)
            {
                if (dag.node[i].sign[j].type == "1") { k = 1; numB = j; }
            }
            if (dag.node[i].m_sign.type == "3" ||
                dag.node[i].m_sign.type == "1" && k == 1) // 主标记为常数或变量且有子标记
            {
                std::cout << block << " ( = " << dag.node[i].sign[numB].name << " _ " << dag.node[i].m_sign.name << " )" << std::endl; // ( = B _ A)
                qua[0].ans.name = dag.node[i].m_sign.name; qua[0].ans.type = dag.node[i].m_sign.type;
                qua[0].block = block;
                qua[0].num1.name = dag.node[i].sign[numB].name; qua[0].num1.type = dag.node[i].sign[numB].type;
                qua[0].num2.name = ""; qua[0].num2.type = "";
                qua[0].op = "=";
                qua[0].type = 0;
                Block[block].push_back(qua[0]);
            }

        }
        else
        {
            l = dag.node[i].left;
            r = dag.node[i].right;
            std::cout << block << " ( " << dag.node[i].op << " " << dag.node[l - 1].m_sign.name << " " << dag.node[r - 1].m_sign.name << " " << dag.node[i].m_sign.name << " )" << std::endl;
            qua[0].ans.name = dag.node[i].m_sign.name; qua[0].ans.type = dag.node[i].m_sign.type;
            qua[0].block = block;
            qua[0].num1.name = dag.node[l - 1].m_sign.name; qua[0].num1.type = dag.node[l - 1].m_sign.type;
            qua[0].num2.name = dag.node[r - 1].m_sign.name; qua[0].num2.type = dag.node[r - 1].m_sign.type;
            qua[0].op = dag.node[i].op;
            qua[0].type = 2;

            Block[block].push_back(qua[0]);
        }
    }
}

void Opt::deletedag() // 清空dag图
{
    int i, j;
    for (i = 0; i < dag.num; i++)
    {
        dag.node[i].id = 0;
        dag.node[i].left = 0;
        dag.node[i].m_sign.name = "";
        dag.node[i].m_sign.type = "";
        dag.node[i].op = "";
        dag.node[i].right = 0;
        for (j = 0; j < maxn; j++)
        {
            dag.node[i].sign[j].name = "";
            dag.node[i].sign[j].type = "";
        }
    }
    dag.num = 0;
}

int Opt::divide() // 划分基本块
{
    int i = 0;
    int num = 1;
    while (qua[i].op != "") // 当还有未处理的四元式时循环
    {
        if (qua[i].type != 3 || qua[i].op == "lb" || qua[i].op == "wh")
            qua[i].block = num;
        if (qua[i].op == "if" || qua[i].op == "el" || qua[i].op == "ie" || qua[i].op == "do" || qua[i].op == "we" || qua[i].op == "gt")
        { 
            qua[i].block = num; 
            num++; 
        }
        i++;
    }
    if (qua[i - 1].op == "if" || qua[i - 1].op == "el" || qua[i - 1].op == "ie" || qua[i - 1].op == "do" || qua[i - 1].op == "we" || qua[i - 1].op == "gt")
        num--;
    return num;
}

void Opt::optimizequa(int block_num) // 优化四元式
{
    int newleft, newright; // 新节点id
    int i = 0, j, k;
    int n;
    std::string p; // p=B op C
    std::string A, B, C; // 操作数B和C的运算结果A
    //init(); // 初始化四元式结构数组
    for (k = 1; k <= block_num; k++) // 主循环
    {
        if (qua[i].type == 3)
        {
            std::cout << qua[i].block << " ( " << qua[i].op << " " << qua[i].num1.name << " " << qua[i].num2.name << " " << qua[i].ans.name << " )" << std::endl;
            Block[qua[i].block].push_back(qua[i]);
            i++;
            k = qua[i].block;
        }
        while (qua[i].op != "" && qua[i].block == k) // 当还有未处理的四元式并且属于同一个基本块时循环
        {
            A = qua[i].ans.name;
            B = qua[i].num1.name;
            C = qua[i].num2.name;
            newleft = 0;
            newright = 0;
            if (getnode(B) == 0 && qua[i].type != 3) { makeleaf(B, qua[i].num1.type); newleft = getnode(B); }
            switch (qua[i].type)
            {
            case 0: // 赋值语句
                n = getnode(B);
                addition(n, A, qua[i].ans.type);
                break;
            case 2: // 双目运算
                if (getnode(C) == 0) { makeleaf(C, qua[i].num2.type); newright = getnode(C); }
                j = 0;
                if ((qua[i].op == "*") || (qua[i].op == "/") || (qua[i].op == "+") || (qua[i].op == "-")) j = 1;
                if ((qua[i].num1.type == "3") && (qua[i].num2.type == "3") && j == 1) // 操作数都是常数
                {
                    p = coutvalue(qua[i].op, B, C);
                    if (newleft != 0) delnode(newleft); // 删除新节点
                    if (newright != 0) delnode(newright);
                    if ((n = getnode(p)) == 0) { makeleaf(p, "3"); n = getnode(p); }
                }
                else
                {
                    if ((n = findnode(qua[i].op, B, C)) == 0) // 在DAG中查找或创建A=B op C的节点
                        n = makenode(qua[i].op, B, C);
                }
                addition(n, A, qua[i].ans.type); // 添加子标记
                break;
            case 3:
                break;
            }
            i++;
        }
        makequa(k); // 生成优化后的四元式
        if (qua[i - 1].type == 3)
        {
            std::cout << qua[i - 1].block << " ( " << qua[i - 1].op << " " << qua[i - 1].num1.name << " " << qua[i - 1].num2.name << " " << qua[i - 1].ans.name << " )" << std::endl;
            Block[qua[i - 1].block].push_back(qua[i - 1]);
        }
        deletedag(); // 清空dag图
    }
}

void Opt::run()
{
    int block_num; // 基本块数量
    //init();
    block_num = divide();
    optimizequa(block_num);
    for (int i = 1; i <= block_num; ++i) {
        // std::cout << Block[i].size() << std::endl;
        ToTarget.push_back(Block[i]);
    }
    std::cout << std::endl;
    puts("yes");
    get_objcode();
}
