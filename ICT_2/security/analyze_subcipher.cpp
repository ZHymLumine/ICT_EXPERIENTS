/*

�@�����Í���ǃv���O����

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
		ascii�R�[�h��32�`127(96��)
	*/

	int i, j, t;
	int len;

	/*�@������̒����擾 */
	len = strlen(input);

	/* �q�X�g�O�����̏����� */
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
		ascii�R�[�h��32�`127(96��)
	*/

	int i, j, t;
	int len;

	/*�@����������̒��� */
	len = strlen(input);

	/*�@�Í���/���� */
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

	/* �t�@�C���I�[�v�� */
	if ((fp_e = fopen("cipher_p.txt", "r")) == NULL) { puts("file not open"); }
	if ((fp_it = fopen("subcipher_inv_table_a_p.txt", "r")) == NULL) { puts("file not open"); }
	if ((fo_d = fopen("decipher_p.txt", "w")) == NULL) { puts("er"); }

	/* �Í����t�@�C���̓ǂݍ��݁E�\�� */
	printf("\n\n");
	printf("*****************************************************\n");
	printf("********    Ciphertext data is displayed.   *********\n");
	printf("*****************************************************\n\n\n");

	while ( fgets(cipher_text, 3000, fp_e) != NULL) 
	{
		printf("%s",cipher_text);
	}

	/* �e�L�X�g�̃q�X�g�O���������߂� */
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
	
	
	/*** �����e�[�u���̓ǂݍ��݁E�\���@***/

	printf("\n\n");
	printf("*****************************************************\n");
	printf("********    Dicipher talbe is displayed.   **********\n");
	printf("*****************************************************\n\n\n");


	/* �����e�[�u���̓ǂݍ��� */
	for(i = 32; i < 128; i++)
	{
		fscanf(fp_it,"%d, %d\n",&i, &inv_table[i]);
		printf("[%c, %c]\n",i, inv_table[i]);
	}

	// for(i = 0; i < alphabet_vectors.size(); i++)
	// {
		
	// 	inv_table[alphabet_vectors[i].first] = hists[i].second;
	// }


	/* ���� */
	shuffle_ascii(cipher_text, decipher_text, inv_table);

	/* �����t�@�C���̕\���E�������� */
	printf("\n\n");
	printf("*****************************************************\n");
	printf("*********    Decipher text is displayed.   **********\n");
	printf("*****************************************************\n\n\n");

	/* �����t�@�C���̏������� */
	printf("%s",decipher_text);
	fprintf(fo_d,"%s",decipher_text);
	fflush(fo_d);

	printf("\n\n");

	/* �I������ */
	fclose(fp_e);
	fclose(fp_it);
	fclose(fo_d);

	printf("[End of analyze_subcipher]\n\n");

	return 0;
}