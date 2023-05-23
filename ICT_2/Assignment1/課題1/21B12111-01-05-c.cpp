//張　一鳴、21B12111、ICT.E218
//プログラミング言語：C++
//コンパイル方法：g++ 21B12111-01-05-c.cpp -o 21B12111-01-05-c
//実行方法：ターミナル上で.\21B12111-01-05-c を実行
//答え:010110010011111010111111100111011010100010100110110100110111111
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
        if(c == 'a') encode += "0";
        else if(c == 'b') encode += "10";
        else if(c == 'c') encode += "110";
        else encode += "111";
    }
    cout << "codewords: " << encode;
    return 0;
}