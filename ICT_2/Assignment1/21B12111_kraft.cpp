//張　一鳴、21B12111、ICT.E218
//プログラミング言語：C++
//コンパイル方法：g++ 21B12111_kraft.cpp -o 21B12111_kraft
//実行方法：ターミナル上で.\21B12111_kraft を実行
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;

const int CODE_SIZE = 128;
const int MAX_NODES = 256;
const int BUFFER_SIZE = 1024;

struct Node
{
    int up, left, right;
    char symbol;
    int visited;
    bool leaf;
} tr[CODE_SIZE];
int root = 1, idx = 1;
vector<string> code; //符号語の集合


void make_tree(int asize, int l[])
{
    int t;
    for (int i = 0; i < asize; i++)
    {
        t = root; // poolの先頭アドレスの節点を根とする
        for (int j = 0; j < l[i]; j++)
        {
            //左の子がない場合は左右の子が無いのでそれらに新規節点を割当
            if (tr[t].left == 0)
            {
                tr[t].left = ++ idx;
                tr[t].right = ++ idx;
                tr[tr[t].left].up = t;
                tr[tr[t].right].up = t;
            }

            // i番目の符号語の割当において左の子を訪問していない場合
            if (tr[tr[t].left].visited != asize + i)
            {
                tr[tr[t].left].visited = asize + i;
                tr[tr[t].left].symbol = '0';
                t = tr[t].left;  //左の子にたどる
                if(tr[t].leaf)  //葉に行ってしまう
                {
                    t = tr[t].up;   //親に戻る
                    j--;            //歩数マイナス１
                }
                if(j == l[i] - 1) tr[t].leaf = true;
            }
            // i番目の符号語の割当において右の子を訪問していない場合
            else if (tr[tr[t].right].visited != asize + i)
            {
                tr[tr[t].right].visited = asize + i;    //訪問したとマークする
                tr[tr[t].right].symbol = '1';           //符号語を割り当てる
                t = tr[t].right;  //右の子にたどる
                if(tr[t].leaf)  //葉に行ってしまう
                {
                    t = tr[t].up;    //親に戻る
                    j--;            //歩数マイナス１
                }
                if(j == l[i] - 1) tr[t].leaf = true;    //最後の一歩で葉とマークする
            }
            //左右の子に訪問していた場合
            else
            {
                t = tr[t].up;   //親に戻る
                j -= 2;        //上の層に戻るため
            }
        }
    }
}

void build_code(int u, string c)
{
     //葉にたどり着いたらその葉が表す記号の符号語をcodeに保存する
    if(tr[u].leaf)
    {
        code.push_back(c);
        return;
    }
    //左の子があれば、
    if(tr[u].left) build_code(tr[u].left, c + tr[tr[u].left].symbol);
    //右の子があれば
    if(tr[u].right) build_code(tr[u].right, c + tr[tr[u].right].symbol);
    return;
}

int main()
{
    int asize;
    printf("alphabet size> ");
    scanf("%d", &asize);

    int l[asize];
    for (int i = 0; i < asize; i++)
    {
        printf("l_%d> ", i + 1);
        scanf("%d", &l[i]);
    }

    //符号の木の作成
    make_tree(asize, l);
    build_code(1, "");  // poolは符号の木の根のアドレスを表す
    for (int i = 0; i < code.size(); i ++)
        printf("cw for l_%d: %s\n", i + 1, code[i].c_str()); // code[i]にi番目の符号語が入っている
    return 0;
}