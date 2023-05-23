/*
 * テキストファイルの頻度分布を生成するプログラム
 */

#include <stdio.h>
#include<stdlib.h>
#include <string.h>

void get_char_histogram(char *input, int *char_hist)
{

	/*
		asciiコードは32～127(96個)
	*/

	int i, j, t;
	int len;

	/*　文字列の長さ取得 */
	len = strlen(input);

	/* ヒストグラムの初期化 */
	for(i = 32; i < 128; i++)
	{
		char_hist[i] = 0;
	}

/*******　ここにヒストグラム分布を定義するコードを記述してください。　*******/
/*******　各文字の頻度は、各文字のACSIIコードをiとするとchar_hist[i]　*******/
/*******　で定義されます。for文を使えば1行で書けます。　　　　　　　　*******/
	for(i = 0; i < len; i ++)
	{
		char_hist[input[i]] ++;
	}
}


int main(void)
{

	FILE*fp;
	FILE*fo;
	char input_text[20000];
	int char_hist[1000];

	int i;

	/* 入力ファイルオープン */
	if((fp = fopen("cipher.txt", "r")) == NULL)
	{
		puts("file not open");
		exit(0);
	}
	
	if((fo = fopen("char_histgram.txt", "w")) == NULL)
	{
		puts("er");
		exit(0);
	}

	/* ファイルの読み込み・表示 */
	printf("\n\n");
	printf("*****************************************************\n");
	printf("********       Text file is displayed.     **********\n");
	printf("*****************************************************\n\n\n");

	while ( fgets(input_text, 1000, fp) != NULL) 
	{
		printf("%s",input_text);
	}

	/* テキストのヒストグラムを求める */
	get_char_histogram(input_text, char_hist);

	/* ヒストグラムの表示・書き込み */
	printf("\n\n");
	printf("*****************************************************\n");
	printf("**********    Histogram is displayed.   ***********\n");
	printf("*****************************************************\n\n\n");


	for(i = 32; i < 128; i++)
	{
		printf("[%c, %d]\n",i, char_hist[i]);
		fprintf(fo,"%d, %d\n",i, char_hist[i]);
	}


	/* 終了処理 */
	fclose(fp);
	fclose(fo);

	return 0;
}