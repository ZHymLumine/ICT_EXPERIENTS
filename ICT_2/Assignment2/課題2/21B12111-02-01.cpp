//張　一鳴、21B12111、ICT.E218
//プログラミング言語：C++
//コンパイル方法：g++ 21B12111-02-01.cpp -o 21B12111-02-01
//実行方法：ターミナル上で.\21B12111-02-01を実行
#include<iostream>
#include<vector>
#include<string>
#include <algorithm>
#include <map>

using namespace std;
int asize, len;
vector<char> symbols;
vector<double> probs;
map<string, double> codes;

//u:current length, last:current code, prob:current probability
void dfs(int u, string last, double prob)
{
    if(u == len)
    {
        codes[last] = prob;
        return;
    }

    for(int i = 0; i < symbols.size(); i ++)
    {
        dfs(u + 1, last + symbols[i], prob * probs[i]);
    }
}

int main()
{
    printf("alphabet size> ");
    cin >> asize;
    printf("length> ");
    cin >> len;

    char symb;
    double p;
    for(int i = 0; i < asize; i ++)
    {
        
        printf("symbol_%d> ", i + 1);
        cin >> symb;
        printf("p_%d> ", i + 1);
        cin >> p;
        symbols.push_back(symb);
        probs.push_back(p);
    }

    dfs(0, "", 1);

    for(map<string, double>::iterator it = codes.begin(); it != codes.end(); it ++)
        printf("P(%s): %lf\n", (it->first).c_str(), it->second);

    return 0;
}