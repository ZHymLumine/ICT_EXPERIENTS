#include <stdio.h>
#include <string.h>

#define CODE_SIZE 128
#define MAX_NODES 256
#define BUFFER_SIZE 1024

//自己参照構造体（木構造を作成する）
typedef struct node_t
{
  struct node_t *up, *left, *right; //親と左右の子
  int visit;                        //訪問済みかどうかのフラグ
  int symbol;                       //記号の番号
  int leaf;                         //葉かどうかのフラグ
} * node;                           // node_tのポインタをnodeと名付ける

// node_t型のpoolをMAX_NODES個作製して初期化。符号の木の節点数が最大MAX_NODES
struct node_t pool[MAX_NODES] = {{0}};
int n_nodes = 1;             //符号の木の節点数
char *code[CODE_SIZE] = {0}; //符号語の集合
char buf[BUFFER_SIZE];       //符号語の書き込みに利用する記憶領域

void make_tree(int asize, int *l)
{
  node t; //符号の木
  for (int i = 0; i < asize; i++)
  {           //符号語長の短い符号語から順に葉を割り当てていく
    t = pool; // poolの先頭アドレスの節点を根とする
    for (int j = 0; j < l[i]; j++)
    {
      //左の子がない場合は左右の子が無いのでそれらに新規節点を割当
      if (t->left == 0)
      {
        t->left = pool + n_nodes++;
        t->left->up = t; //左の子の親を自分自身にする
        t->right = pool + n_nodes++;
        t->right->up = t;
      }

      // i番目の符号語の割当において左の子を訪問していない場合
      if (t->left->visit != asize + i)
      {
        // ???
        t->left->visit = asize + i;
        t = t->left;
        t->symbol = 0;
        if(t->leaf) 
        {
          t = t->up;
          j--;
          continue;
        }
        if(j == l[i] - 1) t->leaf = 1;
      }
      // i番目の符号語の割当において右の子を訪問していない場合
      else if (t->right->visit != asize + i)
      {
        // ???
        t->right->visit = asize + i;
        t = t->right;
        t->symbol = 1;
        if(t->leaf) 
        {
          t = t->up;
          j --;
          continue;
        }
        if(j == l[i] - 1) t->leaf = 1;
      }
      //左右の子に訪問していた場合
      else
      {
        // ???
        t = t->up;
        j--;
      }
    }
  }
}

void build_code(node n, char *c, int len)
{
    static char *out = buf;
    //葉にたどり着いたらその葉が表す記号の符号語をcodeに保存する
    if (n->leaf)
    {
      // ???
      //char *temp = itoa(n->symbol);
      //strcat(buf, temp);
      //strcat(out, c);
      //printf("temp%s\n", temp);
      printf("temp%d\n", n->symbol);
      // printf("leaf%s\n", out);
      //code[len++] = c;
      return;
    }
  // strcat(c, n->symbol + '0');
  printf("%d\n", n->symbol);
  printf("%d %d\n", n->left->leaf, n->right);
  if(n->left != 0) 
  {
    printf("------------");
    build_code(n->left, c, len);
    printf("#############");
  }
  if(n->right != 0) build_code(n->right, c, len);
  return;
}

int main(void)
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

  //符号語の作成
  printf("%d\n", pool);
  char c[MAX_NODES] = {0}; // 1つの符号語の長さは節点数の最大値としておく
  build_code(pool, c, 0);  // poolは符号の木の根のアドレスを表す
  printf("%d\n", l[0]);
  
  for (int i = 0; i < asize; i++)
    printf("cw for l_%d: %s\n", i + 1, code[i]); // code[i]にi番目の符号語が入っている
  return 0;
}
