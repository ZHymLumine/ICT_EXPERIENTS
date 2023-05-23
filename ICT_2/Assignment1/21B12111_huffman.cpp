//張　一鳴、21B12111、ICT.E218
//プログラミング言語：C++
//コンパイル方法：g++ 21B12111_huffman.cpp -o 21B12111_huffman
//実行方法：ターミナル上で.\21B12111_huffman を実行
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>

using namespace std;
struct Char {
    char ch;  //symbol
    double p;  //probability

    Char(char c, double fq): ch(c), p(fq) {}
    Char(): ch(0), p(0) {}
};

//vector<string> code;
vector<Char> charset;
map<char, string> code;
map<string, char> dec_code;

struct BinaryTreeNode {
    Char c;
    BinaryTreeNode* left;
    BinaryTreeNode* right;

    BinaryTreeNode(): c(), left(nullptr), right(nullptr) {}
};
 
//ノードの大小を比較
bool operator<(const BinaryTreeNode& lhs, const BinaryTreeNode& rhs)
{
    if(lhs.c.p != rhs.c.p) return lhs.c.p < rhs.c.p;
    return lhs.c.ch > rhs.c.ch;
}

//ノードの大小を比較
bool operator>(const BinaryTreeNode& lhs, const BinaryTreeNode& rhs)
{
    if(lhs.c.p != rhs.c.p) return lhs.c.p > rhs.c.p;
    return lhs.c.ch < rhs.c.ch;
}

//ヒープから一番小さい元素を取り出す　
BinaryTreeNode* get_min(priority_queue<BinaryTreeNode, vector<BinaryTreeNode>, greater<BinaryTreeNode>>& pq)
{
    if (pq.empty()) {
        cout << "Empty priority queue!" << endl;
        return nullptr;
    }

    BinaryTreeNode* pnode = new BinaryTreeNode(pq.top());
    pq.pop();
    return pnode;
}

//ハフマン木の作成 根のポインターを返す
BinaryTreeNode* huffman(vector<Char>& charset)
{
    // Get the number of chars in charset
    int n = charset.size();
    // Construct priority queue (min heap) for char set
    priority_queue<BinaryTreeNode, vector<BinaryTreeNode>, greater<BinaryTreeNode>> pq;
    for (vector<Char>::iterator it = charset.begin(); it != charset.end(); ++it) {
        BinaryTreeNode node;
        node.c.ch = it->ch; 
        node.c.p = it->p;
        pq.push(node);
    }
    // Get non-leaf and leaf nodes:
    // get the lowest two frequencies objects, then merge them, and insert the merged object into pq
    for (int i = 0; i < n-1; i++) {
        BinaryTreeNode z;
        BinaryTreeNode* x = get_min(pq);
        BinaryTreeNode* y = get_min(pq);
        z.left = y;
        z.right = x;
        z.c.p = x->c.p + y->c.p;
        pq.push(z);
    }
    return get_min(pq);
}

//エントロピーを計算する
double H(int asize, double *prob) {
  double h = 0;
  for(int i = 0; i < asize; i ++) h += -prob[i] * log2(prob[i]);
  return h;
}

//平均符号長を計算する
double alength(int asize, double *prob, char *symb) {
  double al = 0;
  for(int i = 0; i < asize; i ++) al += prob[i] * code[symb[i]].size();
  return al;
} 

//各symbolに対して符号語を割り当てる
void build_code(const BinaryTreeNode* root, string c)
{
    if(root)
    {
        if(root->c.ch != 0) 
        {
            code[root->c.ch] = c; 
            dec_code[c] = root->c.ch;
        }
        if(root->left) build_code(root->left, c + '0');
        if(root->right) build_code(root->right, c + '1');
    }
    return;
}

//符号化
string encode(string ss)
{
    string out = "";
    for(auto c:ss)
        out += code[c];
    return out;
}

//復号
string decode(string ss)
{
    string out = "", last = "";
    for(auto c:ss)
    {
        last += c;
        if(dec_code.find(last) != dec_code.end())
        {
            out += dec_code[last];
            last = "";
        }
    }
    return out;
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
    }
    //ハフマン木の作成
    BinaryTreeNode* root = huffman(charset);

    //codewords
    build_code(root, "");   
    for (int i = 0; i < asize; i++)
        printf("cw for %c: %s\n", symb[i], code[symb[i]].c_str());

    //entropy
    printf("entropy: %lf\n", H(asize, prob));

    //average length
    printf("average length: %lf\n", alength(asize, prob, symb));
    

    //encoded
    string ss;
    printf("symbols> ");
    cin >> ss;
    string out = encode(ss);
    printf("encoded: %s\n", out.c_str());

    //decoded
    string cdws;
    cout << "codewords> ";
    cin >> cdws;
    string dec = decode(cdws);
    cout << "decoded: " << dec << endl;
    return 0;
}
