//張　一鳴、21B12111、ICT.E218
//プログラミング言語：C++
//コンパイル方法：g++ 21B12111-03-01.cpp -o 21B12111-03-01
//実行方法：ターミナル上で.\21B12111-03-01を実行
//521
#include <iostream>
#include <string>

using namespace std;

int main()
{
    int n;
    printf("INPUT:\n");
    printf("n=");
    cin >> n;
    string vec1, vec2;
    cout << "c0=";
    cin >> vec1;
    cout << "c1=";
    cin >> vec2;

    int cnt = 0;
    for(int i = 0; i < n; i ++)
    {
        if(vec1[i] != vec2[i]) cnt ++;
    }
    cout << "OUTPUT:" << endl;
    cout << "d=" << cnt << endl;
    
    return 0;
}