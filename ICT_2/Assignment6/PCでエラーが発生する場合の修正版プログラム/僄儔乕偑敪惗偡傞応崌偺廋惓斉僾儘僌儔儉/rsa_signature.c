//
// 
// Name        : rsa_signature.c
// Author      : Takashi Obi (obi@isl.titech.ac.jp)
// Version     : 1.01
// Description : RSAを用いた署名、署名検証用プログラム
// Note        : コンパイル時に"-lssl -lcrypto"が必要
//
//
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/pem.h>

int main ( int argc, char *argv[] )
{
	FILE *fp; // PEMファイル読み込みしに使用するファイルポインタ
	RSA *private; // 私密鍵
	RSA *public;  // 公開鍵
	int i;
	
	unsigned char plaintext[] = "This is plaintext for the RSA digital signature.";
	unsigned char hash[SHA256_DIGEST_LENGTH]; // 署名対象．平文のSHA-256

	unsigned char *signature; // 署名情報
	unsigned int  signatureLength; // 署名情報のサイズ
	unsigned char *dec; // 署名情報
	int sigret; // sign，verify の返り値を格納する

// 平文のハッシュ値をとり、署名対象データを作る
  SHA256( (unsigned char *)plaintext, strlen( plaintext ), hash );
 
  printf("SHA256 digest: \n ");
  for(i = 0; i<SHA256_DIGEST_LENGTH; i++)
    printf("0x%02X ", (unsigned int)hash[i]);
  printf("\n");

//実験演習１で作成した秘密鍵を読み込み、署名に利用
  if( (fp = fopen("private.pem", "r") ) == NULL ) {
    printf("秘密鍵オープンエラー\n");
    return 1;
  }
  private = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL);
  fclose(fp);

  signatureLength = RSA_size(private); 			//鍵サイズ読み出し
  signature = (char *)malloc(signatureLength);
  memset(signature, '\0', signatureLength);

// ハッシュ値に署名する
  sigret = RSA_sign( NID_sha256, hash, SHA256_DIGEST_LENGTH, signature, &signatureLength, private );
  if (sigret == 1) 
    printf("署名成功\n");

  printf("署名文: \n ");
  for(i = 0; i<signatureLength; i++)
    printf("0x%02X ", (unsigned int)signature[i]);
  printf("\n");
  
//実験演習１で作成した公開鍵を読み込む
  if( (fp = fopen("public.pem", "r") ) == NULL ) {
    printf("公開鍵オープンエラー\n");
    return 1;
  }
  public = PEM_read_RSAPublicKey(fp, NULL, NULL, NULL);
  fclose(fp);

  dec = (char *)malloc(signatureLength);
  memset(dec, '\0', signatureLength);

//署名データを公開鍵で復号。パディング方式は、PKCS #1 v1.5 パディングを指定
  if( RSA_public_decrypt(signatureLength, signature, dec, public, RSA_PKCS1_PADDING) < 0 ) {
    free(dec);
    RSA_free(private);
    printf("RSA復号エラー\n");
    return 1;
  }
  RSA_free(private);

//署名の復号結果表示（HEX)
  printf("復号文(HEX): \n ");
  for(i = 0; i<signatureLength; i++)
    if( dec[i] != '\0' )
      printf("0x%02X ", (unsigned int)dec[i]);
  printf("\n");

// 署名の検証を行う
  sigret = RSA_verify( NID_sha256, hash, SHA256_DIGEST_LENGTH, signature, signatureLength, public );
  if (sigret == 1) 
    printf("署名検証成功\n");

  return 0;
}
