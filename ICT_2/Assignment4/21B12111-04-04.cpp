// 張　一鳴、21B12111、ICT.E218
//協力者：楊　家寧(21B51306)、劉　仕奇(21B51387)
// プログラミング言語：C++
// コンパイル方法：g++ 21B12111-04-04.cpp -o 21B12111-04-04
// 実行方法：ターミナル上で.\21B12111-04-04を実行
/*結果
OUTPUT:
[00100001]*[11111100]=[00110100]
[00010101]*[10101111]=[10110110]
[00111100]*[00111011]=[01000011]

[00000011]/[01110110]=[11011001]
[10100000]/[00110001]=[10100101]
[10111011]/[00010100]=[11000011]
 */
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

int m;
vector<vector<int>> originals;
vector<int> f;
map<int, int> inverse_one;

// 配列をoutputする
void print(vector<int> &vec)
{
    for (int i = 0; i < vec.size(); i++)
        cout << vec[i];
}

// 多項式足し算(引き算は足し算と同じ)
vector<int> add_poly(vector<int> &f, vector<int> &g)
{
    vector<int> res;
    if (f.size() < g.size())
        swap(f, g);
    for (int i = 0; i < f.size(); i++)
    {
        if (i < g.size())
            res.push_back(f[i] ^ g[i]);
        else
            res.push_back(f[i]);
    }
    return res;
}

// 多項式掛け算
vector<int> mul_poly(vector<int> &f, vector<int> &g)
{
    vector<int> res(m + m - 1);
    for (int i = 0; i < g.size(); i++)
    {
        if (g[i] == 1)
        {
            for (int j = 0; j < f.size(); j++)
            {
                res[i + j] = (res[i + j] + f[j]) % 2;
            }
        }
    }
    return res;
}

// 多項式割り算
vector<vector<int>> div_poly(vector<int> f, vector<int> g)
{
    vector<vector<int>> res;
    vector<int> quotient;
    reverse(f.begin(), f.end());
    reverse(g.begin(), g.end());

    vector<int> f_(f);
    int cnt = f_.size();
    for (int i = 0; i < f_.size(); i++)
    {
        if (f_[i] == 0)
            quotient.push_back(0);
        else
        {
            quotient.push_back(1);
            for (int j = i, k = 0; j < f_.size(); j++)
            {
                if (j < i + g.size())
                    f_[j] = (f_[j] + g[k++]) % 2;
            }
        }
        if (i + g.size() == f_.size())
            break;
    }
    vector<int> remainder(f_);
    reverse(quotient.begin(), quotient.end());
    reverse(remainder.begin(), remainder.end());
    while (remainder.back() == 0)
        remainder.pop_back();

    res.push_back(quotient);
    res.push_back(remainder);
    return res;
}

// 有限体の足し算
vector<int> add_field(vector<int> &a, vector<int> &b)
{
    vector<int> res = add_poly(a, b);
    return res;
}

// 有限体の掛け算
vector<int> mul_field(vector<int> a, vector<int> b)
{
    vector<int> tmp = mul_poly(a, b);
    while (tmp.back() == 0 && tmp.size() > m + 1)
        tmp.pop_back();
    vector<vector<int>> qr = div_poly(tmp, f);
    while (qr[1].size() < m)
        qr[1].push_back(0);

    // tmp = qr[1];
    return qr[1];
}

// 有限体の割り算
vector<int> div_field(vector<int> a, vector<int> b)
{
    vector<int> one;
    for (int i = 0; i < m; i++)
    {
        if (i == 0)
            one.push_back(1);
        else
            one.push_back(0);
    }
    vector<int> tmp;
    for (int k = 0; k < originals.size(); k++)
    {
        vector<int> t = mul_field(b, originals[k]);
        while (t.back() == 0 && t.size() > m)
            t.pop_back();
        if (t == one)
        {
            // cout << "yes" << endl;
            tmp = originals[k];
        }
    }
    vector<int> res = mul_field(a, tmp);
    return res;
}

// intを二進数の配列に変換する
vector<int> toBinary(int num)
{
    vector<int> res;
    if (num == 0)
    {
        res.push_back(0);
    }
    while (num > 0)
    {
        res.push_back(num % 2);
        num /= 2;
    }

    while (res.size() < m)
        res.push_back(0);
    return res;
}
// 有限体の加減乗除の結果をoutputする
void print_result(string op)
{
    for (int i = 0; i < originals.size(); i++)
    {
        printf(" [");
        print(originals[i]);
        printf("]");
    }
    printf("\n");
    printf("-----+-------------------------------------------------------\n");
    for (int i = 0; i < originals.size(); i++)
    {
        printf("[");
        print(originals[i]);
        printf("] | ");
        for (int j = 0; j < originals.size(); j++)
        {
            vector<int> tmp;
            if (op == "ADD" || op == "SUB")
            {
                printf("[");
                tmp = add_field(originals[i], originals[j]);
                print(tmp);
                printf("] ");
            }
            else if (op == "MUL")
            {
                printf("[");
                vector<int> res = mul_field(originals[i], originals[j]);
                print(res);
                printf("] ");
            }
            else if (op == "DIV")
            {
                if (j == 0)
                {
                    string t(m + 2, '-');
                    printf("%s ", t.c_str());
                }
                else if (i == 0)
                {
                    printf("[");
                    print(originals[i]);
                    printf("] ");
                }
                else
                {
                    printf("[");
                    vector<int> one;
                    for (int i = 0; i < m; i++)
                    {
                        if (i == 0)
                            one.push_back(1);
                        else
                            one.push_back(0);
                    }
                    vector<int> tmp;
                    for (int k = 0; k < originals.size(); k++)
                    {
                        vector<int> t = mul_field(originals[j], originals[k]);
                        while (t.back() == 0 && t.size() > m)
                            t.pop_back();
                        // print(t);
                        // cout << endl;
                        if (t == one)
                        {
                            // cout << "yes" << endl;
                            tmp = originals[k];
                            inverse_one[i] = k;
                        }
                    }
                    vector<int> res = mul_field(originals[i], tmp);
                    print(res);
                    printf("] ");
                }
            }
        }
        printf("\n");
    }
}

int main()
{
    printf("INPUT:\n");
    cout << "m=";
    cin >> m;
    string str;
    cout << "f(x)=";
    cin >> str;

    for (auto c : str)
    {
        if (c != '[' && c != ']')
            f.push_back(c - '0');
    }
    // for (int i = 2; i < str.size(); i++)
    //     m = m * 10 + str[i] - '0';

    // cin >> str;
    // for (int i = 6; i <= 6 + m; i++)
    //     f.push_back(str[i] - '0');

    for (int i = 0; i < 2 << m - 1; i++)
    {
        vector<int> t = toBinary(i);
        originals.push_back(t);
    }

    printf("OUTPUT:\n");
    // multiply
    //  vector<int> a = {0, 0, 1, 0, 0, 0, 0, 1}, b = {1, 1, 1, 1, 1, 1, 0, 0};
    //  vector<int> res = mul_field(a, b);
    //  printf("[00100001]*[11111100]=["); print(res); printf("]\n");

    // a = {0, 0, 0, 1, 0, 1, 0, 1}, b = {1, 0, 1, 0, 1, 1, 1, 1};
    // res = mul_field(a, b);
    // printf("[00010101]*[10101111]=["); print(res); printf("]\n");

    // a = {0, 0, 1, 1, 1, 1, 0, 0}, b = {0, 0, 1, 1, 1, 0, 1, 1};
    // res = mul_field(a, b);
    // printf("[00111100]*[00111011]=["); print(res); printf("]\n");

    // printf("\n");
    // // divide
    // a = {0, 0, 0, 0, 0, 0, 1, 1}, b = {0, 1, 1, 1, 0, 1, 1, 0};
    // res = div_field(a, b);
    // printf("[00000011]/[01110110]=["); print(res); printf("]\n");

    // a = {1, 0, 1, 0, 0, 0, 0, 0}, b = {0, 0, 1, 1, 0, 0, 0, 1};
    // res = div_field(a, b);
    // printf("[10100000]/[00110001]=["); print(res); printf("]\n");

    // a = {1, 0, 1, 1, 1, 0, 1, 1}, b = {0, 0, 0, 1, 0, 1, 0, 0};
    // res = div_field(a, b);

    // printf("[10111011]/[00010100]=["); print(res); printf("]\n");

    // a = {0, 1, 0, 0, 0, 0, 0, 0}, b = {0, 0, 0, 0, 0, 0, 0, 1};
    // res = mul_field(a, b);
    // print(res); cout << endl;
    printf("ADD  |");
    print_result("ADD");

    printf("MUL  |");
    print_result("MUL");

    printf("SUB  |");
    print_result("SUB");

    printf("DIV  |");
    print_result("DIV");

    return 0;
}
