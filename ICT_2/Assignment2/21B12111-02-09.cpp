// 張　一鳴、21B12111、ICT.E218
//協力者：楊　家寧(21B51306)、劉　仕奇(21B51387)
// プログラミング言語：C++
// コンパイル方法：g++ 21B12111-02-09.cpp -o 21B12111-02-09
// 実行方法：ターミナル上で.\21B12111-02-09を実行
/*
encoded: 010101100000001000100110101000101011000011101
codewords> 010101100000001000100110101000101011000011101
decoded: aadaadaabcccbdbcbacc */
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>
using namespace std;

typedef long long LL;
typedef pair<char, double> PCD;
map<char, int> index_map;
int w = 10, u = 1000;

//数値を01の文字列に変換する
string toBinaryString(LL num)
{
    string res = "";
    if (num == 0)
        return res + "0";
    while (num > 0)
    {
        res += to_string(num % 2);
        num /= 2;
    }
    reverse(res.begin(), res.end());
    return res;
}

//累積頻度を計算する
vector<int> make_freq(vector<PCD> &symbols)
{
    vector<int> freqs;
    int f = 0;
    double p = 0;
    for (int i = 0; i < symbols.size(); i++)
    {
        f = floor(0.5 + u * (p + symbols[i].second));
        freqs.push_back(f);
        p += symbols[i].second;
    }
    return freqs;
}

//encode
string encode(string input, vector<int> &freqs)
{
    LL X = 0, Y = (LL)pow(2, w), L = 0;
    LL low = pow(2, w), high = 2 * low;

    for (auto c : input)
    {
        int i = index_map[c];
        LL V = (LL)floor((double)Y * freqs[i] / u + 0.5) - floor((double)Y * freqs[i - 1] / u + 0.5);
        LL s;
        for (s = 0;; s++) // find s
        {
            LL res = V * pow(2, s);
            if (res >= low && res < high)
                break;
        }
        X = (X + (LL)floor((double)Y * freqs[i - 1] / u + 0.5)) * pow(2, s);
        Y = V * pow(2, s);
        L = L + s;
    }
    LL res = X >> w;

    string encoded = toBinaryString(res);
    if (encoded.size() < L)
    {
        encoded = string(L - encoded.size(), '0') + encoded;
    }
    return encoded;
}

//復号するときにXを計算する
LL calc_X(string str)
{
    reverse(str.begin(), str.end());
    LL res = 0;
    LL t = 1;
    for (int i = 0; i < str.size(); i++)
    {
        LL num = str[i] - '0';
        res = res + num * t;
        t *= 2;
    }
    return res;
}

//decode
string decode(string codewords, vector<int> &freqs, vector<PCD>& symbols)
{
    string decoded = "";
    // step(1)
    codewords = codewords + string(w, '1');
    string most_w_bits = "";
    for (int i = 0; i < w; i++) most_w_bits += codewords[i];
    LL X = calc_X(most_w_bits), Y = pow(2, w), L = 0;

    // step(2)
    for (int i = w; i < codewords.size(); i++)
    {
        LL e = floor((u * (2 * X + 1) - 1) / (2 * Y));
        int index;
        for (index = 1; index < freqs.size(); index++)
            if (e >= freqs[index - 1] && e < freqs[index])
                break;
        

        decoded += symbols[index].first;        //output the symbol
        LL Z = floor((double)Y * freqs[index - 1] / u + 0.5);
        X = X - Z;
        LL V = floor((double)Y * freqs[index] / u + 0.5) - Z;
        LL s;
        LL low = pow(2, w), high = 2 * low;
        for (s = 0;; s++) // find s
        {
            LL res = V * pow(2, s);
            if (res >= low && res < high)
                break;
        }
        X = X * pow(2, s);
        string s_bits = "";
        for (int k = i; k < i + s; k ++) s_bits += codewords[k];
        i = i + s - 1;
        
        Y = V*pow(2, s);
        //cout << X << ' ' << e << ' ' << Z << ' ' << V << ' ' << s << ' '<< Y << endl;
        X = X + calc_X(s_bits);

    }
    return decoded;
}

int main()
{
    int asize;
    printf("alphabet size> ");
    cin >> asize;

    vector<PCD> symbols(asize + 1);
    for (int i = 1; i <= asize; i++)
    {
        char symb;
        double p;
        printf("symbol_%d: ", i);
        cin >> symb;
        printf("p_%d: ", i);
        cin >> p;
        index_map[symb] = i;
        symbols[i] = make_pair(symb, p);
    }

    vector<int> freqs = make_freq(symbols);

    // encode
    string input_symbols;
    printf("symbols> ");
    cin >> input_symbols;

    string encoded = encode(input_symbols, freqs);
    printf("encoded: %s\n", encoded.c_str());

    // decode
    string codewords;
    printf("codewords> ");
    cin >> codewords;

    string decoded = decode(codewords, freqs, symbols);
    printf("decoded: %s\n", decoded.c_str());

    return 0;
}