//張　一鳴、21B12111、ICT.E218
//プログラミング言語：C++
//コンパイル方法：g++ 21B12111_given_inst_enc.cpp -o 21B12111_given_inst_enc
//実行方法：ターミナル上で.\21B12111_given_inst_encを実行
#include<iostream>
#include <cstring>
#include <vector>
#include<map>
using namespace std;

map<char, string> code;
string ans;
int asize;

string encode(string& input)
{
    string res = "", last = "";
    for(auto c: input)
    {
        if(code.find(c) != code.end())
        {
            res += code[c];
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

        code[symbol[i]] = codeword[i];
    }

    printf("symbols> ");
    string input;
    cin >> input;

    string res = encode(input);
    cout << "codewords: " << res << endl;

    return 0;
}
