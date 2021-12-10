#include"polynomial.h"
#include<iostream>
#include<cmath>
#include<cstdlib>
#include<string>
using std::string;
#include<vector>
using std::vector;
#include<stack>
using std::stack;

Poly Add(Poly p1, Poly p2)
{
    Poly p;
    if(p1.len >= p2.len)
    {
        p.len = p1.len;
        p.coefficient.assign(p1.coefficient.begin(), p1.coefficient.end());
        for(unsigned int i = 0; i < p2.coefficient.size(); ++i)
            p.coefficient[p.coefficient.size() - 1 - i] += p2.coefficient[p2.coefficient.size() - 1 - i];
    }
    else
    {
        p.len = p2.len;
        p.coefficient.assign(p2.coefficient.begin(), p2.coefficient.end());
        for(unsigned int i = 0; i < p1.coefficient.size(); ++i)
            p.coefficient[p.coefficient.size() - 1 - i] += p1.coefficient[p1.coefficient.size() - 1 - i];
    }
    while(p.coefficient[0] == 0)
    {
        p.coefficient.erase(p.coefficient.begin());
        --p.len;
    }
    return p;
}

Poly Multiply(Poly p, double x)
{
    for(unsigned int i = 0; i < p.len; ++i)
        p.coefficient[i] *= x;
    return p;
}

Poly Minus(Poly p1, Poly p2)
{
    return Add(p1, Multiply(p2, -1));
}

Poly Multiply(Poly p1, Poly p2)
{
    if(p1.coefficient[0] == 0)
        return p1;
    else if(p2.coefficient[0] == 0)
        return p2;
    else
    {
        Poly p;
        p.len = p1.len + p2.len - 1;
        for(int i = 0; i < p.len; ++i)
            p.coefficient.push_back(0);
        for(unsigned int i = 0; i < p1.coefficient.size(); ++i)
        {
            for(unsigned int j = 0; j < p2.coefficient.size(); ++j)
                p.coefficient[i + j] += p1.coefficient[i] * p2.coefficient[j];
        }
        return p;
    }
}

double Substitute(Poly p, double x)
{
    double xx = 1;
    double sum = 0;
    for(int i = p.len - 1; i >= 0; --i)
    {
        sum += (p.coefficient[i] * xx);
        xx *= x;
    }
    return sum;
}

Poly Integral(double d1, double d2, Poly p)
{
    for(unsigned int i = 0; i < p.coefficient.size(); ++i)
        p.coefficient[i] /= (p.coefficient.size() - i);
    Poly p1;
    p1.len = 1;
    p1.coefficient.push_back(d2 * Substitute(p, d2) - d1 * Substitute(p, d1));
    return p1;
}

Poly Derive(Poly p)
{
    if(p.len == 1)
        p.coefficient[0] = 0;
    else
    {
        --p.len;
        p.coefficient.pop_back();
        for(unsigned int i = 0; i < p.coefficient.size(); ++i)
            p.coefficient[i] *= (p.coefficient.size() - i);
    }
    return p;
}

bool IsPoly(string name, vector<Poly> polynomials)
{
    for(unsigned int i = 0; i < polynomials.size(); ++i)
    {
        if(polynomials[i].name == name)
            return true;
    }
    return false;
}

Poly FindPoly(string name, vector<Poly> polynomials)
{
    for(unsigned int i = 0; i < polynomials.size(); ++i)
    {
        if(polynomials[i].name == name)
            return polynomials[i];
    }
}

bool CheckIntegralHelper(string s, double& d)
{
    if(s[0] == '.')
        s = "0" + s;
    else if(s[s.length() - 1] == '.')
        s.erase(s.end() - 1);
    if(s.find_first_of(".") != s.find_last_of("."))
        return false;
    for(int i = 0; i < s.length(); ++i)
    {
        if(!(s[i] <= '9' && s[i] >= '0') && s[i] != '.')
            return false;
    }
    d = atof(s.c_str());
    return true;
}

bool CheckIntegral(string s, double& inf, double& sup)
{
    if(s.substr(0, 2) != "$[" || s.substr(s.length() - 1) != "]")
        return false;
    s.erase(0, 2);
    s.erase(s.length() - 1, 1);
    string s1, s2;
    string::size_type pos = s.find_first_of(",");
    if(pos == s.npos)
        return false;
    s1 = s.substr(0, pos);
    s2 = s.substr(pos + 1);
    if(CheckIntegralHelper(s1, inf) && CheckIntegralHelper(s2, sup))
        return true;
    return false;
}

bool Tokenization(string s, vector<Token>& t, vector<Poly> polynomials)
{
    int i = 0;
    while(i < s.length())
    {
        string ss = s.substr(i, 1);
        if(IsAlpha(ss))
        {
            ++i;
            while(i < s.length() && IsAlpha(s.substr(i, 1)))
            {
                ss += s.substr(i, 1);
                ++i;
            }
            if(!IsPoly(ss, polynomials))
            {
                std::cout << "非法多项式" << ss << "! 请重试！" << std::endl << std::endl;
                return false;
            }
            else
            {
                Token tt;
                tt.priority = 0;
                tt.p = FindPoly(ss, polynomials);
                t.push_back(tt);
            }
        }
        else
        {
            if(ss == "(")
            {
                Token tt;
                tt.priority = 5;
                t.push_back(tt);
                ++i;
            }
            else if(ss == ")")
            {
                Token tt;
                tt.priority = 6;
                t.push_back(tt);
                ++i;
            }
            else if(ss == "!")
            {
                Token tt;
                tt.priority = 1;
                t.push_back(tt);
                ++i;
            }
            else if(ss == "$")
            {
                ++i;
                while(i < s.length() && s.substr(i, 1) != "]")
                {
                    ss += s.substr(i, 1);
                    ++i;
                }
                if(i == s.length())
                {
                    std::cout << "非法积分运算符！" << std::endl << std::endl;
                    return false;
                }
                else
                {
                    ss += s.substr(i, 1);
                    ++i;
                }
                Token tt;
                tt.priority = 2;
                if(!CheckIntegral(ss, tt.a, tt.b))
                {
                    std::cout << "非法积分运算符！" << std::endl << std::endl;
                    return false;
                }
                else
                    t.push_back(tt);
            }
            else if(ss == "*")
            {
                Token tt;
                tt.priority = 3;
                t.push_back(tt);
                ++i;
            }
            else if(ss == "+")
            {
                Token tt;
                tt.priority = 4;
                t.push_back(tt);
                ++i;
            }
            else
            {
                std::cout << "非法运算符！" << std::endl << std::endl;
                return false;
            }
        }
    }
    return true;
}

bool LegitFandB(vector<Token> formula)
{
    //first one must be poly $ or (
    if(formula.front().priority != 0 && formula.front().priority != 2 && formula.front().priority != 5)
    {
        std::cout << "算式只允许由多项式名，积分符号，或左括号开始！" << std::endl << std::endl;
        return false;
    }
    //last one must be poly ! or )
    if(formula.back().priority != 0 && formula.back().priority != 1 && formula.back().priority != 6)
    {
        std::cout << "算式只允许由多项式名，求导符号，或右括号结束！" << std::endl << std::endl;
        return false;
    }
    return true;
}

bool LegitMiddle(vector<Token> formula, stack<Token> &StackOperator, stack<Token> &StackOperand)
{
    for(unsigned int i = 0; i < formula.size(); ++i)
    {
        //if the token is poly put it into the stackoperand
        if(formula[i].priority == 0)
        {
            StackOperand.push(formula[i]);
            //after the poly only + * ! and ) is allowed
            if(i < formula.size() - 1)
            {
                if(formula[i + 1].priority != 3 && formula[i + 1].priority != 4 && formula[i + 1].priority != 1 && formula[i + 1].priority != 6)
                {
                    std::cout << "多项式后只允许双目运算符，求导符号或右括号！" << std::endl << std::endl;
                    return false;
                }
            }
        }
        else if(formula[i].priority == 1)
        {
            //if is ! put it into the stackoperator
            StackOperator.push(formula[i]);
            if(i < formula.size() - 1)
            {
                //after ! only + * ) is allowed
                if(formula[i + 1].priority != 3 && formula[i + 1].priority != 4 && formula[i + 1].priority != 6)
                {
                    std::cout << "求导符号后只允许双目运算符或右括号！" << std::endl << std::endl;
                    return false;
                }
            }
        }
        else if(formula[i].priority == 2)
        {
            //if is $ and top of the stackoperator is inferior to $
            if(StackOperator.empty() || StackOperator.top().priority != 1)
                StackOperator.push(formula[i]);
            else //put all the superior ones into the stackoperator and put $ in stackoperand
            {
                while(StackOperator.top().priority == 1)
                {
                    StackOperand.push(StackOperator.top());
                    StackOperator.pop();
                }
                StackOperator.push(formula[i]);
            }
            if(i < formula.size() - 1)
            {
                //after $ only poly and ( is allowed
                if(formula[i + 1].priority != 5 && formula[i + 1].priority != 0)
                {
                    std::cout << "定积分符号后只允许多项式名或左括号！" << std::endl << std::endl;
                    return false;
                }
            }
        }
        else if(formula[i].priority == 3)
        {
            if(StackOperator.empty() || StackOperator.top().priority >= 3)
            {
                StackOperator.push(formula[i]);
            }
            else
            {
                while(!(StackOperator.empty() || StackOperator.top().priority >= 3))
                {
                    StackOperand.push(StackOperator.top());
                    StackOperator.pop();
                }
                StackOperator.push(formula[i]);
            }
            if(i < formula.size() - 1)
            {
                //after * only poly $ and ( is allowed
                if(formula[i + 1].priority != 0 && formula[i + 1].priority != 2 && formula[i + 1].priority != 5)
                {
                    std::cout << "乘法符号后仅允许多项式名，积分符号或左括号！" << std::endl << std::endl;
                    return false;
                }
            }
        }
        else if(formula[i].priority == 4)
        {
            if(StackOperator.empty() || StackOperator.top().priority >= 4)
                    StackOperator.push(formula[i]);
            else
            {
                while(!(StackOperator.empty() || StackOperator.top().priority >= 4))
                {
                    StackOperand.push(StackOperator.top());
                    StackOperator.pop();
                }
                StackOperator.push(formula[i]);
            }
            if(i < formula.size() - 1)
            {
                //after + only poly $ and ( is allowed
                if(formula[i + 1].priority != 0 && formula[i + 1].priority != 2 && formula[i + 1].priority != 5)
                {
                    std::cout << "加法符号后仅允许多项式名，积分符号或左括号！" << std::endl << std::endl;
                    return false;
                }
            }
        }
        else if(formula[i].priority == 5)
        {
            //if is ( put it directly into the stackoperator
            StackOperator.push(formula[i]);
            if(i < formula.size() - 1)
            {
                //after ( only ( $ and poly is allowed
                if(formula[i + 1].priority != 5 && formula[i + 1].priority != 2 && formula[i + 1].priority != 0)
                {
                    std::cout << "左括号后仅允许左括号，积分符号或多项式名！" << std::endl << std::endl;
                    return false;
                }
            }
        }
        else if(formula[i].priority == 6)
        {
            if(i < formula.size() - 1)
            {
                //after ) only ) + * ! is allowed
                if(formula[i + 1].priority != 3 && formula[i + 1].priority != 4 && formula[i + 1].priority != 1 && formula[i + 1].priority != 6)
                {
                    std::cout << "右括号后仅允许右括号，双目运算符或求导符号！" << std::endl << std::endl;
                    return false;
                }
            }
            bool flag = false; //check if the () matching
            while(!StackOperator.empty())
            {
                if(StackOperator.top().priority == 5)
                {
                    flag = true;
                    StackOperator.pop();
                    break;
                }
                else
                {
                    StackOperand.push(StackOperator.top());
                    StackOperator.pop();
                }
            }
            if(!flag)
            {
                std::cout << "括号不匹配！" << std::endl << std::endl;
                return false;
            }
        }
    }
    return true;
}

bool CompoundArithmeticHelper(stack<Poly> &StackPoly, stack<Token> StackOperator)
{
    while(!StackOperator.empty())
    {
        if(StackOperator.top().priority == 0)
            StackPoly.push(StackOperator.top().p);
        else if(StackOperator.top().priority == 1)
        {
            if(StackPoly.empty())
            {
                std::cout << "非法算式！" << std::endl << std::endl;
                return false;
            }
            Poly p1 = StackPoly.top();
            StackPoly.pop();
            p1 = Derive(p1);
            StackPoly.push(p1);
        }
        else if(StackOperator.top().priority == 2)
        {
            if(StackPoly.empty())
            {
                std::cout << "非法算式！" << std::endl << std::endl;
                return false;
            }
            Poly p1 = StackPoly.top();
            StackPoly.pop();
            p1 = Integral(StackOperator.top().a, StackOperator.top().b, p1);
            StackPoly.push(p1);
        }
        else if(StackOperator.top().priority == 3)
        {
            if(StackPoly.size() < 2)
            {
                std::cout << "非法算式！" << std::endl << std::endl;
                return false;
            }
            Poly p1 = StackPoly.top();
            StackPoly.pop();
            Poly p2 = StackPoly.top();
            StackPoly.pop();
            p1 = Multiply(p1, p2);
            StackPoly.push(p1);
        }
        else if(StackOperator.top().priority == 4)
        {
            if(StackPoly.size() < 2)
            {
                std::cout << "非法算式！" << std::endl << std::endl;
                return false;
            }
            Poly p1 = StackPoly.top();
            StackPoly.pop();
            Poly p2 = StackPoly.top();
            StackPoly.pop();
            p1 = Add(p1, p2);
            StackPoly.push(p1);
        }
        StackOperator.pop();
    }
    if(StackPoly.size() != 1)
    {
        std::cout << "非法算式！" << std::endl << std::endl;
        return false;
    }
    return true;
}

void CompoundArithmetic(vector<Poly> polynomials)
{
    std::cout << "请输入不含空格的算式，结尾以回车结束：";
    string s;
    std::cin >> s;
    vector<Token> formula;
    stack<Token> StackOperator;
    stack<Token> StackOperand;
    if(!Tokenization(s, formula, polynomials) || !LegitFandB(formula) || !LegitMiddle(formula, StackOperator, StackOperand))
    {
        std::cout << "是否继续？输入y继续，其他字符退出：";
        string yesorno;
        std::cin >> yesorno;
        if(yesorno == "y")
            CompoundArithmetic(polynomials);
        else
            return;
    }
    
    //get the suffix expression
    while(!StackOperand.empty())
    {
        StackOperator.push(StackOperand.top());
        StackOperand.pop();
    }

    stack<Poly> StackPoly;
    if(!CompoundArithmeticHelper(StackPoly, StackOperator))
    {
        std::cout << "是否继续？输入y继续，其他字符退出：";
        string yesorno;
        std::cin >> yesorno;
        if(yesorno == "y")
            CompoundArithmetic(polynomials);
        else
            return;
    }

    std::cout << s << " = ";
    PrintPolyHelper(StackPoly.top().coefficient);
    std::cout << std::endl;
    std::cout << "是否继续？输入y继续，其他字符退出：";
        string yesorno;
        std::cin >> yesorno;
        if(yesorno == "y")
            CompoundArithmetic(polynomials);
        else
            return;
}

Poly DivisionHelper(Poly d1, Poly d2)
{
    if(d1.len < d2.len)
    {
        Poly p3;
        p3.len = 1;
        p3.coefficient.push_back(0);
        return p3;
    }
    else if(d1.len == d2.len)
    {
        Poly p3;
        p3.len = 1;
        p3.coefficient.push_back(d1.coefficient[0] / d2.coefficient[0]);
        return p3;
    }
    else
    {
        Poly p3;
        p3.len = d1.len - d2.len + 1;
        p3.coefficient.push_back(d1.coefficient[0] / d2.coefficient[0]);
        for(int i = 1; i < p3.len; ++i)
            p3.coefficient.push_back(0);
        d1 = Minus(d1, Multiply(d2, p3));
        return Add(p3, DivisionHelper(d1, d2));
    }
}

Poly InverseHelper(vector<Poly>& m)
{
    if(m[3].len == 1 && m[3].coefficient[0] != 0)
        return DivisionHelper(m[5], m[3]);
    else if(m[0].len == 1 && m[0].coefficient[0] != 0)
        return DivisionHelper(m[2], m[0]);
    else if(m[0].len >= m[3].len)
    {
        Poly p = DivisionHelper(m[0], m[3]);
        m[0] = Minus(m[0], Multiply(p, m[3]));
        m[1] = Minus(m[1], Multiply(p, m[4]));
        m[2] = Minus(m[2], Multiply(p, m[5]));
        return InverseHelper(m);
    }
    else
    {
        Poly p = DivisionHelper(m[3], m[0]);
        m[3] = Minus(m[3], Multiply(p, m[0]));
        m[4] = Minus(m[4], Multiply(p, m[1]));
        m[5] = Minus(m[5], Multiply(p, m[2]));
        return InverseHelper(m);
    }
}

void Inverse(vector<Poly> polynomials)
{
    std::cout << "多项式名为：";
    string s;
    std::cin >> s;
    while(!IsPoly(s, polynomials))
    {
        std::cout << "不存在此多项式。若想要重新输入请输入y, 输入其他字符则退出：";
        std::cin >> s;
        if(s == "y")
        {
            std::cout << "多项式名为：";
            std::cin >> s;
        }
        else
            return;
    }
    Poly p = FindPoly(s, polynomials);
    if(p.coefficient[p.len - 1] == 0)
    {
        std::cout << "多项式 " << p.name << " 没有逆元。" << std::endl << std::endl;
        return;
    }
    Poly p0, p1, p2;
    p0.len = 1;
    p0.coefficient.push_back(0);
    p1.len = 1;
    p1.coefficient.push_back(1);
    p2.len = p.len + 1;
    p2.coefficient.push_back(1);
    for(int i = 1; i < p2.len; ++i)
        p2.coefficient.push_back(0);
    vector<Poly> m;
    m.push_back(p2);
    m.push_back(p1);
    m.push_back(p0);
    m.push_back(p);
    m.push_back(p0);
    m.push_back(p1);
    std::cout << s << "-1 = ";
    PrintPolyHelper(InverseHelper(m).coefficient);
    std::cout << '\n';
    return;
}

void Division(vector<Poly> polynomials)
{
    string s1, s2;
    std::cout << "请输入被除多项式：";
    std::cin >> s1;
    while(!IsPoly(s1, polynomials))
    {
        std::cout << "非法多项式名，请重新输入：";
        std::cin >> s1;
    }
    std::cout << "请输入除多项式：";
    std::cin >> s2;
    while(!IsPoly(s2, polynomials))
    {
        std::cout << "非法多项式名，请重新输入：";
        std::cin >> s1;
    }
    Poly p1 = FindPoly(s1, polynomials);
    Poly p2 = FindPoly(s2, polynomials);
    Poly p3 = DivisionHelper(p1, p2);
    std::cout << s1 << '/' << s2 << " 商为：";
    PrintPolyHelper(p3.coefficient);
    if(!(Multiply(p3, p2).coefficient == p1.coefficient))
    {
        std::cout << "余多项式为：";
        PrintPolyHelper(Minus(p1, Multiply(p2, p3)).coefficient);
    }
    std::cout << std::endl;
    return;
}

void Root(vector<Poly> polynomials)
{
    std::cout << "多项式名为：";
    string s;
    std::cin >> s;
    while(!IsPoly(s, polynomials))
    {
        std::cout << "不存在此多项式。若想要重新输入请输入y, 输入其他字符则退出：";
        std::cin >> s;
        if(s == "y")
            std::cin >> s;
        else
            return;
    }
    Poly p = FindPoly(s, polynomials);
    Poly dp = Derive(p);
    if(p.len == 1)
    {
        std::cout << "此方程无解。" << std::endl << std::endl;
        return;
    }
    double a = 1;
    double b = 0;
    while(fabs(a - b) >= 0.000001)
    {
        a = b;
        b = a - Substitute(p, a) / Substitute(dp, a);
    }
    std::cout << "多项式" << s << "存在的一实根为：" << b << std::endl << std::endl;
    return;
}