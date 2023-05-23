#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

int main()
{
    char *out = buf;
    char b[10] = {"12"};
    char *c = "12";
    code[0] = c;
    char *temp = itoa(1);
    strcat(out, temp);
    //buf = out;
    printf("%s\n", out);
    printf("%s\n", code[0]);
    return 0;
}