//張　一鳴、21B12111、ICT.E218
//プログラミング言語：C++
//コンパイル方法：g++ 21B12111_inst_enc_a.cpp -o 21B12111_inst_enc_a
//実行方法：ターミナル上で.\21B12111_inst_enc_a を実行
#include<iostream>
#include <cstring>
using namespace std;

int main() 
{
    string input;
    cout << "symbols> ";
    cin >> input;
    string encode = "";
    for(auto c:input)
    {
        if(c == 'a') encode += '0';
        else if(c == 'b') encode += '1';
    }
    cout << "codewords: " << encode;
    return 0;
}