//
// 
// Name        : all_speed_check.c
// Author      : Takashi Obi (obi@isl.titech.ac.jp)
// Version     : 1.10
// Description : RSA,DES,AESの実行速度比較用プログラム
// Note        : コンパイル時に"-lssl -lcrypto" が必要
//               speed_check.cの完全版
//
//
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/des.h>
#include <openssl/aes.h>
#include <openssl/engine.h>
#include <openssl/pem.h>
#include <openssl/err.h>

static void printError(char *msg, unsigned long err);

int main(int argc, char *argv[])
{
  FILE *fp;
  int  i, k;
  size_t  keylen;
  clock_t  first_time, end_time;
  double run_time;

// 平文 (32byte)
  unsigned char plainData[] = "This is plaintext for speed test";
  unsigned char *ptext;
  
// 暗号文、復号文用ポインタ
  unsigned char *enc, *dec;

// RSA用変数
  int rsaBitsize; //RSA鍵長(bit)
  long rsaSize;   //RSA鍵長(byte)
  BIGNUM *expornent = BN_new();
  BN_set_word(expornent,65537); //一般的に利用されるeの値を設定
  RSA *rsaKey;
  
//DES,AES用変数
  EVP_CIPHER_CTX  *ctx;
  ctx = EVP_CIPHER_CTX_new();
  int cipher_len, plain_len, enc_len, f_len=0;

//キー情報
  unsigned char key_16[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  unsigned char key_24[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};

// セキュリティ鍵長 80bit

  printf("\nセキュリティ鍵長 80bit\n");
 
// RSA 1024bit での暗号化、復号処理
// 鍵ペアの作成
  rsaBitsize = 1024;
  rsaKey = RSA_new();
  RSA_generate_key_ex(rsaKey, rsaBitsize, expornent, NULL);
  if (rsaKey == NULL){
      printError("failed to generate RSA keys", ERR_get_error());
      exit(-1);
  }
  rsaSize = RSA_size(rsaKey);
  
  enc = (char *)malloc(rsaSize);
  dec = (char *)malloc(rsaSize);

// 開始CPU使用時間を取得 
  first_time = clock();

//planeDataを公開鍵で暗号化。パディング方式は、PKCS #1 v1.5 パディングを指定
  for (i=0; i<100000; i++){
    if( RSA_public_encrypt(strlen(plainData), plainData, enc, rsaKey, RSA_PKCS1_PADDING) < 0 ) {
      free(enc);
      printError("RSA暗号化エラー", ERR_get_error());
      exit(-1);
    }
  }
  end_time =  clock();
  run_time = (double)(end_time - first_time)/(double)CLOCKS_PER_SEC;
  printf("RSA 1024bit 暗号化（100,000回）CPU使用時間：%3.2f 秒\n", run_time);

// 開始CPU使用時間を取得 
  first_time = clock();
  
//encを秘密鍵で復号。パディング方式は、PKCS #1 v1.5 パディングを指定
  for (i=0; i<100000; i++){
    if( RSA_private_decrypt(rsaSize, enc, dec, rsaKey, RSA_PKCS1_PADDING) < 0 ) {
      free(dec);
      printError("RSA復号エラー", ERR_get_error());
      exit(-1);
    }
  }
  end_time =  clock();
  run_time = (double)(end_time - first_time) / (double)CLOCKS_PER_SEC;
  printf("RSA 1024bit 復号（100,000回）CPU使用時間：%3.2f 秒\n", run_time);
  
// 領域の開放
   RSA_free(rsaKey);
   free(dec);
   free(enc);

// 2-key Triple DES (112bit) ECBモード利用での暗号化、復号処理

// 平文サイズを1024bitに変更
    ptext = (unsigned char *)calloc(rsaSize+1,sizeof(char));
    memset(ptext, '\0', rsaSize+1);
    k = rsaSize / 32;
    for(i=0; i<rsaSize / 32; i++){
      memcpy(ptext+32*i, plainData, 32);
    }

// 開始CPU使用時間を取得 
    first_time = clock();

// 2Key T-DES暗号化処理
  for (i=0; i<100000; i++){
    cipher_len=strlen(ptext)+EVP_MAX_BLOCK_LENGTH;   // 暗号データの長さを仮置き（Byte)
    enc=(unsigned char *)calloc(cipher_len,sizeof(char));
    EVP_CIPHER_CTX_init(ctx);
    EVP_EncryptInit_ex(ctx, EVP_des_ede(), NULL, (unsigned char *)key_16, NULL);
    EVP_EncryptUpdate(ctx, enc, &cipher_len, (unsigned char *)ptext, strlen(ptext));
    EVP_EncryptFinal_ex(ctx,(unsigned char *)(enc+cipher_len),&f_len);
    EVP_CIPHER_CTX_cleanup(ctx);
    enc_len = cipher_len+f_len;  // 出力された暗号化データの長さ（Byte)
  }
  end_time =  clock();
  run_time = (double)(end_time - first_time) / (double)CLOCKS_PER_SEC;
  printf("2Key Triple-DES暗号化（100,000回）CPU使用時間：%3.2f 秒\n", run_time);

// 開始CPU使用時間を取得 
  first_time = clock();
  
// 2Key T-DES復号処理
  for (i=0; i<100000; i++){
    plain_len=enc_len+EVP_MAX_BLOCK_LENGTH;  // 復号データの長さを仮置き（Byte)
    dec=(unsigned char *)calloc(plain_len,sizeof(char));
    EVP_CIPHER_CTX_init(ctx);
    EVP_DecryptInit_ex(ctx, EVP_des_ede(), NULL, (unsigned char*)key_16, NULL);
    EVP_DecryptUpdate(ctx, dec, &plain_len, (unsigned char *)enc, cipher_len+f_len);
    EVP_DecryptFinal_ex(ctx,(unsigned char *)(dec+plain_len),&f_len);
    EVP_CIPHER_CTX_cleanup(ctx);
  }
  end_time =  clock();
  run_time = (double)(end_time - first_time) / (double)CLOCKS_PER_SEC;
  printf("2Key Triple-DES復号（100,000回）CPU使用時間：%3.2f 秒\n", run_time);

	free(enc);
    free(dec);

// セキュリティ鍵長 112bit
  printf("\nセキュリティ鍵長 112bit\n");

// RSA 2048bit
// 鍵ペアの作成
  rsaBitsize = 2048;
  rsaKey = RSA_new();
  RSA_generate_key_ex(rsaKey, rsaBitsize, expornent, NULL);
  if (rsaKey == NULL){
      printError("failed to generate RSA keys", ERR_get_error());
      exit(-1);
  }
  rsaSize = RSA_size(rsaKey);
  
  enc = (char *)malloc(rsaSize);
  dec = (char *)malloc(rsaSize);

// 開始CPU使用時間を取得 
  first_time = clock();

//planeDataを公開鍵で暗号化。パディング方式は、PKCS #1 v1.5 パディングを指定
  for (i=0; i<100000; i++){
    if( RSA_public_encrypt(strlen(plainData), plainData, enc, rsaKey, RSA_PKCS1_PADDING) < 0 ) {
      free(enc);
      printError("RSA暗号化エラー", ERR_get_error());
      exit(-1);
    }
  }
  end_time =  clock();
  run_time = (double)(end_time - first_time)/(double)CLOCKS_PER_SEC;
  printf("RSA 2048bit 暗号化（100,000回）CPU使用時間：%3.2f 秒\n", run_time);


// 開始CPU使用時間を取得 
  first_time = clock();
  
//encを秘密鍵で復号。パディング方式は、PKCS #1 v1.5 パディングを指定
  for (i=0; i<100000; i++){
    if( RSA_private_decrypt(rsaSize, enc, dec, rsaKey, RSA_PKCS1_PADDING) < 0 ) {
      free(dec);
      printError("RSA復号エラー", ERR_get_error());
      exit(-1);
    }
  }
  end_time =  clock();
  run_time = (double)(end_time - first_time) / (double)CLOCKS_PER_SEC;
  printf("RSA 2048bit 復号（100,000回）CPU使用時間：%3.2f 秒\n", run_time);
  
// 領域の開放
   RSA_free(rsaKey);
   free(dec);
   free(enc);

// 3-key Triple DES (112bit) ECBモード利用
// 平文サイズを2048bitに変更
    ptext = (unsigned char *)calloc(rsaSize+1,sizeof(char));
    memset(ptext, '\0', rsaSize+1);
    k = rsaSize / 32;
    for(i=0; i<rsaSize / 32; i++){
      memcpy(ptext+32*i, plainData, 32);
    }

// 開始CPU使用時間を取得 
    first_time = clock();

// 3Key T-DES暗号化処理
  for (i=0; i<100000; i++){
    cipher_len=strlen(ptext)+EVP_MAX_BLOCK_LENGTH;
    enc=(unsigned char *)calloc(cipher_len,sizeof(char));
    EVP_CIPHER_CTX_init(ctx);
    EVP_EncryptInit_ex(ctx, EVP_des_ede3(), NULL, (unsigned char *)key_24, NULL);
    EVP_EncryptUpdate(ctx, enc, &cipher_len, (unsigned char *)ptext, strlen(ptext));
    EVP_EncryptFinal_ex(ctx,(unsigned char *)(enc+cipher_len),&f_len);
    EVP_CIPHER_CTX_cleanup(ctx);
    enc_len = cipher_len+f_len;  // 出力された暗号化データの長さ（Byte)
  }
  end_time =  clock();
  run_time = (double)(end_time - first_time) / (double)CLOCKS_PER_SEC;
  printf("3Key Triple-DES暗号化（100,000回）CPU使用時間：%3.2f 秒\n", run_time);

// 開始CPU使用時間を取得 
  first_time = clock();
  
// 3Key T-DES復号処理
  for (i=0; i<100000; i++){
    plain_len=enc_len+EVP_MAX_BLOCK_LENGTH;  // 復号データの長さを仮置き（Byte)
    dec=(unsigned char *)calloc(plain_len,sizeof(char));
    EVP_CIPHER_CTX_init(ctx);
    EVP_DecryptInit_ex(ctx, EVP_des_ede3(), NULL, (unsigned char*)key_24, NULL);
    EVP_DecryptUpdate(ctx, dec, &plain_len, (unsigned char *)enc, cipher_len+f_len);
    EVP_DecryptFinal_ex(ctx,(unsigned char *)(dec+plain_len),&f_len);
    EVP_CIPHER_CTX_cleanup(ctx);
  }
  end_time =  clock();
  run_time = (double)(end_time - first_time) / (double)CLOCKS_PER_SEC;
  printf("3Key Triple-DES復号（100,000回）CPU使用時間：%3.2f 秒\n", run_time);

  free(enc);
  free(dec);

// セキュリティ鍵長 128bit
  printf("\nセキュリティ鍵長 128bit\n");

// RSA 3072bit
// 鍵ペアの作成
  rsaBitsize = 3072;
  rsaKey = RSA_new();
  RSA_generate_key_ex(rsaKey, rsaBitsize, expornent, NULL); 
  if (rsaKey == NULL){
      printError("failed to generate RSA keys", ERR_get_error());
      exit(-1);
  }
  rsaSize = RSA_size(rsaKey);
  
  enc = (char *)malloc(rsaSize);
  dec = (char *)malloc(rsaSize);

// 開始CPU使用時間を取得 
  first_time = clock();

//planeDataを公開鍵で暗号化。パディング方式は、PKCS #1 v1.5 パディングを指定
  for (i=0; i<100000; i++){
    if( RSA_public_encrypt(strlen(plainData), plainData, enc, rsaKey, RSA_PKCS1_PADDING) < 0 ) {
      free(enc);
      printError("RSA暗号化エラー", ERR_get_error());
      exit(-1);
    }
  }
  end_time =  clock();
  run_time = (double)(end_time - first_time)/(double)CLOCKS_PER_SEC;
  printf("RSA 3072bit 暗号化（100,000回）CPU使用時間：%3.2f 秒\n", run_time);
  
// 開始CPU使用時間を取得 
  first_time = clock();
  
//encを秘密鍵で復号。パディング方式は、PKCS #1 v1.5 パディングを指定
  for (i=0; i<100000; i++){
    if( RSA_private_decrypt(rsaSize, enc, dec, rsaKey, RSA_PKCS1_PADDING) < 0 ) {
      free(dec);
      printError("RSA復号エラー", ERR_get_error());
      exit(-1);
    }
  }
  end_time =  clock();
  run_time = (double)(end_time - first_time) / (double)CLOCKS_PER_SEC;
  printf("RSA 3072bit 復号（100,000回）CPU使用時間：%3.2f 秒\n", run_time);
  
// 領域の開放
   RSA_free(rsaKey);
   free(dec);
   free(enc);

// AES 128bit ECBモード利用
// 平文サイズを3072bitに変更
    ptext = (unsigned char *)calloc(rsaSize+1,sizeof(char));
    memset(ptext, '\0', rsaSize+1);
    k = rsaSize / 32;
    for(i=0; i<rsaSize / 32; i++){
      memcpy(ptext+32*i, plainData, 32);
    }

// 開始CPU使用時間を取得 
  first_time = clock();
  
// AES 暗号化処理
  for (i=0; i<100000; i++){
    cipher_len=strlen(ptext)+EVP_MAX_BLOCK_LENGTH;
    enc=(unsigned char *)calloc(cipher_len,sizeof(char));
    EVP_CIPHER_CTX_init(ctx);
    EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, (unsigned char *)key_16, NULL);
    EVP_EncryptUpdate(ctx, enc, &cipher_len, (unsigned char *)ptext, strlen(ptext));
    EVP_EncryptFinal_ex(ctx,(unsigned char *)(enc+cipher_len),&f_len); 
    EVP_CIPHER_CTX_cleanup(ctx);
    enc_len = cipher_len+f_len;  // 出力された暗号化データの長さ（Byte)
  }
  end_time =  clock();
  run_time = (double)(end_time - first_time) / (double)CLOCKS_PER_SEC;
  printf("AES暗号化（100,000回）CPU使用時間：%3.2f 秒\n", run_time);

// 開始CPU使用時間を取得 
  first_time = clock();

// AES 復号処理
  for (i=0; i<100000; i++){
    plain_len=enc_len+EVP_MAX_BLOCK_LENGTH;  // 復号データの長さを仮置き（Byte)
    dec=(unsigned char *)calloc(plain_len,sizeof(char));
    EVP_CIPHER_CTX_init(ctx);
    EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, (unsigned char*)key_16, NULL);
    EVP_DecryptUpdate(ctx, dec, &plain_len, (unsigned char *)enc, cipher_len+f_len);
    EVP_DecryptFinal_ex(ctx,(unsigned char *)(dec+plain_len),&f_len);
    EVP_CIPHER_CTX_cleanup(ctx);
  }
  end_time =  clock();
  run_time = (double)(end_time - first_time) / (double)CLOCKS_PER_SEC;
  printf("AES復号（100,000回）CPU使用時間：%3.2f 秒\n", run_time);

  EVP_CIPHER_CTX_free(ctx);
  free(enc);
  free(dec);
    
  return 0;

}

//エラー表示用
static void printError(char *msg, unsigned long err)
{
  char errmsg[256];

  ERR_load_crypto_strings();
  ERR_error_string(err, errmsg);
  fprintf(stderr, "%s\n", errmsg);

}


