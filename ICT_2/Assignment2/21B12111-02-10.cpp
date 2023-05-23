// 張　一鳴、21B12111、ICT.E218
// プログラミング言語：C++
// コンパイル方法：g++ 21B12111-02-10.cpp -o 21B12111-02-10
// 実行方法：ターミナル上で.\21B12111-02-10を実行
/*encoded: 0100110000101101000001101111101011111000011000010000110111000001100100000010111110001010110100011001110001000011000010000011100100000011001011000011101000000011010001101010111110000001110100000000111011100000011011110100001101100100110111001100000011100110000001101100011010111001110110010111110011101100001100010110000101000011000111001101101101000000111000001100011001011011001110011000000011010010100110110111000101101101100000000011001110100110111001010000101110100001111011011010110010111000000000001110101000000011000100101110110100101011001101000001101011001000010000110100100011001011111011110011000010111100110010100110001110011001000011000100000000111100100100001000001101010011100100001010110100000010101101101001000010011000011010110110111000101101100001011011011100001110101111100000001001010000101011000110100110110001000010000101101001000010111011001011100110111000010000011000100000000011100100000110011011101111000110010010001000011100100010100010111000010000101010000011110110010101101000101111100001010111001000100000110000100101110111001100100110101111100000000110101100001100110111100100100110111000010110111001101110010101101000000000011000110001110101111100010010011001000010000111001000110000111000000011010110001100100010110100100000000111011000011010110110001101000010111010010100110100000110000110010100011100111011110100000110000100101110110011101001010110100100100010110100000000000110110100110010110011001001010110110100011100110100000100000110001000001010111001101000010111001100010000110011001001010101111111001110110000100001100111100100010000111011010011010110100100001010111010001000010110111100001100111001110000110110001100101110111010100001110110100100001100110011110101010101011100000000010110010101010101001100000000101001111111000101111100010010100110100000000010000010101011101101111011001100110010100110010001011100000100001000010000110000110100100000111011001010001100101110100010010110110100100001100010111110011111101100001100001110110110100100001011000110100001101100110011110000110010100000110011000110000110100101100011010100110010100010110011001010010001101100111011001010111010000100111011100110110111001101111000011000110110100101101010111111000001101011111001010100110000100010110011010010001100101101111000000000110011000001000011100110000110101110110000011010111001000000101011011000000100001110101001010100110100100000000011100010010100101101001000100010110111100101001011100110001110001101111011001110111000000011111011100111001110001101111001011110111001100000000011000110100101101101101000110000010110001110111011011000010100101100100001000010110111000100100010111110000000001000111000000000100100100000000010001100101011101110100010010110101111100000000010001000000000001000101101101110100110010000001010101001011101101011111101000100110011110010101011010000110011101011111101010000110010100111101011010010010111101010000000000000100111010110101010111110000010101101110001111010101101010110110010100000000000000101110
decoded: They_are_both_theoretically_dictionary_coders.*/
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

//数値を01の文字列に変換する
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

//01の文字列をint型の整数に変換する
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

//encode
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

//decode
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