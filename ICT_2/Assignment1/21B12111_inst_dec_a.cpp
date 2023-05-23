//張　一鳴、21B12111、ICT.E218
//プログラミング言語：C++
//コンパイル方法：g++ 21B12111_inst_dec_a.cpp -o 21B12111_inst_dec_a
//実行方法：ターミナル上で.\21B12111_inst_dec_a を実行
#include<iostream>
#include <cstring>
using namespace std;

int main() 
{
    string input;
    cout << "codewords> ";
    cin >> input;
    string decode = "";
    for(auto c:input)
    {
        if(c == '0') decode += 'a';
        else if(c == '1') decode += 'b';
    }
    cout << "symbols: " << decode;
    return 0;
}