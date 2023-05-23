//張　一鳴、21B12111、ICT.E218
//プログラミング言語：C++
//コンパイル方法：g++ 21B12111_inst_dec_c.cpp -o 21B12111_inst_dec_c
//実行方法：ターミナル上で.\21B12111_inst_dec_cを実行
#include<iostream>
#include <cstring>
using namespace std;

int main() 
{
    string input;
    cout << "codewords> ";
    cin >> input;
    string decode = "";
    string last = "";
    for(auto c:input)
    {
        last += c;
        if(last == "0") decode += "a", last = "";
        else if(last == "10") decode += "b", last = "";
        else if(last == "110") decode += "c", last = "";
        else if(last == "111") decode += "d", last = "";
    }
    cout << "symbols: " << decode;
    return 0;
}