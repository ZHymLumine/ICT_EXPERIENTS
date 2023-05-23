//
// 
// Name        : rsa_enc.c
// Author      : Takashi Obi (obi@isl.titech.ac.jp)
// Version     : 1.00
// Description : RSAを用いた暗号化プログラム
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
  FILE *fp_r, *fp_w;
  int  i;
  long rsaSize;
  unsigned char *enc, *dec;
  unsigned char planeData[] = "This is test phrase for RSA 512.";
  unsigned char publickey[] = "public.pem";
  unsigned char encryptedData[] = "encrypted_rsa.dat";
  
  
//送信したい学生の公開鍵を読み込む
  if( (fp_r = fopen(publickey, "r") ) == NULL ) {
    printf("公開鍵オープンエラー\n");
    return 1;
  }
  public = PEM_read_RSAPublicKey(fp_r, NULL, NULL, NULL);
  fclose(fp_r);


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

//暗号文を保存
  if( (fp_w = fopen(encryptedData, "wb") ) == NULL ) {
    printf("暗号分ファイルオープンエラー\n");
    return 1;
  }

  fwrite(enc, sizeof(unsigned char), rsaSize, fp_w);

  fclose(fp_w);



  return 0;
}

