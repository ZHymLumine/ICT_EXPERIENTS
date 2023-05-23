// 張　一鳴、21B12111、ICT.E218
// プログラミング言語：C++
// コンパイル方法：g++ 21B12111-03-04.cpp -o 21B12111-03-04
// 実行方法：ターミナル上で.\21B12111-03-04を実行
/*
M=65536
R=0.5
d_min=5
G'=
10000000000000000110111000001000
01000000000000000001011010001001
00100000000000001011100011011011
00010000000000000111111011000011
00001000000000000000101011100111
00000100000000000100011101001001
00000010000000001001100110101101
00000001000000001010011101110011
00000000100000001101111010001101
00000000010000000011001000100111
00000000001000000011100110000100
00000000000100001001010110111010
00000000000010001001111110000011
00000000000001000110110101000101
00000000000000100000101111000100
00000000000000011001101100110011
H'=
00100011100110011000000000000000
10010100100001000100000000000000
10110001011001000010000000000000
01110010111110010001000000000000
10111010101011110000100000000000
11010101100111000000010000000000
11011101110010110000001000000000
00000111001111110000000100000000
01111010101110100000000010000000
00111101000001100000000001000000
00001011010100010000000000100000
00100001000100010000000000010000
11100110100100000000000000001000
00001010111001100000000000000100
00111001010110010000000000000010
01111111110011010000000000000001
G(H')^T=
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
G'(H')^T=
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
*/
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

int n, k;
vector<string> codes;

//行列をoutput
void print_matrix(vector<vector<int>>& G)
{
    int r = G.size(), c = G[0].size();
    for(int i = 0; i < r; i ++)
    {
        for(int j = 0; j < c; j ++)
            cout << G[i][j];
        cout << endl;
    }
}

//ハミング重みを計算する
int haming_weight(vector<int>& s1)
{
    int d = 0;
    for (int i = 0; i < n; i++)
        if (s1[i])
            d++;
    return d;
}

// gauss elimination
vector<vector<int>> gauss(vector<vector<int>> a)
{
    int c, r;
    int k = a.size(), n = a[0].size();
    for (c = 0, r = 0; c < n && r < k; c++)
    {
        int t = r;
        for (int i = r; i < k; i++) // find non-zero row
            if (a[i][c])
                t = i;

        if (!a[t][c])
            continue;

        for (int i = c; i < n; i++)
            swap(a[r][i], a[t][i]);    // move the non-zero row to the first row
        for (int i = 0; i < k; i++) // use this row to eliminate all the ones
            if (a[i][c] && i != r)
                for (int j = n; j >= c; j--)
                    a[i][j] ^= a[r][j];

        r++;
    }
    return a;
}

//標準パリティ検査行列を作る
vector<vector<int>> createH(vector<vector<int>>& G_)
{
    int k = G_.size(), n = G_[0].size();
    vector<vector<int>> H(n-k, vector<int>(n));
    for(int i = 0; i < n-k; i ++)
    {
        for(int j = 0; j < k; j++)
        {
            H[i][j] = G_[j][k+i];
        }
    }
    for(int i = 0, j = k; i < n-k; i++, j ++)
    {
        H[i][j] = 1;
    }
    return H;
}



//行列を転置する
vector<vector<int>> transpose(vector<vector<int>>& M)
{
    int n = M.size(), m = M[0].size();
    vector<vector<int>>M_T(m, vector<int>(n));

    for(int i = 0; i < m; i ++)
    {
        for(int j = 0; j < n; j ++)
            M_T[i][j] = M[j][i];
    }
    return M_T;
}

//行列の掛け算
vector<vector<int>> multiply_matrix(vector<vector<int>>& a, vector<vector<int>>& b)
{
    int k = a.size(), n = b[0].size();
    int t = b.size();
    vector<vector<int>>res(k, vector<int>(n));   

    for(int i = 0; i < k; i ++)
        for(int j = 0; j < n; j ++)
            for(int k = 0; k < t;k ++)
                res[i][j] = (res[i][j] + a[i][k] * b[k][j]) % 2;
    
    return res;
}

//数値をベクトルに直す
vector<int> num2vector(int num, int k)
{
    vector<int> res;
    while(num != 0)
    {
        res.push_back(num % 2);
        num /= 2;
    }
    while(res.size() < k)
    {
        res.push_back(0);
    }
    reverse(res.begin(), res.end());
    return res;
}


int main()
{

    cout << "n=";
    cin >> n;
    cout << "k=";
    cin >> k;
    cout << "G=" << endl;
    

    vector<string> strG;
    for(int i = 0; i < k; i ++)
    {
        string ss;
        cin >> ss;
        strG.push_back(ss);
    }
    int M = pow(2, k);
    vector<vector<int>> G(k, vector<int>(n));
    for (int i = 0; i < k; i++)
    {
        int d = 0;
        for (int j = 0; j < n; j++)
        {
            G[i][j] = strG[i][j] - '0';
        }
    }

    
    vector<vector<int>> G_ = gauss(G); //標準形生成行列
    int d_min = INT_MAX;
    // for (int i = 0; i < k; i++)
    // {
    //     int d = 0;
    //     for (int j = 0; j < n; j++)
    //     {
    //         if(G_[i][j]) d ++;
    //     }
    //     d_min = min(d_min, d);
    // }
    vector<vector<int>> H = createH(G_);

    print_matrix(H);
    cout << "creater H over" << endl;
    vector<vector<int>> info_vectors;
    
    for(int i = 0; i < 2 << k - 1; i ++)
    {
        vector<int> u = num2vector(i, k);
        info_vectors.push_back(u);
    }

    
    //CとC^perpを生成する
    vector<vector<int>> A_codes =  multiply_matrix(info_vectors, G_);
    for(int i = 0; i < A_codes.size(); i ++)
    {
        int d = haming_weight(A_codes[i]);
        if(d) d_min = min(d_min, d);
    }
    
    cout << "OUTPUT:" << endl;
    cout << "M=" << M << endl;
    cout << "R=" << (double)k/n << endl;
    cout << "d_min=" << d_min << endl;

    //output G'
    cout << "G'=" << endl;
    print_matrix(G_);
    //output H'
    cout << "H'=" << endl;
    print_matrix(H);
    
    vector<vector<int>> H_T = transpose(H);
    vector<vector<int>> GH_T = multiply_matrix(G, H_T);
    vector<vector<int>> G_H_T = multiply_matrix(G_, H_T);
    
    //output G(H')^T
    cout << "G(H')^T=" << endl;
    print_matrix(GH_T);
    //G'(H')^T
    cout << "G'(H')^T=" << endl;
    print_matrix(G_H_T);
    return 0;
}