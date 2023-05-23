// 張　一鳴、21B12111、ICT.E218
//協力者：楊　家寧(21B51306)、劉　仕奇(21B51387)
// プログラミング言語：C++
// コンパイル方法：g++ 21B12111-04-02.cpp -o 21B12111-04-02
// 実行方法：ターミナル上で.\21B12111-04-02を実行
/*結果
OUTPUT:
x=
5
0
4
2
5
0
5
6
1
6
*/
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;
int n, p;

// 足し算を計算する
vector<vector<int>> calc_add(int p)
{
    vector<vector<int>> add(p, vector<int>(p));
    for (int i = 0; i < p; i++)
        for (int j = 0; j < p; j++)
            add[i][j] = (i + j) % p;
    return add;
}

// 足し算を計算する
vector<vector<int>> calc_mul(int p)
{
    vector<vector<int>> mul(p, vector<int>(p));
    for (int i = 0; i < p; i++)
        for (int j = 0; j < p; j++)
            mul[i][j] = (i * j) % p;
    return mul;
}

// 足し算を計算する
vector<vector<int>> calc_sub(int p)
{
    vector<vector<int>> sub(p, vector<int>(p));
    for (int i = 0; i < p; i++)
        for (int j = 0; j < p; j++)
            sub[i][j] = (i - j + p) % p;
    return sub;
}

// 足し算を計算する
vector<vector<int>> calc_div(int p, vector<vector<int>> &mul)
{
    vector<vector<int>> div(p, vector<int>(p));
    for (int i = 0; i < p; i++)
        for (int j = 0; j < p; j++)
        {
            if (j == 0)
                continue;
            if (i == 0)
                div[i][j] = 0;
            else
            {
                int mul_inv;
                for (int k = 0; k < p; k++)
                {
                    if (mul[j][k] == 1)
                    {
                        mul_inv = k;
                        break;
                    }
                }
                div[i][j] = (i * mul_inv) % p;
            }
        }
    return div;
}

// gauss elimination
vector<vector<int>> gauss(vector<vector<int>> &a, vector<vector<int>> &mul, vector<vector<int>> &sub, vector<vector<int>> &div)
{
    int c, r;
    for (c = 0, r = 0; c < n; c++)
    {
        cout << c << ' ' << r << endl;
        int t = r;
        for (int i = r; i < n; i++) // find non-zero row
            if (a[i][c] > a[t][c])
                t = i;

        if (!a[t][c])
            continue;

        for (int i = c; i <= n; i++)
            swap(a[r][i], a[t][i]); // move the non-zero row to the first row

        for (int i = n; i >= c; i--)
        {
            int divide = a[r][c];
            int t = a[r][i];
            a[r][i] = div[t][divide];
        }

        for (int i = 0; i < n; i++)
        {
            if (a[i][c] && i != r)
                for (int j = n; j >= c; j--)
                {
                    int subtraction = mul[a[r][j]][a[i][c]];
                    a[i][j] = sub[a[i][j]][subtraction];
                    // a[i][j] -= a[r][j] * a[i][c];
                }
        }
        r++;
    }

    // 後退代入
    for (int i = n - 1; i >= 0; i--)
        for (int j = i + 1; j < n; j++)
        {
            int substraction = mul[a[i][j]][a[j][n]];
            a[i][n] = sub[a[i][n]][substraction];
            // a[i][n] -= a[i][j] * a[j][n];
        }

    return a;
}

int main()
{
    cout << "INPUT:" << endl;
    cout << "p=";
    cin >> p;
    cout << "n=";
    cin >> n;
    cout << "A=" << endl;

    vector<vector<int>> A(n + 1, vector<int>(n + 1));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> A[i][j];

    cout << "b=" << endl;
    for (int i = 0; i < n; i++)
        cin >> A[i][n];
    // string str;
    // cin >> str;
    // for (int i = 2; i < str.size(); i++)
    // {
    //     p = p * 10 + str[i] - '0';
    // }
    // cin >> str;
    // for (int i = 2; i < str.size(); i++)
    // {
    //     n = n * 10 + str[i] - '0';
    // }
    // cin >> str;
    // vector<vector<int>> A(n + 1, vector<int>(n + 1));
    // for (int i = 0; i < n; i++)
    // {
    //     cin >> str;
    //     for (int j = 0; j < str.size(); j++)
    //         A[i][j] = str[j] - '0';
    // }
    // cin >> str;
    // for (int i = 0; i < n; i++)
    // {
    //     cin >> A[i][n];
    // }

    vector<vector<int>> add = calc_add(p);
    vector<vector<int>> mul = calc_mul(p);
    vector<vector<int>> sub = calc_sub(p);
    vector<vector<int>> div = calc_div(p, mul);
    vector<vector<int>> res = gauss(A, mul, sub, div);

    printf("OUTPUT:\n");
    printf("x=\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\n", res[i][n]);
    }
    return 0;
}