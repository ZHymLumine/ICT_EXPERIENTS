//張　一鳴、21B12111、ICT.E218
//プログラミング言語：C++
//コンパイル方法：g++ 21B12111-03-03.cpp -o 21B12111-03-03
//実行方法：ターミナル上で.\21B12111-03-03を実行
//i_MD=56
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
    string r;
    cout << "r=" << endl;
    cin >> r;

    int index, d_min = INT_MAX;
    for(int i = 0; i < codes.size(); i ++)
    {
        int d = haming(r, codes[i]);
        if(d < d_min)
        {
            d_min = d;
            index = i;
        }
    }

    cout << "OUTPUT:" << endl;
    cout << "hat_c=" << endl;
    cout << codes[index] << endl;
    cout << "i_MD=" << index << endl;
    return 0;
}