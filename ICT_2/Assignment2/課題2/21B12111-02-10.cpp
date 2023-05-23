// 張　一鳴、21B12111、ICT.E218
// プログラミング言語：C++
// コンパイル方法：g++ 21B12111-02-10.cpp -o 21B12111-02-10
// 実行方法：ターミナル上で.\21B12111-02-10を実行
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <cmath>
#include <bitset>
#include <cstring>
using namespace std;

const int N = 100010;
struct Node
{
    int id;        // 番号
    string symbol; // 文字列
    map<char, int> hash;

    Node():id(), symbol(), hash(){}
    Node(int ID, string s, map<char, int>m): id(ID), symbol(s), hash(m){}
} tr[N];

int idx; // 参照番号
map<string, string> symbol2code;
map<string, string> codes;

string toBinary(int num)
{
    string res = "";
    if(num == 0) return res + "0";
    while(num > 0)
    {
        res += to_string(num % 2);
        num /= 2;
    }
    reverse(res.begin(), res.end());
    return res;
} 

int toDecimal(string str)
{
    reverse(str.begin(), str.end());
    int res = 0;
    int t = 1;
    for(int i = 0; i < str.size(); i ++)
    {
        int num = str[i] - '0';
        res = res + num * t;
        t *= 2;
    }
    return res;
}

string encode(string input)
{
    string res = "";
    string current_symbol = "";
    int root = 0;
    tr[root] = Node(0, "", map<char, int>());
    for (int i = 0; i < input.size(); i++)
    {
        char c = input[i];
        current_symbol += c;
        if (tr[root].hash.find(c) != tr[root].hash.end())
        {
            if(i == input.size() - 1) 
            {
                string head = toBinary(root);
                res += head + symbol2code[current_symbol];
                break;
            }
            root = tr[root].hash[c];
        }
        else
        {
            string head;
            if(i) head = toBinary(root); //一番目符号の０を省略
            int ch = c;
            bitset<8> tail = ch;         //記号の2進数表示
            int bits = ceil(log2(idx + 1));
            if(head.size() < bits) head =  string(bits - head.size(), '0') + head; //先頭に０を追加する

            string code = head + tail.to_string();  //符号語
            res += code;

            //新しい節点を作る
            tr[root].hash[c] = ++ idx; 
            tr[idx] = Node(idx, current_symbol, map<char, int>());

            codes[code] = current_symbol;
            symbol2code[current_symbol] = code;
            current_symbol = "";
            root = 0;
        }
    }
    return res;
}

string decode(string codewords)
{
    //memset(tr, 0, sizeof(tr));
    int cnt = 1;
    string decoded = "", last = "";
    int root = 0;
    for(int i = 0; i < codewords.size(); i++)
    {
        if(!i) //一番目の符号先頭の0が省略される
        {
            int j;
            for(j = i; j < 8; j ++) last += codewords[j];
            char c = toDecimal(last);
            decoded += c;
            cnt ++;
            i = j - 1;
            
            tr[root] = Node(0, "", map<char, int>());

            tr[root].hash[c] = cnt - 1;
            tr[cnt - 1] = Node(cnt - 1, decoded, map<char, int>());
        }
        else
        {
            string number = ""; //先頭の番号
            int j;
            //先頭の参照する番号を読み込む
            for(j = i; j < i + ceil(log2(cnt)); j ++) number += codewords[j];
            string ss = "";
            int k = j + 8;
            for(; j < k; j ++) ss += codewords[j];   //8桁のASCIIコードを読み込む
            
            int node = toDecimal(number);
            char symb = toDecimal(ss);
            if(tr[node].hash.find(symb) != tr[node].hash.end())
            {
                decoded += tr[node].symbol + symb;
            }
            else
            {
                int t = node;
                string current_symbol = tr[t].symbol + symb;
                decoded += current_symbol;
                tr[t].hash[symb] = cnt; //新しい節点を作る
                tr[cnt] = Node(cnt, current_symbol, map<char,int>());
                current_symbol = "";    
                cnt ++;    //辞書のノードの数をプラス1
            }
            i = j - 1;
        }
    }
    return decoded;
}

int main()
{
    string symbols;
    int length;
    printf("symbols> ");
    cin >> symbols;

    //encode
    string encoded = encode(symbols);
    printf("encoded: ");
    cout << encoded << endl;

    //decode
    string codewords;
    printf("codewords> ");
    cin >> codewords;

    string decoded = decode(codewords);
    printf("decoded: ");
    cout << decoded << endl;
    return 0;
}