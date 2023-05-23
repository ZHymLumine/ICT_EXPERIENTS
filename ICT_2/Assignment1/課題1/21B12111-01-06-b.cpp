//張　一鳴、21B12111、ICT.E218
//プログラミング言語：C++
//コンパイル方法：21B12111-01-06-b.cpp -o 21B12111-01-06-b
//実行方法：ターミナル上で.\21B12111-01-06-b を実行
//答え:caacabbcacaccac
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