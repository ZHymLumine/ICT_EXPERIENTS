/*
 * dump結果をファイル出力するプログラム
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

const void *wt_dump(const void *addr, size_t bytes, FILE *fp)
{
    const unsigned char *p = addr;
    char text[WIDTH+1];
    unsigned i = 0;

    while (i < bytes) {
        if ((i % WIDTH) == 0) {
            fprintf(fp, "%6d: ", i);

            memset(text, '\0', sizeof(text));
        }


        fprintf(fp, "%02x ", *p);

        text[i % WIDTH] = isprint(*p) ? *p : '.';

        p++; i++;


        if ((i % WIDTH) == 0) {
            fprintf(fp, ": %s\n", text);
        }
    }

    if ((i % WIDTH) != 0) {
        fprintf(fp, "%*s: %s\n", (WIDTH - (i % WIDTH)) * 3, " ", text);
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
	
	if((fo = fopen("dump.txt", "w")) == NULL)
	{
		puts("er");
		exit(0);
	}

	/* 読み込みファイルのファイルサイズを求める */
	fsize = getFSZ(fp);

	/* ファイルの読み込み・表示 */
	printf("\n\n");
	printf("*****************************************************\n");
	printf("********        Input file is dumped.      **********\n");
	printf("*****************************************************\n\n\n");

	fread(input_binary, sizeof(unsigned char), fsize, fp);	

	dump(input_binary, fsize);

	printf("\n\n");
	
	/* ダンプのファイル書き込み */
	wt_dump(input_binary, fsize, fo);


	/* 終了処理 */
	fclose(fp);
	fclose(fo);

	return 0;
}