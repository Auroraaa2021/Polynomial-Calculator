#include<iostream>
#include "calculator.h"

void Banner();
bool Command();

vector<Poly> polynomials;

int main()
{
    while(Command()){;}
    return 0;
}

void Banner()
{
    for(int i = 0; i < 28; ++i)
        std::cout << '=';
    std::cout << " 多项式计算器 ";
    for(int i = 0; i < 29; ++i)
        std::cout << '=';
    std::cout << std::endl;

    std::cout << "    1.输入 2.混合运算 3.求逆元 4.除法/取模运算 5.求根 6.查看 7.退出" << std::endl;

    for(int i = 0; i < 71; ++i)
        std::cout << '=';
    std::cout << std::endl;
    return;
}

bool Command()
{
    Banner();
    std::cout << "请选择：";
    int i;
    while(true)
    {
        bool loopagain = false;
        std::cin >> i;
        switch (i)
        {
            case 1: Input(polynomials); break;
            case 2: CompoundArithmetic(polynomials); break;
            case 3: Inverse(polynomials); break;
            case 4: Division(polynomials); break;
            case 5: Root(polynomials); break;
            case 6: PrintPoly(polynomials); break;
            case 7: return false;
            default: loopagain = true;
        }
        if(loopagain)
            std::cout << "输入失败，请重试: ";
        else
            break;
        
    }
    return true;
}
