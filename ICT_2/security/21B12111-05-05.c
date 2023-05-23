/*
 * dump結果をファイル出力するプログラム
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h> /* size_t    */
#include <ctype.h>  /* isprint() */

#define WIDTH (16)

const void *dump(const void *addr, size_t bytes)
{
    const unsigned char *p = addr;
    char text[WIDTH + 1];
    unsigned i = 0;

    while (i < bytes)
    {
        if ((i % WIDTH) == 0)
        {
            printf("%6d: ", i);

            memset(text, '\0', sizeof(text));
        }

        printf("%02x ", *p);

        text[i % WIDTH] = isprint(*p) ? *p : '.';

        p++;
        i++;

        if ((i % WIDTH) == 0)
        {
            printf(": %s\n", text);
        }
    }

    if ((i % WIDTH) != 0)
    {
        printf("%*s: %s\n", (WIDTH - (i % WIDTH)) * 3, " ", text);
    }

    return addr;
}

void file_xor(const void *addr1, const void *addr2, size_t bytes, unsigned char *out_binary)
{
    const unsigned char *p1 = addr1;
    const unsigned char *p2 = addr2;

    unsigned i = 0;
    while(i < bytes)
    {
        out_binary[i] = (*p1) ^ (*p2);

        p1 ++;
        p2 ++;
        i ++;
    }
}

const void *wt_dump(const void *addr, size_t bytes, FILE *fp)
{
    const unsigned char *p = addr;
    char text[WIDTH + 1];
    unsigned i = 0;

    while (i < bytes)
    {
        if ((i % WIDTH) == 0)
        {
            fprintf(fp, "%6d: ", i);

            memset(text, '\0', sizeof(text));
        }

        fprintf(fp, "%02x ", *p);

        text[i % WIDTH] = isprint(*p) ? *p : '.';

        p++;
        i++;

        if ((i % WIDTH) == 0)
        {
            fprintf(fp, ": %s\n", text);
        }
    }

    if ((i % WIDTH) != 0)
    {
        fprintf(fp, "%*s: %s\n", (WIDTH - (i % WIDTH)) * 3, " ", text);
    }

    return addr;
}

int getFSZ(FILE *fp)
{
    int size = 0;

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return size;
}

int main(void)
{

    FILE *fp1;
    FILE *fp2;
    FILE *fo;
    unsigned char input_binary1[200000];
    unsigned char input_binary2[200000];
    unsigned char out_binary[200000];
    int fsize;

    int i;

    /* 入力ファイルオープン */
    if ((fp1 = fopen("cipher.txt", "rb")) == NULL)
    {
        puts("file not open");
        exit(0);
    }
    if ((fp2 = fopen("cipher1.txt", "rb")) == NULL)
    {
        puts("file not open");
        exit(0);
    }
    if ((fo = fopen("dump_compare_sub.txt", "w")) == NULL)
    {
        puts("er");
        exit(0);
    }

    /* 読み込みファイルのファイルサイズを求める */
    fsize = getFSZ(fp1);
    int fsize2 = getFSZ(fp2);
    printf("size1=%d, size2=%d\n", fsize, fsize2);
    if(fsize == fsize2) printf("YES\n");
    /* ファイルの読み込み・表示 */
    printf("\n\n");
    printf("*****************************************************\n");
    printf("********        Input file is dumped.      **********\n");
    printf("*****************************************************\n\n\n");

    fread(input_binary1, sizeof(unsigned char), fsize, fp1);
    fread(input_binary2, sizeof(unsigned char), fsize, fp2);

    file_xor(input_binary1, input_binary2, fsize, out_binary);

    dump(out_binary, fsize);

    printf("\n\n");

    /* ダンプのファイル書き込み */
    wt_dump(out_binary, fsize, fo);

    /* 終了処理 */
    fclose(fp1);
    fclose(fp2);
    fclose(fo);

    return 0;
}