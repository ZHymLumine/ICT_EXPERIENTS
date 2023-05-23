// 張　一鳴、21B12111、ICT.E218
//協力者：楊　家寧(21B51306)、劉　仕奇(21B51387)
// プログラミング言語：C++
// コンパイル方法：g++ 21B12111-04-06-c.cpp -o 21B12111-04-06-c
// 実行方法：ターミナル上で.\21B12111-04-06-cを実行
/*結果
[00011110][11101101][01011111][10101010]
 */
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>
using namespace std;

int n, k, t;
const int m = 8;
vector<vector<int>> primitives;
vector<int> f; // 原始多項式
vector<vector<vector<int>>> A;

// 配列をoutputする(debug用)
void print(vector<int> &vec)
{
    for (int i = 0; i < vec.size(); i++)
        cout << vec[i];
}

void print_matrix(vector<vector<int>> &m)
{
    for (auto vec : m)
    {
        print(vec);
        cout << endl;
    }
}
// 行列をoutputする(debug用)
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
            tmp = originals[k];
        }
    }
    vector<int> res = mul_field(a, tmp);
    return res;
}


//-q0/q1を計算する
vector<vector<int>> div_poly_matrix(vector<vector<int>> f, vector<vector<int>> g)
{
    int len1 = f.size();
    int len2 = g.size();
    reverse(f.begin(), f.end());
    reverse(g.begin(), g.end());

    vector<vector<int>> res;
    for (int i = 0; i < f.size(); i++)
    {
        vector<int> q = f[i]; // quotient
        res.push_back(q);
        for (int j = 0; j < g.size(); j++)
        {
            vector<int> tmp = mul_field(q, g[j]);
            f[i + j] = add_field(f[i + j], tmp);
        }

        if (i + g.size() == f.size())
            break;
    }
    reverse(res.begin(), res.end());
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

// 原始元を生成する(1, alpha ..., aplha^(254))
void init()
{
    vector<int> one, alpha;
    for (int i = 0; i < m; i++)
    {
        if (i == 0)
            one.push_back(1);
        else
            one.push_back(0);
        if (i == 1)
            alpha.push_back(1);
        else
            alpha.push_back(0);
    }
    primitives.push_back(one);
    primitives.push_back(alpha);

    vector<int> tmp = alpha;
    for (int i = 0; i < n - 2; i++)
    {
        tmp = mul_field(tmp, alpha);
        primitives.push_back(tmp);
    }
}

// 行列を転置する
vector<vector<vector<int>>> transpose(vector<vector<vector<int>>> &M)
{
    int k = M.size(), n = M[0].size();
    vector<vector<vector<int>>> M_T(n, vector<vector<int>>(k, vector<int>(m)));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < k; j++)
            M_T[i][j] = M[j][i];
    }
    return M_T;
}

// gauss elimination
vector<vector<vector<int>>> gauss(vector<vector<vector<int>>> &a)
{
    cout << endl;
    // 単位元を生成する
    vector<int> one, zero;
    for (int i = 0; i < m; i++)
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

// 生成行列Gの一行を生成して戻り値として返す
vector<vector<int>> createA(vector<vector<int>> &alpha, vector<vector<int>> &o)
{
    vector<vector<int>> res;
    for (int i = 0; i < alpha.size(); i++)
    {
        vector<int> tmp = mul_field(alpha[i], o[i]);
        res.push_back(tmp);
    }
    return res;
}

// 有限体の行列の掛け算
vector<vector<vector<int>>> product(vector<vector<vector<int>>> &a, vector<vector<vector<int>>> &b)
{
    // vector<vector<vector<int>>> res;
    int k = a.size(), n = b[0].size();
    int t = b.size();
    vector<vector<vector<int>>> res(k, vector<vector<int>>(n, vector<int>(m)));

    for (int i = 0; i < k; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < t; k++)
            {
                vector<int> tmp = mul_field(a[i][k], b[k][j]);
                tmp = add_field(res[i][j], tmp);
                res[i][j] = tmp;
            }

    return res;
}

int main()
{
    string str;
    printf("INPUT:\n");
    printf("f(x)=");
    cin >> str;
    for (auto c : str)
    {
        if (c == '[')
            continue;
        else if (c == ']')
            continue;
        else
            f.push_back(c - '0');
    }
    printf("n=");
    cin >> n;
    printf("k=");
    cin >> k;
    printf("r=\n");
    cin >> str;

    t = ceil((n - k) / 2);
    vector<vector<int>> r;
    vector<int> cash;
    for (auto c : str)
    {
        if (c == '[')
            continue;
        else if (c == ']')
        {
            r.push_back(cash);
            cash.clear();
            continue;
        }
        else
            cash.push_back(c - '0');
    }

    vector<int> one;
    for (int i = 0; i < m; i++)
    {
        if (i == 0)
            one.push_back(1);
        else
            one.push_back(0);
    }

    init(); // 原始元を生成する
    vector<vector<int>> tmp;
    for (int i = 0; i < n; i++)
    {
        tmp.push_back(one);
    }

    for (int i = 0; i < 2 << m - 1; i++)
    {
        vector<int> t = toBinary(i);
        originals.push_back(t);
    }
    A.push_back(tmp); // 生成行列の第一行
    // A.push_back(primitives);    //生成行列の第二行
    // tmp = primitives;
    for (int i = 1; i <= n - t - 1; i++)
    {
        tmp = createA(tmp, primitives);
        A.push_back(tmp);
    }
    A.push_back(r);
    tmp = r;
    for (int i = 1; i <= t; i++)
    {
        tmp = createA(tmp, primitives);
        A.push_back(tmp);
    }

    A = transpose(A);

    vector<vector<vector<int>>> res = gauss(A);
    vector<vector<int>> Q0, Q1;
    printf("OUTPUT:\n");
    for (int i = 0; i < n; i++)
    {
        if (i < n - t)
            Q0.push_back(res[i][n]);
        else
            Q1.push_back(res[i][n]);
    }
    Q1.push_back(one);
    printf("q0(X)=\n");
    for (auto vec : Q0)
    {
        printf("[");
        print(vec);
        printf("]");
    }
    printf("\n");

    printf("q1(X)=\n");
    for (auto vec : Q1)
    {
        printf("[");
        print(vec);
        printf("]");
    }
    printf("\n");

    vector<vector<int>> f_hat = div_poly_matrix(Q0, Q1);
    printf("-q0(X)/q1(X)=\n");
    for (auto vec : f_hat)
    {
        printf("[");
        print(vec);
        printf("]");
    }
    printf("\n");
    return 0;
}
