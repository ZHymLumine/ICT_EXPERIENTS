// 張　一鳴、21B12111、ICT.E218
// プログラミング言語：C++
// コンパイル方法：g++ 21B12111-02-04.cpp -o 21B12111-02-04
// 実行方法：ターミナル上で.\21B12111-02-04を実行
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <cmath>
#include <queue>
#include <cstdlib>
using namespace std;

typedef pair<double, double> PDD;

vector<char> symbols;
vector<double> probs;
map<char, PDD> code2intervals; //各符号に対する区間
int asize;

//[s, t] -> [s + (t-s)*p1, s + (t - s)*p2]
PDD encode(string intput_symbols)
{
    PDD res = make_pair(0, 1);
    for (auto c : intput_symbols)
    {
        double start = code2intervals[c].first, end = code2intervals[c].second;
        double length = res.second - res.first; // length of current interval
        double t = res.first;
        res.first = t + length * start, res.second = t + length * end; // update interval
    }
    return res;
}

string ToBinary(double target, int al)
{
    string res = "";
    for (int i = 0; i < al; i++)
    {
        if (target * 2 >= 1)
        {
            target = target * 2 - 1;
            res += "1";
        }
        else
        {
            target = target * 2;
            res += "0";
        }
    }
    return res;
}

string decode(string codewords, int len)
{
    //binary to double
    string res = "";
    double p = 0.0, delta = 0.5;
    for(int i = 0; i < codewords.size(); i ++)
    {
        if(codewords[i] == '1') p += delta;
        delta *= 0.5;
    }

    //decode
    for(int i = 0; i < len; i ++)
    {
        for(map<char, PDD>::iterator it = code2intervals.begin(); it != code2intervals.end(); it++)
        {
            double start = (it->second).first, end = (it->second).second;
            if(p >= start && p < end)
            {
                res += it->first;
                p = (p - start) / (end - start);
                break;
            }
        }
    }
    return res;
}

int main()
{
    printf("alphabet size> ");
    cin >> asize;

    char symb;
    double p, last = 0;
    for (int i = 0; i < asize; i++)
    {

        printf("symbol_%d: ", i + 1);
        cin >> symb;
        printf("p_%d: ", i + 1);
        cin >> p;
        symbols.push_back(symb);
        probs.push_back(p);
        code2intervals[symb] = make_pair(last, last + p);
        last += p;
    }

    //encode
    string input_symbols;
    printf("symbols> ");
    cin >> input_symbols;

    PDD range = encode(input_symbols);
    printf("range: [%.10lf, %.10lf)\n", range.first, range.second);
    double m = 0.5 * (range.second + range.first);
    int al = ceil(-log2(range.second - range.first)) + 1;

    string encoded = ToBinary(m, al);
    printf("encoded: %s\n", encoded.c_str());

    //decode
    printf("codewords> ");
    string codewords;
    cin >> codewords;

    printf("length> ");
    int length;
    cin >> length;

    string decoded = decode(codewords, length);
    printf("decoded: %s\n", decoded.c_str());
    return 0;
}