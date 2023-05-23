// 張　一鳴、21B12111、ICT.E218
//協力者：楊　家寧(21B51306)、劉　仕奇(21B51387)
// プログラミング言語：C++
// コンパイル方法：g++ 21B12111-03-05.cpp -o 21B12111-03-05
// 実行方法：ターミナル上で.\21B12111-03-05を実行
/*
A(X, Y) = 
+1X^64*Y^0 +3X^41*Y^23 +3X^40*Y^24 +6X^39*Y^25
+7X^38*Y^26 +19X^37*Y^27 +16X^36*Y^28 +10X^35*Y^29
+31X^34*Y^30 +22X^33*Y^31 +21X^32*Y^32 +26X^31*Y^33 
+16X^30*Y^34 +22X^29*Y^35 +16X^28*Y^36 +12X^27*Y^37
+9X^26*Y^38 +5X^25*Y^39 +5X^24*Y^40 +2X^23*Y^41
+1X^22*Y^42 +1X^21*Y^43 +2X^20*Y^44 
B(X, Y) =
+1X^64*Y^0 +10X^62*Y^2 +142X^61*Y^3 
+2457X^60*Y^4 +29775X^59*Y^5 +292948X^58*Y^6 +2428124X^57*Y^7
+17287905X^56*Y^8 +107574778X^55*Y^9 +591704806X^54*Y^10 +2904655842X^53*Y^11 
+12828944073X^52*Y^12 +51316014465X^51*Y^13 +186936199792X^50*Y^14 +623120671160X^49*Y^15
+1908309094309X^48*Y^16 +5388164860724X^47*Y^17 +14069095209962X^46*Y^18 +34062025918670X^45*Y^19 
+76639554783421X^44*Y^20 +160578108163663X^43*Y^21 +313857223887964X^42*Y^22 +573130576781892X^41*Y^23 
+979098059911925X^40*Y^24 +1566556930278102X^39*Y^25 +2349835367096342X^38*Y^26 +3307175640083986X^37*Y^27
+4370196475467805X^36*Y^28 +5425071549308697X^35*Y^29 +6329249996351400X^34*Y^30 +6941758027899024X^33*Y^31 
+7158688099951363X^32*Y^32 +6941758156300568X^31*Y^33 +6329250020122414X^30*Y^34 +5425071456360602X^29*Y^35 
+4370196436432267X^28*Y^36 +3307175685715621X^27*Y^37 +2349835408467340X^26*Y^38 +1566556919065252X^25*Y^39        
+979098027173251X^24*Y^40 +573130573949958X^23*Y^41 +313857243687554X^22*Y^42 +160578112195542X^21*Y^43 
+76639545763835X^20*Y^44 +34062024027995X^19*Y^45 +14069098178144X^18*Y^46 +5388165442104X^17*Y^47 
+1908308443287X^16*Y^48 +623120452980X^15*Y^49 +186936283102X^14*Y^50 +51316139786X^13*Y^51 
+12828936031X^12*Y^52 +2904597053X^11*Y^53 +591707460X^10*Y^54 +107592124X^9*Y^55
+17287527X^8*Y^56 +2425194X^7*Y^57 +292738X^6*Y^58 +30006X^5*Y^59 
+2527X^4*Y^60 +139X^3*Y^61 +8X^2*Y^62
*/
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>
using namespace std;

typedef unsigned long long ull;
int n, k;
vector<string> codes;
map<int, ull> Aweights; // A重み分布
map<int, ull> Bweights; // B重み分布
map<int, ull> XplusY;   // B重み分布
map<int, ull> XminusY;  // B重み分布
const int N = 66;
ull c[N][N];

// 組合せの数を計算する
void init()
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j <= i; j++)
            if (!j)
                c[i][j] = 1;
            else
                c[i][j] = (c[i - 1][j] + c[i - 1][j - 1]);
}

// ハミング重みを計算する
int haming_weight(vector<int> &s1)
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
            swap(a[r][i], a[t][i]); // move the non-zero row to the first row
        for (int i = 0; i < k; i++) // use this row to eliminate all the ones
            if (a[i][c] && i != r)
                for (int j = n; j >= c; j--)
                    a[i][j] ^= a[r][j];

        r++;
    }
    return a;
}

//標準パリティ検査行列を作る
vector<vector<int>> createH(vector<vector<int>> &G_)
{
    int k = G_.size(), n = G_[0].size();
    vector<vector<int>> H(n - k, vector<int>(n));

    for (int i = 0; i < n - k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            H[i][j] = G_[j][k + i];
        }
    }

    for (int i = 0, j = k; i < n - k; i++, j++)
    {
        H[i][j] = 1;
    }
    return H;
}

//行列をoutput
void print_matrix(vector<vector<int>> &G)
{
    int r = G.size(), c = G[0].size();
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
            cout << G[i][j];
        cout << endl;
    }
}

//行列を転置する
vector<vector<int>> transpose(vector<vector<int>> &M)
{
    int n = M.size(), m = M[0].size();
    vector<vector<int>> M_T(m, vector<int>(n));

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
            M_T[i][j] = M[j][i];
    }
    return M_T;
}

//行列の掛け算
vector<vector<int>> multiply_matrix(vector<vector<int>> &a, vector<vector<int>> &b)
{
    int k = a.size(), n = b[0].size();
    int t = b.size();
    vector<vector<int>> res(k, vector<int>(n));

    for (int i = 0; i < k; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < t; k++)
                res[i][j] = (res[i][j] + a[i][k] * b[k][j]) % 2;

    return res;
}

//数値をベクトルに直す
vector<int> num2vector(int num, int k)
{
    vector<int> res;
    while (num != 0)
    {
        res.push_back(num % 2);
        num /= 2;
    }
    while (res.size() < k)
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
    for (int i = 0; i < k; i++)
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

    vector<vector<int>> G_ = gauss(G); // 標準形生成行列
    vector<vector<int>> H = createH(G_); //標準パリティ検査行列

    int total = pow(2, k);
    vector<vector<int>> info_vectors;

    for (int i = 0; i < 2 << k - 1; i++)
    {
        vector<int> u = num2vector(i, k);
        info_vectors.push_back(u);
    }

    // CとC^perpを生成する
    vector<vector<int>> A_codes = multiply_matrix(info_vectors, G_);
    info_vectors.clear();

    // 重み分布を計算する
    for (int i = 0; i < A_codes.size(); i++)
    {
        Aweights[haming_weight(A_codes[i])]++;
    }
    
    cout << "OUTPUT:" << endl;
    printf("A(X, Y) = \n");
    int cnt = 0;
    for (map<int, ull>::iterator it = Aweights.begin(); it != Aweights.end(); it++)
    {
        cnt++;
        printf("+%dX^%d*Y^%d ", it->second, n - it->first, it->first);
        if (cnt == 4)
        {
            printf("\n");
            cnt = 0;
        }
    }
    printf("\n");
    init();
    
    for (map<int, ull>::iterator it = Aweights.begin(); it != Aweights.end(); it++)
    {
        int xtimes = n - it->first, ytimes = it->first, coef = it->second;
        XplusY.clear();
        XminusY.clear();
        for (int i = 0; i <= xtimes; i++) // X+Yのべき乗を計算する
        {
            XplusY[i] = c[xtimes][i];
           // cout << xtimes << ' ' << i <<' ' <<  XplusY[i] << endl;
        }
        for (int i = 0; i <= ytimes; i++) //X-Yのべき乗を計算する
        {
            if (i % 2 == 0)
                XminusY[i] = c[ytimes][i]; // Yの次数が偶数
            else
                XminusY[i] = -c[ytimes][i]; // Yの次数が奇数
        }

        //(X+Y)^? (X-Y)^? を計算する
        for (map<int, ull>::iterator itPlus = XplusY.begin(); itPlus != XplusY.end(); itPlus++)
        {
            for (map<int, ull>::iterator itMinus = XminusY.begin(); itMinus != XminusY.end(); itMinus++)
            {
                Bweights[itPlus->first + itMinus->first] += (ull)coef * itPlus->second * itMinus->second;
            }
        }
    }
    // B(X,Y) * |C| を計算する
    int Csize = A_codes.size();
    for (map<int, ull>::iterator it = Bweights.begin(); it != Bweights.end(); it++)
        it->second = it->second / Csize;
    
    printf("B(X, Y) = \n");
    cnt = 0;
    for (map<int, ull>::iterator it = Bweights.begin(); it != Bweights.end(); it++)
    {
        cnt++;
        if(it->second) printf("+%lluX^%d*Y^%d ", it->second, n - it->first, it->first);
        if (cnt == 4)
        {
            printf("\n");
            cnt = 0;
        }
    }
    return 0;
}