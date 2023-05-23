//
// 
// Name        : rsa_dec.c
// Author      : Takashi Obi (obi@isl.titech.ac.jp)
// Version     : 1.01
// Description : RSAを用いた復号処理プログラム
// Note        : コンパイル時に"-lssl -lcrypto"が必要
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
  FILE *fp_rk, *fp_re, *fp_w;
  int  i;
  long rsaSize;
  unsigned char *enc, *dec;
  unsigned char privatekey[] = "private.pem";
  unsigned char encryptedData[] = "encrypted_rsa.dat";
  
  
//自身の秘密鍵を読み込む
  if( (fp_rk = fopen(privatekey, "r") ) == NULL ) {
    printf("秘密鍵オープンエラー\n");
    return 1;
  }
  private = PEM_read_RSAPrivateKey(fp_rk, NULL, NULL, NULL);
  fclose(fp_rk);

  rsaSize = RSA_size(private); 			//鍵サイズ読み出し

  enc = (char *)malloc(rsaSize);
  memset(enc, '\0', rsaSize);

  dec = (char *)malloc(rsaSize);
  memset(dec, '\0', rsaSize);

//暗号文を読み込む
  if( (fp_re = fopen(encryptedData, "rb") ) == NULL ) {
    printf("暗号文ファイルオープンエラー\n");
    return 1;
  }

  fread(enc, sizeof(unsigned char), rsaSize, fp_re);

  fclose(fp_re);

//暗号文表示（HEX）
  printf("暗号文(HEX): \n");
  for(i = 0; i<rsaSize; i++){
    printf("0x%02X ", enc[i]);
  }
  printf("\n");

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

//復号文表示（HEX）
  printf("復号文(HEX): \n");
  for(i = 0; i<rsaSize; i++){
    printf("0x%02X ", dec[i]);
  }
  printf("\n");

  RSA_free(private);

  return 0;
}

