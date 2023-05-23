//張　一鳴、21B12111、ICT.E218
//協力者：楊　家寧(21B51306)、劉　仕奇(21B51387)
//プログラミング言語：C++
//コンパイル方法：g++ 21B12111-02-07.cpp -o 21B12111-02-07
//実行方法：ターミナル上で.\21B12111-02-07を実行
/*cw for aaaa: 1
cw for aaab: 011
cw for aaba: 0000
cw for aabb: 01000
cw for abaa: 0001
cw for abab: 01001
cw for abba: 01010
cw for abbb: 0011101
cw for baaa: 0010
cw for baab: 01011
cw for baba: 001100
cw for babb: 0011110
cw for bbaa: 001101
cw for bbab: 0011111
cw for bbba: 00111000
cw for bbbb: 00111001
entropy: 0.721928
average length: 0.740800
symbols> aaababaaaaaaaaaabaaabbaa
encoded: 0110001110010001101
codewords> 0110001110010001101
decoded: aaababaaaaaaaaaabaaabbaa */
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <cmath>
#include <queue>
using namespace std;

int asize, len;
vector<char> symbols;
vector<double> probs;
map<string, double> blocks; //block化した後のsymbol

struct String {
    string ss;  //symbol
    double p;  //probability

    String(string s, double fq): ss(s), p(fq) {}
    String(): ss(""), p(0) {}
};

vector<String> charset;
map<string, string> code;
map<string, string> dec_code;

struct BinaryTreeNode {
    String s;
    BinaryTreeNode* left;
    BinaryTreeNode* right;

    BinaryTreeNode(): s(), left(nullptr), right(nullptr) {}
};
 
//ノードの大小を比較
bool operator<(const BinaryTreeNode& lhs, const BinaryTreeNode& rhs)
{
    if(lhs.s.p != rhs.s.p) return lhs.s.p < rhs.s.p;
    return lhs.s.ss > rhs.s.ss;
}

//ノードの大小を比較
bool operator>(const BinaryTreeNode& lhs, const BinaryTreeNode& rhs)
{
    if(lhs.s.p != rhs.s.p) return lhs.s.p > rhs.s.p;
    return lhs.s.ss < rhs.s.ss;
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
BinaryTreeNode* huffman(vector<String>& charset)
{
    // Get the number of chars in charset
    int n = charset.size();
    // Construct priority queue (min heap) for char set
    priority_queue<BinaryTreeNode, vector<BinaryTreeNode>, greater<BinaryTreeNode>> pq;
    for (vector<String>::iterator it = charset.begin(); it != charset.end(); ++it) {
        BinaryTreeNode node;
        node.s.ss = it->ss; 
        node.s.p = it->p;
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
        z.s.p = x->s.p + y->s.p;
        pq.push(z);
    }
    return get_min(pq);
}

//エントロピーを計算する
double H() {
  double h = 0;
  for(map<string, double>::iterator it = blocks.begin(); it != blocks.end(); it ++) 
  {
    h += -it->second * log2(it->second);
  }
  return h;
}

//平均符号長を計算する
double alength() {
  double al = 0;
  for(map<string, double>::iterator it = blocks.begin(); it != blocks.end(); it ++) 
    al += it->second * code[it->first].size();
  return al;
} 

//各symbolに対して符号語を割り当てる
void build_code(const BinaryTreeNode* root, string last)
{
    if(root)
    {
        if(root->s.ss != "") 
        {
            code[root->s.ss] = last; 
            dec_code[last] = root->s.ss;
        }
        if(root->left) build_code(root->left, last + '0');
        if(root->right) build_code(root->right, last + '1');
    }
    return;
}

//符号化
string encode(string ss)
{
    string out = "", last = "";
    for(auto s:ss)
    {
        last += s;
        if(code.find(last) != code.end())
        {
            out += code[last];
            last = "";
        }
    }
    return out;
}

//復号
string decode(string ss)
{
    string out = "", last = "";
    for(auto s:ss)
    {
        last += s;
        if(dec_code.find(last) != dec_code.end())
        {
            out += dec_code[last];
            last = "";
        }
    }
    return out;
}

//u:current length, last:current code, prob:current probability
void dfs(int u, string last, double prob)
{
    if(u == len)
    {
        blocks[last] = prob;
        String t(last, prob);
        charset.push_back(t);
        return;
    }

    for(int i = 0; i < symbols.size(); i ++)
    {
        dfs(u + 1, last + symbols[i], prob * probs[i]);
    }
}


int main()
{
    printf("alphabet size> ");
    cin >> asize;

    printf("length> ");
    cin >> len;

    char symb;
    double p;
    for(int i = 0; i < asize; i ++)
    {
        
        printf("symbol_%d: ", i + 1);
        cin >> symb;
        printf("p_%d: ", i + 1);
        cin >> p;
        symbols.push_back(symb);
        probs.push_back(p);
    }

    dfs(0, "", 1);

    //ハフマン木の作成
    BinaryTreeNode* root = huffman(charset);

    //codewords
    build_code(root, "");   

    for(map<string, string>::iterator it = code.begin(); it != code.end(); it ++)
        printf("cw for %s: %s\n", (it->first).c_str(), (it->second).c_str());

    //entropy
    printf("entropy: %lf\n", H() / len);

    //average length
    printf("average length: %lf\n", alength() / len);
    

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