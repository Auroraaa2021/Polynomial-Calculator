#include<iostream>
#include"polynomial.h"

bool IsAlpha(string s)
{
    for(int i = 0; i < s.length(); ++i)
    {
        if((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'))
            ;
        else
           return false;
    }
    return true;
}

void Input(vector<Poly>& polynomials)
{
    Poly p;
    std::cout << "多项式长度为：";
    std::cin >> p.len;
    while(p.len <= 0)
    {
        std::cout << "非合法多项式长度，请重新输入：";
        std::cin >> p.len;
    }
    std::cout << "请由最高非零系数开始，依次输入各项系数（包括0），空格隔开，结尾回车：" << std::endl;
    for(int i = 0; i < p.len; ++i)
    {
        double d;
        std::cin >> d;
        p.coefficient.push_back(d);
    }
    while(p.len != 1 && p.coefficient[0] == 0)
    {
        std::cout << "最高项系数不能为零！\n请由最高非零系数开始，依次输入各项系数（包括0），空格隔开，结尾回车：" << std::endl;
        p.coefficient.clear();
        for(int i = 0; i < p.len; ++i)
        {
            double d;
            std::cin >> d;
            p.coefficient.push_back(d);
        }
    }
    std::cout << "多项式名为：";
    std::cin >> p.name;
    while(!IsAlpha(p.name))
    {
        std::cout << "非法多项式名！多项式名中仅允许大小写字母！请重新输入：";
        std::cin >> p.name;
    }
    for(unsigned int i = 0; i < polynomials.size(); ++i)
    {
        if(p.name == polynomials[i].name)
        {
            polynomials[i].len = p.len;
            polynomials[i].coefficient.swap(p.coefficient);
            std::cout << "输入成功！" << p.name << " = ";
            PrintPolyHelper(p.coefficient);
            std::cout << '\n';
            return;
        }
    }
    polynomials.push_back(p);
    std::cout << "输入成功！" << p.name << " = ";
    PrintPolyHelper(p.coefficient);
    std::cout << '\n';
    return;
}

void PrintPolyHelper(vector<double> c)
{
    if(c.size() == 1)
    {
        std::cout << c[0] << std::endl;
        return;
    }
    for(unsigned int i = 0; i < c.size(); ++i)
    {
        if(c[i] == 0)
            continue;
        else if(c[i] > 0)
        {
            if(i != 0)
                std::cout << " + ";
        }
        else
        {
            std::cout << " - ";
            c[i] = 0 - c[i];
        }
        if(i == c.size() - 1 || c[i] != 1)
            std::cout << c[i];
        if(c.size() - 1 - i > 0)
            std::cout << 'x';
        if(c.size() - 1 - i > 1)
            std::cout << '^' << (c.size() - 1 - i); 
    }
    std::cout << std::endl;
    return;
}

void PrintPoly(vector<Poly> polynomials)
{
    std::cout << "多项式名为：";
    string s;
    std::cin >> s;
    for(unsigned int i = 0; i < polynomials.size(); ++i)
    {
        if(s == polynomials[i].name)
        {
            std::cout << s << " = ";
            PrintPolyHelper(polynomials[i].coefficient);
            std::cout << std::endl;
            return;
        }
    }
    std::cout << "没有名为 " << s << " 的多项式。\n";
    if(polynomials.empty())
    {
        std::cout << "无多项式，请先输入！\n\n";
        return;
    }
    std::cout << "存在的多项式有：\n";
    for(unsigned int i = 0; i < polynomials.size(); ++i)
    {
        std::cout << polynomials[i].name << " = ";
        PrintPolyHelper(polynomials[i].coefficient);
    }
    std::cout <<'\n';
    return;
}