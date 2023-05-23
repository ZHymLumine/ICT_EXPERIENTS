//張　一鳴、21B12111、ICT.E218
//プログラミング言語：C++
//コンパイル方法：g++ 21B12111-02-06.cpp -o 21B12111-02-06
//実行方法：ターミナル上で.\21B12111-02-06を実行
#include<iostream>
#include<vector>
#include<string>
#include <algorithm>
#include <map>

using namespace std;
typedef pair<int, int> PII;

int asize, len;
vector<char> symbols;
vector<double> probs; //生成確率
map<PII, double> condition_probs;   //条件付き確率
map<string, double> codes;  //各符号語の確率

//u:current length, last:current code, prob:current probability
void dfs(int u, string last, int prev, double prob)
{
    if(u == len)
    {
        codes[last] = prob;
        return;
    }

    for(int i = 0; i < symbols.size(); i ++)
    {
        if(u == 0) dfs(u + 1, last + symbols[i], i + 1, prob * probs[i]);
        else dfs(u + 1, last + symbols[i], i+1, prob * condition_probs[make_pair(prev, i+1)]);
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

    for(int i = 0; i < asize; i ++)
        for(int j = 0; j < asize; j ++)
        {
            printf("cp_%d%d> ", i + 1, j + 1);
            double p;
            cin >> p;
            condition_probs[make_pair(i+1, j+1)] = p;
        }

    dfs(0, "", 0, 1);

    for(map<string, double>::iterator it = codes.begin(); it != codes.end(); it ++)
        printf("P(%s): %lf\n", (it->first).c_str(), it->second);

    return 0;
}