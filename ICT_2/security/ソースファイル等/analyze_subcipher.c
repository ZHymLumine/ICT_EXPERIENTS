/*

　換字暗号解読プログラム

 */


#include <stdio.h>
#include<stdlib.h>
#include <string.h>

void shuffle_ascii(char *input, char *output, int *tbl)
{

	/*
		asciiコードは32～127(96個)
	*/

	int i, j, t;
	int len;

	/*　平文文字列の長さ */
	len = strlen(input);

	/*　暗号化/復号 */
	for(i = 0; i < len; i++)
	{
		output[i] = tbl[input[i]];
	}

	output[len] = '\0';


}

int main(void)
{

	FILE*fp_e;
	FILE*fp_it;
	FILE*fo_d;
	char cipher_text[3000];
	char decipher_text[3000];
	int inv_table[256];
	int i;

	/* ファイルオープン */
	if ((fp_e = fopen("cipher_p.txt", "r")) == NULL) { puts("file not open"); }
	if ((fp_it = fopen("subcipher_inv_table_a_p.txt", "r")) == NULL) { puts("file not open"); }
	if ((fo_d = fopen("decipher_p.txt", "w")) == NULL) { puts("er"); }

	/* 暗号化ファイルの読み込み・表示 */
	printf("\n\n");
	printf("*****************************************************\n");
	printf("********    Ciphertext data is displayed.   *********\n");
	printf("*****************************************************\n\n\n");

	while ( fgets(cipher_text, 3000, fp_e) != NULL) 
	{
		printf("%s",cipher_text);
	}


	/*** 復号テーブルの読み込み・表示　***/

	printf("\n\n");
	printf("*****************************************************\n");
	printf("********    Dicipher talbe is displayed.   **********\n");
	printf("*****************************************************\n\n\n");


	/* 復号テーブルの読み込み */
	for(i = 32; i < 128; i++)
	{
		fscanf(fp_it,"%d, %d\n",&i, &inv_table[i]);
		printf("[%c, %d]\n",i, inv_table[i]);
	}


	/* 復号 */
	shuffle_ascii(cipher_text, decipher_text, inv_table);

	/* 復号ファイルの表示・書き込み */
	printf("\n\n");
	printf("*****************************************************\n");
	printf("*********    Decipher text is displayed.   **********\n");
	printf("*****************************************************\n\n\n");

	/* 復号ファイルの書き込み */
	printf("%s",decipher_text);
	fprintf(fo_d,"%s",decipher_text);
	fflush(fo_d);

	printf("\n\n");

	/* 終了処理 */
	fclose(fp_e);
	fclose(fp_it);
	fclose(fo_d);

	printf("[End of analyze_subcipher]\n\n");

	return 0;
}