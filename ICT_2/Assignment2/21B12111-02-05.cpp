// 張　一鳴、21B12111、ICT.E218
//協力者：楊　家寧(21B51306)、劉　仕奇(21B51387)
// プログラミング言語：C++
// コンパイル方法：g++ 21B12111-02-05.cpp -o 21B12111-02-05
// 実行方法：ターミナル上で.\21B12111-02-05を実行
/*encoded: (0, ,) (0, .) (0, 1) (0, 7) (0, 9) (0, A) (0, D) (0, E) (0, F) (0, G) (0, I) (0, J) (0, L) (0, N) (0, S) (0, Z) (0, _) (0, a) (0, b) (0, c) (0, d) (0, e) (0, f) (0, g) (0, h) (0, i) (0, k) (0, l) (0, m) (0, n) (0, o) (0, p) (0, q) (0, r) (0, s) (0, t) (0, u) (0, v) (0, w) (0, y) (1, Z) (3, 7) (3, 8) (3, _) (5, c) (5, h) (5, l) (5, m) (5, n) (5, r) (5, s) (5, t) (6, _) (6, c) (6, g) (6, o) (6, s) (6, y) (7, _) (7, a) (7, e) (7, i) (8, 1) (8, A) (8, B) (8, L) (8, T) (8, Z) (8, a) (8, b) (8, c) (8, f) (8, i) (8, l) (8, r) (8, s) (8, t) (8, u) (8, v) (9, 2) (9, M) (10, .) (11, l) (11, n) (11, s) (12, _) (12, e) (12, m) (12, s) (13, ,) (13, _) (13, c) (13, d) (13, l) (13, m) (13, r) (13, s) (13, v) (14, h) (14, w) (15, e) (15, m) (16, a) (16, b) (17, a) (17, h) (17, i) (17, o) (18, n) (19, _) (19, b) (19, m) (19, n) (19, r) (19, s) (22, _) (22, e) (22, h) (22, i) (22, s) (23, g) (23, i) (23, s) (23, u) (24, ,) (24, e) (24, i) (24, p) (25, f) (25, o) (25, p) (25, t) (28, o) (30, a) (30, e) (31, s) (33, c) (33, n) (33, o) (33, s) (33, t) (33, v) (35, g) (36, _) (37, _) (37, i) (37, m) (39, s) (41, d) (41, i) (41, s) (42, a) (42, i) (42, r) (45, _) (46, n) (47, P) (47, s) (49, l) (50, .) (52, .) (52, _) (57, Z) (60, d) (61, Z) (61, i) (63, a) (67, c) (67, f) (68, 9) (71, _) (74, h) (75, _) (75, i) (75, m) (77, i) (80, a) (85, S) (85, W) (87, o) (87, t) (101, t) (102, e) (103, _) (103, a) (103, p) (106, e) (110, o) (119, l) (120, e) (124, _) (129, T) (131, _) (135, m) (149, h) (156, o) (162, g) (168, e) (181, _) (182, P) (183, L) (187, _)
codewords length> 10
node: 0
symbol: L
node: 0
symbol: Z
node: 0
symbol: 7
node: 3
symbol: _
node: 0
symbol: a
node: 0
symbol: n
node: 0
symbol: d
node: 0
symbol: _
node: 1
symbol: Z
node: 3
symbol: 8
decoded: LZ77_and_LZ78 */
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

//encode
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