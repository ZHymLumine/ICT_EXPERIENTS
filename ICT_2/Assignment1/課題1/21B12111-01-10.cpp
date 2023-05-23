//張　一鳴、21B12111、ICT.E218
//協力者：楊　家寧(21B51306)、劉　仕奇(21B51387)
//プログラミング言語：C++
//コンパイル方法：g++ 21B12111-01-10.cpp -o 21B12111-01-10
//実行方法：ターミナル上で.\21B12111-01-10を実行
//答え　abdacadabdaabdacadabda
#include<iostream>
#include <cstring>
#include<map>
using namespace std;

map<string, char> code;
string ans;

void init_code()
{
    code["10"] = 'a';
    code["00"] = 'b';
    code["11"] = 'c';
    code["110"] = 'd';
}

bool dfs(string& input, int pos, int start, string last)
{
    if(pos == input.size()) return true;
    for(int i = start; i < input.size(); i ++)
    {
        last += input[i];
        if(code.find(last) != code.end())
        {
            string back = ans;
            ans += code[last];
            if(dfs(input, pos + last.size(), i + 1, "")) return true;
            ans = back;
        }
    }
    return false;
}

int main() 
{
    init_code();
    string input;
    cout << "codewords> ";
    cin >> input;
    string decode;
    dfs(input, 0, 0, "");

    cout << "symbols: " << ans << endl;
    return 0;
}