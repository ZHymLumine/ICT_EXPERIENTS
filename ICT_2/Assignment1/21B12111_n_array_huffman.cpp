//張　一鳴、21B12111、ICT.E218
//プログラミング言語：C++
//コンパイル方法：g++ g++ 21B12111_n_array_huffman.cpp -o 21B12111_n_array_huffman
//実行方法：ターミナル上で.\21B12111_n_array_huffman を実行
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>
#include <set>

using namespace std;
struct Char {
    char ch;  //symbol
    double p;  //probability

    Char(char c, double fq): ch(c), p(fq) {}
    Char(): ch(0), p(0) {}
};

//vector<string> code;
vector<Char> charset;
set<char> original_symbol;
map<char, string> code;
map<string, char> dec_code;
int q = 3;  //ｑ元ハフマン符号

struct TernaryTreeNode {
    Char c;
    TernaryTreeNode* left;
    TernaryTreeNode* mid;
    TernaryTreeNode* right;

    TernaryTreeNode(): c(), left(nullptr), mid(nullptr), right(nullptr) {}
};

//ノードの大小を比較
bool operator<(const TernaryTreeNode& lhs, const TernaryTreeNode& rhs)
{
    if(lhs.c.p != rhs.c.p) return lhs.c.p < rhs.c.p;
    return lhs.c.ch > rhs.c.ch;
}

//ノードの大小を比較
bool operator>(const TernaryTreeNode& lhs, const TernaryTreeNode& rhs)
{
    if(lhs.c.p != rhs.c.p) return lhs.c.p > rhs.c.p;
    return lhs.c.ch < rhs.c.ch;
}

//ヒープから一番小さい元素を取り出す　
TernaryTreeNode* get_min(priority_queue<TernaryTreeNode, vector<TernaryTreeNode>, greater<TernaryTreeNode>>& pq)
{
    if (pq.empty()) {
        cout << "Empty priority queue!" << endl;
        return nullptr;
    }

    TernaryTreeNode* pnode = new TernaryTreeNode(pq.top());
    pq.pop();
    return pnode;
}

//ハフマン木の作成 根のポインターを返す
TernaryTreeNode* huffman(vector<Char>& charset)
{
    // Get the number of chars in charset
    int n = charset.size();
    // Construct priority queue (min heap) for char set
    priority_queue<TernaryTreeNode, vector<TernaryTreeNode>, greater<TernaryTreeNode>> pq;
    for (vector<Char>::iterator it = charset.begin(); it != charset.end(); ++it) {
        TernaryTreeNode node;
        node.c.ch = it->ch; 
        node.c.p = it->p;
        pq.push(node);
    }
    // Get non-leaf and leaf nodes:
    // get the lowest two frequencies objects, then merge them, and insert the merged object into pq
    for (int i = n; i > 1; i -= 2) {
        TernaryTreeNode z;
        TernaryTreeNode* x = get_min(pq);
        TernaryTreeNode* y = get_min(pq);
        TernaryTreeNode* w = get_min(pq);
        z.left = w;
        z.mid = y;
        z.right = x;
        z.c.p = x->c.p + y->c.p + w->c.p;  //確率を足す
        // cout << x->c.ch << ' ' << y->c.ch << ' ' << w->c.ch << endl;
        // cout << x->c.p << ' ' << y->c.p << ' ' << w->c.p << ' ' << z.c.p << endl;
        pq.push(z);
    }
    TernaryTreeNode* pnode = new TernaryTreeNode(pq.top());
    return get_min(pq); //根を返す
}

//各symbolに対して符号語を割り当てる
void build_code(const TernaryTreeNode* root, string c)
{
    if(root)
    {
        if(original_symbol.find(root->c.ch) != original_symbol.end()) 
        {
            code[root->c.ch] = c; 
            dec_code[c] = root->c.ch;
        }
        if(root->left) build_code(root->left, c + '0');
        if(root->mid) build_code(root->mid, c + '1');
        if(root->right) build_code(root->right, c + '2');
    }
    return;
}

int main(void)
{
    int asize;
    printf("alphabet size> ");
    scanf("%d", &asize);
    double prob[asize];
    char symb[asize];

    for(int i = 0; i < asize; i++) {
        printf("symbol_%d> ", i + 1);
        scanf("%s", &symb[i]);
        printf("p_%d> ", i + 1);
        scanf("%lf", &prob[i]);
        Char t(symb[i], prob[i]);
        charset.push_back(t);
        original_symbol.insert(symb[i]);
    }

    int N = asize;
    //記号数が(q-1)m + 1を満たさない場合、確率０の記号を追加する
    while((N % (q-1)) != 1)
    {
        Char t(0, 0);
        charset.push_back(t);
        N ++;
    }

    //ハフマン木の作成
    TernaryTreeNode* root = huffman(charset);

    //codewords
    build_code(root, "");   
    for (int i = 0; i < asize; i++)
        printf("cw for %c: %s\n", symb[i], code[symb[i]].c_str());

    return 0;
}
