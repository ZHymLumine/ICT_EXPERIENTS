/*
 * バイナリファイルの頻度分布を生成するプログラム
 */

#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <stddef.h>     /* size_t    */
#include <ctype.h>      /* isprint() */

#define WIDTH   (16)

const void *dump(const void *addr, size_t bytes)
{
    const unsigned char *p = addr;
    char text[WIDTH+1];
    unsigned i = 0;

    while (i < bytes) {
        if ((i % WIDTH) == 0) {
            printf("%6d: ", i);

            memset(text, '\0', sizeof(text));
        }


        printf("%02x ", *p);

        text[i % WIDTH] = isprint(*p) ? *p : '.';

        p++; i++;


        if ((i % WIDTH) == 0) {
            printf(": %s\n", text);
        }
    }

    if ((i % WIDTH) != 0) {
        printf("%*s: %s\n", (WIDTH - (i % WIDTH)) * 3, " ", text);
    }

    return addr;
}

int getFSZ(FILE *fp)
{
  int size = 0;
  
  fseek( fp, 0, SEEK_END );
   size = ftell(fp);
  fseek(fp, 0, SEEK_SET );
  
   return size;
}

void get_binary_histogram(unsigned char *input, int *char_hist)
{

	int i, j, t;
	int len;

	/*　バイト列の長さ取得 */
	len = strlen(input);

	/* ヒストグラムの初期化 */
	for(i = 0; i < 256; i++)
	{
		char_hist[i] = 0;
	}

/*******　ここにヒストグラム分布を定義するコードを記述してください。　*******/
/*******　各バイト値の頻度は、各バイト値をiとするとchar_hist[i]で定 　*******/
/*******　義されます。for文を使えば1行で書けます。　　　　　　　　    *******/

}


int main(void)
{

	FILE*fp;
	FILE*fo;
	unsigned char input_binary[200000];
	int binary_hist[1000];
	int fsize;

	int i;

	/* 入力ファイルオープン */
	if((fp = fopen("crypted", "rb")) == NULL)
	{
		puts("file not open");
		exit(0);
	}
	
	if((fo = fopen("binary_histgram.txt", "w")) == NULL)
	{
		puts("er");
		exit(0);
	}

	/* 読み込みファイルのファイルサイズを求める */
	fsize = getFSZ(fp);

	/* ファイルの読み込み・表示 */
	printf("\n\n");
	printf("*****************************************************\n");
	printf("********       Binary file is dumped.      **********\n");
	printf("*****************************************************\n\n\n");

	fread(input_binary, sizeof(unsigned char), fsize, fp);	

	dump(input_binary, fsize);

	printf("\n\n");
	
	/*  バイナリファイルのヒストグラムを求める */
	get_binary_histogram(input_binary, binary_hist);

	/* ヒストグラムの表示・書き込み */
	printf("\n\n");
	printf("*****************************************************\n");
	printf("**********    Histogram is displayed.   ***********\n");
	printf("*****************************************************\n\n\n");


	for(i = 0; i < 256; i++)
	{
		printf("[%d, %d]\n",i, binary_hist[i]);
		fprintf(fo,"%d, %d\n",i, binary_hist[i]);
	}


	/* 終了処理 */
	fclose(fp);
	fclose(fo);

	return 0;
}