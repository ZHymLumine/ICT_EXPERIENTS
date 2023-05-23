// 張　一鳴、21B12111、ICT.E218
// プログラミング言語：C++
// コンパイル方法：g++ 21B12111-04-05.cpp -o 21B12111-04-05
// 実行方法：ターミナル上で.\21B12111-04-05を実行
/*結果
OUTPUT:
x=
[01001101]
[00010001]
[01110011]
[00010110]
[00001101]
[01011101]
[11001000]
[00001100]
 */
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

int n, m;
vector<vector<int>> originals; //代表元
vector<int> f;  //原始多項式
map<int, int> inverse_one;
vector<vector<vector<int>>> matrix;

// 配列をoutputする(debug用)
void print(vector<int> &vec)
{
    for (int i = 0; i < vec.size(); i++)
        cout << vec[i];
}

//行列をoutputする(debug用)
void print_matrix(vector<vector<vector<int>>> matrix)
{
    for (auto vec2 : matrix)
    {
        for (auto vec : vec2)
        {
            print(vec);
            printf(" ");
        }
        cout << endl;
    }
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

//intを二進数の配列に変換する
vector<int> toBinary(int num)
{
    vector<int> res;
    if(num == 0) 
    {
        res.push_back(0);
    }
    while(num > 0)
    {
        res.push_back(num % 2);
        num /= 2;
    }

    while(res.size() < m) res.push_back(0);
    return res;
}

// gauss elimination
vector<vector<vector<int>>> gauss(vector<vector<vector<int>>> &a)
{
    // 単位元を生成する
    vector<int> one, zero;
    for (int i = 0; i < n; i++)
    {
        zero.push_back(0);
        if (i == 0)
            one.push_back(1);
        else
            one.push_back(0);
    }

    int c, r;
    for (c = 0, r = 0; c < n; c++)
    {
        int t = r;
        for (int i = r; i < n; i++) // find non-zero row
            if (a[i][c] != zero)
                t = i;

        if (a[t][c] == zero)
            continue;
    
        for (int i = c; i <= n; i++)
            swap(a[r][i], a[t][i]); // move the non-zero row to the first row

        for (int i = n; i >= c; i--)
        {
            vector<int> divide = a[r][c];
            a[r][i] = div_field(a[r][i], divide);
        }
        for (int i = 0; i < n; i++)
        {
            if ((a[i][c] != zero) && (i != r))
                for (int j = n; j >= c; j--)
                {
                    vector<int> subtraction = mul_field(a[r][j], a[i][c]);
                    a[i][j] = add_field(a[i][j], subtraction);
                }
        }
        r++;
    }

    // 後退代入
    for (int i = n - 1; i >= 0; i--)
        for (int j = i + 1; j < n; j++)
        {
            vector<int> subtraction = mul_field(a[i][j], a[j][n]);
            a[i][n] = add_field(a[i][n], subtraction);
        }
    return a;
}

int main()
{
    printf("INPUT:\n");
    cout << "m=";
    cin >> m;
    cout << "n=";
    cin >> n;
    cout << "f(x)=";

    string str;
    cin >> str;

    //input f
    for (auto c : str)
    {
        if (c != '[' && c != ']')
            f.push_back(c - '0');
    }
    // for (int i = 2; i < str.size(); i++)
    //     m = m * 10 + str[i] - '0';

    // cin >> str;
    // for (int i = 2; i < str.size(); i++)
    //     n = n * 10 + str[i] - '0';

    // input f
    // cin >> str;
    // for (int i = 6; i <= 6 + m; i++)
    //     f.push_back(str[i] - '0');

    // input A
    cout << "A=" << endl;
    for (int i = 0; i < n; i++)
    {
        cin >> str;
        vector<vector<int>> row;
        vector<int> tmp;
        for (auto c : str)
        {
            if (c == '[')
                continue;
            else if (c == ']')
            {
                row.push_back(tmp);
                tmp.clear();
                continue;
            }
            else
                tmp.push_back(c - '0');
        }
        matrix.push_back(row);
    }

    // input b
    cout << "b=" << endl;
    for (int i = 0; i < n; i++)
    {
        cin >> str;
        vector<int> tmp;
        for (auto c : str)
        {
            if (c == '[')
                continue;
            else if (c == ']')
            {
                matrix[i].push_back(tmp);
                continue;
            }
            else
                tmp.push_back(c - '0');
        }
    }

    for (int i = 0; i < 2 << m - 1; i++)
    {
        vector<int> t = toBinary(i);
        originals.push_back(t);
    }

    vector<vector<vector<int>>> res = gauss(matrix);
    printf("OUTPUT:\n");
    printf("x=\n");
    for (int i = 0; i < n; i++)
    {
        printf("[");
        print(res[i][n]);
        printf("]\n");
    }
    return 0;
}
