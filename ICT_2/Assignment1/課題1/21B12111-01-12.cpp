//張　一鳴、21B12111、ICT.E218
//プログラミング言語：C++
//コンパイル方法：g++ 21B12111-01-12.cpp -o 21B12111-01-12
//実行方法：ターミナル上で.\21B12111-01-12を実行
//答え　abaaadbddcabcabc
#include<iostream>
#include <cstring>
#include <vector>
#include<map>
using namespace std;

map<string, char> code;
string ans;
int asize;

string decode(string& input)
{
    string res = "", last = "";
    for(auto c: input)
    {
        last += c;
        if(code.find(last) != code.end())
        {
            res += code[last];
            last = "";
        }
    }
    return res;
}

int main()
{
    cout << "alphabet size> ";
    cin >> asize;
    vector<char> symbol(asize);
    vector<string> codeword(asize);
    for(int i = 0; i < asize; i ++)
    {
        printf("symbol_%d> ", i + 1);
        cin >> symbol[i];
        printf("codeword_%d> ", i + 1);
        cin >> codeword[i];

        code[codeword[i]] = symbol[i];
    }

    printf("codewords> ");
    string input;
    cin >> input;

    string res = decode(input);
    cout << "symbols: " << res << endl;

    return 0;
}
