// 張　一鳴、21B12111、ICT.E218
// プログラミング言語：C++
// コンパイル方法：g++ 21B12111-04-01.cpp -o 21B12111-04-01
// 実行方法：ターミナル上で.\21B12111-04-01を実行
/*結果
OUTPUT:
ADD
  | 0 1 2 3 4 5 6
--+-----------------------
0 | 0 1 2 3 4 5 6
1 | 1 2 3 4 5 6 0
2 | 2 3 4 5 6 0 1
3 | 3 4 5 6 0 1 2
4 | 4 5 6 0 1 2 3
5 | 5 6 0 1 2 3 4
6 | 6 0 1 2 3 4 5
MUL
  | 0 1 2 3 4 5 6
--+-----------------------
0 | 0 0 0 0 0 0 0
1 | 0 1 2 3 4 5 6
2 | 0 2 4 6 1 3 5
3 | 0 3 6 2 5 1 4
4 | 0 4 1 5 2 6 3
5 | 0 5 3 1 6 4 2
6 | 0 6 5 4 3 2 1
SUB
  | 0 1 2 3 4 5 6
--+-----------------------
0 | 0 6 5 4 3 2 1
1 | 1 0 6 5 4 3 2
2 | 2 1 0 6 5 4 3
3 | 3 2 1 0 6 5 4
4 | 4 3 2 1 0 6 5
5 | 5 4 3 2 1 0 6
6 | 6 5 4 3 2 1 0
DIV
  | 0 1 2 3 4 5 6
--+--------------------
0 | - 0 0 0 0 0 0
1 | - 1 4 5 2 3 6
2 | - 2 1 3 4 6 5
3 | - 3 5 1 6 2 4
4 | - 4 2 6 1 5 3
5 | - 5 6 4 3 1 2
6 | - 6 3 2 5 4 1
*/

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;
int p;

// 足し算を計算する
vector<vector<int>> calc_add(int p)
{
    vector<vector<int>> add(p, vector<int>(p));
    for (int i = 0; i < p; i++)
        for (int j = 0; j < p; j++)
            add[i][j] = (i + j) % p;
    return add;
}

// 掛け算を計算する
vector<vector<int>> calc_mul(int p)
{
    vector<vector<int>> mul(p, vector<int>(p));
    for (int i = 0; i < p; i++)
        for (int j = 0; j < p; j++)
            mul[i][j] = (i * j) % p;
    return mul;
}

// 引き算を計算する
vector<vector<int>> calc_sub(int p)
{
    vector<vector<int>> sub(p, vector<int>(p));
    for (int i = 0; i < p; i++)
        for (int j = 0; j < p; j++)
            sub[i][j] = (i - j + p) % p;
    return sub;
}

// 割り算を計算する
vector<vector<int>> calc_div(int p, vector<vector<int>>& mul)
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

void print_result(vector<vector<int>> &vec)
{
    for (int i = 0; i < p; i++)
    {
        printf(" %d", i);
    }
    printf("\n");
    printf("--+---------------\n");
    for (int i = 0; i < p; i++)
    {
        printf("%d | ", i);
        for (int j = 0; j < p; j++)
        {
            cout << vec[i][j] << ' ';
        }
        printf("\n");
    }
}

int main()
{
    cout << "INPUT:" << endl;
    cout << "p=";
    cin >> p;

    vector<vector<int>> add = calc_add(p);
    vector<vector<int>> mul = calc_mul(p);
    vector<vector<int>> sub = calc_sub(p);
    vector<vector<int>> div = calc_div(p, mul);

    printf("OUTPUT:\n");
    printf("ADD\n");
    printf("  |");
    print_result(add);

    // MUL
    printf("MUL\n");
    printf("  |");
    print_result(mul);

    // SUB
    printf("SUB\n");
    printf("  |");
    print_result(sub);

    // DIV
    printf("DIV\n");
    printf("  |");
    for (int i = 0; i < p; i++)
    {
        printf(" %d", i);
    }
    printf("\n");
    printf("--+---------------\n");
    for (int i = 0; i < p; i++)
    {
        printf("%d | ", i);
        for (int j = 0; j < p; j++)
        {   
            if(j == 0) cout << "- ";
            else cout << div[i][j] << ' ';
        }
        printf("\n");
    }
    return 0;
}