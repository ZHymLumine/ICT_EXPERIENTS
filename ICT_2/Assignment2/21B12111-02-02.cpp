//張　一鳴、21B12111、ICT.E218
//協力者：楊　家寧(21B51306)、劉　仕奇(21B51387)
//プログラミング言語：C++
//コンパイル方法：g++ 21B12111-02-02.cpp -o 21B12111-02-02
//実行方法：ターミナル上で.\21B12111-02-02を実行
/*cw for aaa: 101000101
cw for aab: 10100011
cw for aac: 000110110
cw for aad: 01001100
cw for aba: 11111100
cw for abb: 0001110
cw for abc: 1111111
cw for abd: 0000111
cw for aca: 000110111
cw for acb: 1010010
cw for acc: 01001101
cw for acd: 0110110
cw for ada: 01000110
cw for adb: 101010
cw for adc: 0110111
cw for add: 010110
cw for baa: 11111101
cw for bab: 0001111
cw for bac: 00001100
cw for bad: 0000100
cw for bba: 0100010
cw for bbb: 10011
cw for bbc: 001000
cw for bbd: 00101
cw for bca: 00001101
cw for bcb: 001001
cw for bcc: 0000101
cw for bcd: 11100
cw for bda: 111100
cw for bdb: 00110
cw for bdc: 11101
cw for bdd: 1101
cw for caa: 101000100
cw for cab: 1010011
cw for cac: 01000111
cw for cad: 0110100
cw for cba: 00011010
cw for cbb: 010000
cw for cbc: 111101
cw for cbd: 11000
cw for cca: 01001110
cw for ccb: 101011
cw for ccc: 0110101
cw for ccd: 010111
cw for cda: 0100100
cw for cdb: 10110
cw for cdc: 011000
cw for cdd: 01110
cw for daa: 01001111
cw for dab: 111110
cw for dac: 0100101
cw for dad: 011001
cw for dba: 0001100
cw for dbb: 00111
cw for dbc: 11001
cw for dbd: 00010
cw for dca: 1010000
cw for dcb: 10111
cw for dcc: 010100
cw for dcd: 01111
cw for dda: 010101
cw for ddb: 00000
cw for ddc: 10010
cw for ddd: 1000
entropy: 1.846439
average length: 1.859000
symbols> abcdaabbccdd
encoded: 11111110100111100100001110
codewords> 11111110100111100100001110
decoded: abcdaabbccdd */
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <cmath>
#include <queue>
using namespace std;

int asize, len = 3;
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