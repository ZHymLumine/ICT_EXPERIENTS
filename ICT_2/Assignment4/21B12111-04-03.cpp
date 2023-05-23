// 張　一鳴、21B12111、ICT.E218
//協力者：楊　家寧(21B51306)、劉　仕奇(21B51387)
// プログラミング言語：C++
// コンパイル方法：gg++ 21B12111-04-03.cpp -o 21B12111-04-03
// 実行方法：ターミナル上で.\21B12111-04-03を実行
/*結果
OUTPUT:
f+g=[11010011100110101110100011]
f-g=[11010011100110101110100011]
f*g=[01001111011010100001000100000010011001000111]
f/g= quotient [11110111] remainder [101010001000000111]
*/
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

int n, m;

// 配列をoutputする
void print(vector<int> &vec)
{
    for (int i = 0; i < vec.size(); i++)
        cout << vec[i];
}

// 多項式足し算
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
    if (f.size() == g.size() && f[f.size() - 1] == 1 && g[g.size() - 1] == 1)
        res.push_back(1);
    return res;
}

// 多項式掛け算
vector<int> mul_poly(vector<int> &f, vector<int> &g)
{
    vector<int> res(n + m + 1);
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

// 多項式の割り算
vector<vector<int>> div_poly(vector<int> &f, vector<int> &g)
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

int main()
{
    printf("INPUT:\n");
    cout << "n=";
    cin >> n;
    cout << "m=";
    cin >> m;
    cout << "f=";

    vector<int> f;
    vector<int> g;
    string str;
    cin >> str;

    for (auto c : str)
    {
        if (c != '[' && c != ']')
            f.push_back(c - '0');
    }
    cout << "g=";

    cin >> str;
    for (auto c : str)
    {
        if (c != '[' && c != ']')
            g.push_back(c - '0');
    }
    // cin >> str;
    // for (int i = 2; i < str.size(); i++)
    //     n = n * 10 + str[i] - '0';
    // cin >> str;
    // for (int i = 2; i < str.size(); i++)
    //     m = m * 10 + str[i] - '0';

    // cin >> str;
    // for (int i = 3; i <= 3 + n; i++)
    //     f.push_back(str[i] - '0');
    // cin >> str;
    // for (int i = 3; i <= 3 + m; i++)
    //     g.push_back(str[i] - '0');

    vector<int> add = add_poly(f, g);
    vector<int> sub = add_poly(f, g);
    vector<int> mul = mul_poly(f, g);
    vector<vector<int>> div = div_poly(f, g);

    printf("OUTPUT:\n");
    printf("f+g=[");
    print(add);
    printf("]\n");
    printf("f-g=[");
    print(sub);
    printf("]\n");
    printf("f*g=[");
    print(mul);
    printf("]\n");
    printf("f/g= quotient [");
    print(div[0]);
    printf("] remainder [");
    print(div[1]);
    printf("]\n");
    return 0;
}
