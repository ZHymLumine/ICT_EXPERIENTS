//張　一鳴、21B12111、ICT.E218
//プログラミング言語：C++
//コンパイル方法：g++ 21B12111_inst_enc_b.cpp -o 21B12111_inst_enc_b
//実行方法：ターミナル上で.\21B12111_inst_enc_b を実行
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
        if(c == 'a') encode += "1";
        else if(c == 'b') encode += "01";
        else if(c == 'c') encode += "001";
    }
    cout << "codewords: " << encode;
    return 0;
}