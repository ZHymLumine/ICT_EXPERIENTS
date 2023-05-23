//張　一鳴、21B12111、ICT.E218
//プログラミング言語：C++
//コンパイル方法：g++ 21B12111-03-02.cpp -o 21B12111-03-02
//実行方法：ターミナル上で.\21B12111-03-02を実行
//d_min=453
//R=0.00683594
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

int n, M;
vector<string> codes;

int haming(string& s1, string& s2)
{
    int d = 0;
    for(int i = 0; i < n; i ++)
        if(s1[i] != s2[i])
            d ++;
    return d;
}

int main()
{
    
    cout << "n=";
    cin >> n;
    cout << "M=";
    cin >> M;
    cout << "C=" << endl;

    for(int i = 0; i < M; i ++)
    {
        string str;
        cin >> str;
        codes.push_back(str);
    }

    int d_min = INT_MAX;
    for(int i = 0; i < codes.size(); i ++)
    {
        for(int j = i + 1; j < codes.size(); j ++)
        {  
            int d = haming(codes[i], codes[j]);
            d_min = min(d_min, d);
        }
    }

    cout << "OUTPUT:" << endl;
    cout << "d_min=" << d_min << endl;
    cout << "R=" << log2(M) / n << endl;
    return 0;
}