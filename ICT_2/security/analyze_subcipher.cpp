/*

　換字暗号解読プログラム

 */


#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

typedef pair<int, char> PIC;
typedef pair<char, double> PCD;
vector<PIC> hists;

map<char, double> alphabet = {
	{'A', 7.25},
	{'B', 1.5},
	{'C', 3.5},
	{'D', 4.25},
	{'E', 12.75},
	{'F', 3},
	{'G', 2},
	{'H', 3.5},
	{'I', 7.75},
	{'J', 0.25},
	{'K', 0.5},
	{'L', 3.75},
	{'M', 2.75},
	{'N', 7.75},
	{'O', 7.5},
	{'P', 2.75},
	{'Q', 0.5},
	{'R', 8.5},
	{'S', 6},
	{'T', 9.25},
	{'U', 3},
	{'V', 1.5},
	{'W', 1.5},
	{'X', 0.5},
	{'Y', 2.25},
	{'Z', 0.25},
};

bool cmp(const PCD& lhs, const PCD& rhs) {
	return lhs.second > rhs.second;
}

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

	for(i = 0; i < len; i ++)
	{
		char_hist[input[i]] ++;
	}
}

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
	int char_hist[1000];
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

	/* テキストのヒストグラムを求める */
	get_char_histogram(cipher_text, char_hist);

	printf("\n\n");
	printf("*****************************************************\n");
	printf("**********    Histogram is displayed.   ***********\n");
	printf("*****************************************************\n\n\n");



	for(i = 32; i < 128; i++)
	{
		printf("[%c, %d]\n",i, char_hist[i]);
		hists.push_back(make_pair(char_hist[i], i));
	}

	sort(hists.begin(), hists.end());
	reverse(hists.begin(), hists.end());	
	cout << "--------------------------------------------------" << endl;
	for(auto hist: hists)
	{
		cout << hist.first << ' ' << hist.second << endl;
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
		printf("[%c, %c]\n",i, inv_table[i]);
	}

	// for(i = 0; i < alphabet_vectors.size(); i++)
	// {
		
	// 	inv_table[alphabet_vectors[i].first] = hists[i].second;
	// }


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