#pragma once

#include<vector>
using std::vector;
#include<string>
using std::string;

struct Poly
{
    string name;
    int len;
    vector<double> coefficient;
};

// t is poly t.priority = 0
// if t is poly put the poly in p
// t is derive t.priority = 1
// t is integral t.priority = 2
// if t is integral a is inf b is sup
// t is multiply t.priority = 3
// t is plus t.priority = 4
// t is ( t.priority = 5
// t is ) t.priority = 6
struct Token
{
    int priority;
    double a;
    double b;
    Poly p;
};

void Input(vector<Poly>& polynomials);
void PrintPoly(vector<Poly> polynomials);
void PrintPolyHelper(vector<double> c);
bool IsAlpha(string s);