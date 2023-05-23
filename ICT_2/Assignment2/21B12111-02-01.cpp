//張　一鳴、21B12111、ICT.E218
//プログラミング言語：C++
//コンパイル方法：g++ 21B12111-02-01.cpp -o 21B12111-02-01
//実行方法：ターミナル上で.\21B12111-02-01を実行
/*P(aaaa): 0.000100
P(aaab): 0.000200
P(aaac): 0.000700
P(aaba): 0.000200
P(aabb): 0.000400
P(aabc): 0.001400
P(aaca): 0.000700
P(aacb): 0.001400
P(aacc): 0.004900
P(abaa): 0.000200
P(abab): 0.000400
P(abac): 0.001400
P(abba): 0.000400
P(abbb): 0.000800
P(abbc): 0.002800
P(abca): 0.001400
P(abcb): 0.002800
P(abcc): 0.009800
P(acaa): 0.000700
P(acab): 0.001400
P(acac): 0.004900
P(acba): 0.001400
P(acbb): 0.002800
P(acbc): 0.009800
P(acca): 0.004900
P(accb): 0.009800
P(accc): 0.034300
P(baaa): 0.000200
P(baab): 0.000400
P(baac): 0.001400
P(baba): 0.000400
P(babb): 0.000800
P(babc): 0.002800
P(baca): 0.001400
P(bacb): 0.002800
P(bacc): 0.009800
P(bbaa): 0.000400
P(bbab): 0.000800
P(bbac): 0.002800
P(bbba): 0.000800
P(bbbb): 0.001600
P(bbbc): 0.005600
P(bbca): 0.002800
P(bbcb): 0.005600
P(bbcc): 0.019600
P(bcaa): 0.001400
P(bcab): 0.002800
P(bcac): 0.009800
P(bcba): 0.002800
P(bcbb): 0.005600
P(bcbc): 0.019600
P(bcca): 0.009800
P(bccb): 0.019600
P(bccc): 0.068600
P(caaa): 0.000700
P(caab): 0.001400
P(caac): 0.004900
P(caba): 0.001400
P(cabb): 0.002800
P(cabc): 0.009800
P(caca): 0.004900
P(cacb): 0.009800
P(cacc): 0.034300
P(cbaa): 0.001400
P(cbab): 0.002800
P(cbac): 0.009800
P(cbba): 0.002800
P(cbbb): 0.005600
P(cbbc): 0.019600
P(cbca): 0.009800
P(cbcb): 0.019600
P(cbcc): 0.068600
P(ccaa): 0.004900
P(ccab): 0.009800
P(ccac): 0.034300
P(ccba): 0.009800
P(ccbb): 0.019600
P(ccbc): 0.068600
P(ccca): 0.034300
P(cccb): 0.068600
P(cccc): 0.240100*/
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