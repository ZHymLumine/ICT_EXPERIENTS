// 張　一鳴、21B12111、ICT.E218
// プログラミング言語：C++
// コンパイル方法：g++ 21B12111-02-05.cpp -o 21B12111-02-05
// 実行方法：ターミナル上で.\21B12111-02-05を実行
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
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
map<int, char> num2code;
map<string, int> code2num;

void encode(string input)
{

    string current_symbol = "";
    int root = 0;
    tr[root] = Node(0, "", map<char, int>());
    for (int i = 0; i < input.size(); i++)
    {
        char c = input[i];
        current_symbol += c;
        if (tr[root].hash.find(c) != tr[root].hash.end())
        {
            root = tr[root].hash[c];
        }
        else
        {
            int t = root;
            tr[t].hash[c] = ++ idx; //新しい節点を作る
            tr[idx] = Node(idx, current_symbol, map<char, int>());
            current_symbol = "";
            root = 0;
        }
    }
}

int main()
{
    string symbols;
    int length;
    printf("symbols> ");
    cin >> symbols;
    encode(symbols);

    printf("encoded: ");
    for(int i = 0; i < idx; i++)
    {
        map<char, int> cur = tr[i].hash;
        for(map<char, int>::iterator it = cur.begin(); it != cur.end(); it++)
        {
            printf("(%d, %c) ", tr[i].id, it->first);
        }
    }
    printf("\n");

    printf("codewords length> ");
    cin >> length;

    string decoded = "";
    for(int i = 0; i < length; i ++)
    {
        int node;
        char symb;
        printf("node: ");
        cin >> node;
        printf("symbol: ");
        cin >> symb;

        decoded += tr[tr[node].hash[symb]].symbol;
    }
    printf("decoded: %s\n", decoded.c_str());
    return 0;
}