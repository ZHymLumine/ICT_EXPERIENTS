//張　一鳴、21B12111、ICT.E218
//プログラミング言語：C++
//コンパイル方法：g++ 21B12111_lamp.cpp -o 21B12111_lamp
//実行方法：ターミナル上で.\21B12111_lamp を実行
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>
#include <set>

using namespace std;
typedef pair<double, int> PDI;

int main()
{
    int n;
    printf("num of things> ");
    cin >> n;
    //vector<PDI> things;
    vector<int> lamp;
    double p;
    for(int i = 0; i < n; i ++)
    {
        printf("p_%d> ", i + 1);
        cin >> p;
        //things.push_back(make_pair(p, i + 1));
        lamp.push_back(i + 1);
    }

    //sort(things.begin(), things.end());
    int l = 0, r = n - 1;
    char c;
    while(l < r)
    {
        int mid = l + r + 1 >> 1;
        printf("question: x >= %d?\n", lamp[mid]);
        printf("answer> ");
        cin >> c;
        if(c == 'y') l = mid;
        else r = mid - 1;
    }
    printf("question: x = %d?\n", lamp[r]);
    printf("answer> ");
    cin >> c;
    if(c == 'y') printf("final answer: %d\n", lamp[r]);


    return 0;
}
