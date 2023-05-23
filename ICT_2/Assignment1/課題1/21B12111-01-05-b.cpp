//張　一鳴、21B12111、ICT.E218
//プログラミング言語：C++
//コンパイル方法：g++ 21B12111-01-05-b.cpp -o 21B12111-01-05-b
//実行方法：ターミナル上で.\21B12111-01-05-b を実行
//答え:0011111110011100111100101110010101010010011010100100101
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