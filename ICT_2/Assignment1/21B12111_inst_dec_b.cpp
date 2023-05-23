//張　一鳴、21B12111、ICT.E218
//プログラミング言語：C++
//コンパイル方法：g++ 21B12111_inst_dec_b.cpp -o 21B12111_inst_dec_b
//実行方法：ターミナル上で.\21B12111_inst_dec_b を実行
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
        if(last == "1") decode += "a", last = "";
        else if(last == "01") decode += "b", last = "";
        else if(last == "001") decode += "c", last = "";
    }
    cout << "symbols: " << decode;
    return 0;
}