//
// 
// Name        : rsa_encdec.c
// Author      : Takashi Obi (obi@isl.titech.ac.jp)
// Version     : 1.00
// Description : RSAを用いた暗号化、復号処理プログラム
// Note        : コンパイル時に"-lcrypto -lssl"が必要
//
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/engine.h>

int main(int argc, char *argv[])
{
  RSA *public, *private;
  FILE *fp;
  int  i;
  long rsaSize;
  unsigned char *enc, *dec;
  unsigned char planeData[] = "This is plaintext.";
  
  
//実験演習１で作成した公開鍵を読み込む
  if( (fp = fopen("public.pem", "r") ) == NULL ) {
    printf("公開鍵オープンエラー\n");
    return 1;
  }
  public = PEM_read_RSAPublicKey(fp, NULL, NULL, NULL);
  fclose(fp);

  rsaSize = RSA_size(public); 			//鍵サイズ読み出し
  enc = (char *)malloc(rsaSize);
  memset(enc, '\0', rsaSize);

//planeDataを公開鍵で暗号化。パディング方式は、PKCS #1 v1.5 パディングを指定
  if( RSA_public_encrypt(strlen(planeData), planeData, enc, public, RSA_PKCS1_PADDING) < 0 ) {
    free(enc);
    RSA_free(public);
    printf("RSA暗号化エラー\n");
    return 1;
  }
  RSA_free(public);

//平文表示（文字列）
  printf("平文: %s\n", planeData);

//平文表示（HEX）
  printf("平文(HEX): \n");
  for(i = 0; i<strlen(planeData); i++){
    printf("0x%02X ", planeData[i]);
  }
  printf("\n");

//暗号文表示（HEX）公開鍵により暗号化
  printf("暗号文(HEX): \n");
  for(i = 0; i<rsaSize; i++){
    printf("0x%02X ", enc[i]);
  }
  printf("\n");


//実験演習１で作成した秘密鍵を読み込む
  if( (fp = fopen("private.pem", "r") ) == NULL ) {
    printf("秘密鍵オープンエラー\n");
    return 1;
  }
  private = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL);
  fclose(fp);

  dec = (char *)malloc(rsaSize);
  memset(dec, '\0', rsaSize);

//暗号文を秘密鍵で復号。パディング方式は、PKCS #1 v1.5 パディングを指定
  memset(dec, '\0', rsaSize);
  if( RSA_private_decrypt(rsaSize, enc, dec, private, RSA_PKCS1_PADDING) < 0 ) {
    free(dec);
    RSA_free(private);
    printf("RSA復号エラー\n");
    return 1;
  }
//暗号文表示（文字列）公開鍵により暗号化
  printf("復号文: %s\n", dec);

//暗号文を秘密鍵で復号。パディングなしを指定
  if( RSA_private_decrypt(rsaSize, enc, dec, private, RSA_NO_PADDING) < 0 ) {
    free(dec);
    RSA_free(private);
    printf("RSA復号エラー\n");
    return 1;
  }

//暗号文表示（HEX）
  printf("復号文(HEX): \n");
  for(i = 0; i<rsaSize; i++){
    printf("0x%02X ", dec[i]);
  }
  printf("\n");

  RSA_free(private);

  return 0;
}

