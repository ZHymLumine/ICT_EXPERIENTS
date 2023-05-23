/*

?@??????????v???O????

 */


#include <stdio.h>
#include<stdlib.h>
#include <string.h>

void shuffle_ascii(char *input, char *output, int *tbl)
{

	/*
		ascii?R?[?h??32?`127(96??)
	*/

	int i, j, t;
	int len;

	/*?@?????????????? */
	len = strlen(input);

	/*?@?????/???? */
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

	/* ?t?@?C???I?[?v?? */
	if ((fp_e = fopen("cipher_p.txt", "r")) == NULL) { puts("file not open"); }
	if ((fp_it = fopen("subcipher_inv_table_a_p.txt", "r")) == NULL) { puts("file not open"); }
	if ((fo_d = fopen("decipher_p.txt", "w")) == NULL) { puts("er"); }

	/* ??????t?@?C?????????E?\?? */
	printf("\n\n");
	printf("*****************************************************\n");
	printf("********    Ciphertext data is displayed.   *********\n");
	printf("*****************************************************\n\n\n");

	while ( fgets(cipher_text, 3000, fp_e) != NULL) 
	{
		printf("%s",cipher_text);
	}


	/*** ?????e?[?u?????????E?\???@***/

	printf("\n\n");
	printf("*****************************************************\n");
	printf("********    Dicipher talbe is displayed.   **********\n");
	printf("*****************************************************\n\n\n");


	/* ?????e?[?u????????? */
	for(i = 32; i < 128; i++)
	{
		fscanf(fp_it,"%d, %d\n",&i, &inv_table[i]);
		printf("[%c, %d]\n",i, inv_table[i]);
	}


	/* ???? */
	shuffle_ascii(cipher_text, decipher_text, inv_table);

	/* ?????t?@?C????\???E???????? */
	printf("\n\n");
	printf("*****************************************************\n");
	printf("*********    Decipher text is displayed.   **********\n");
	printf("*****************************************************\n\n\n");

	/* ?????t?@?C??????????? */
	printf("%s",decipher_text);
	fprintf(fo_d,"%s",decipher_text);
	fflush(fo_d);

	printf("\n\n");

	/* ?I?????? */
	fclose(fp_e);
	fclose(fp_it);
	fclose(fo_d);

	printf("[End of analyze_subcipher]\n\n");

	return 0;
}