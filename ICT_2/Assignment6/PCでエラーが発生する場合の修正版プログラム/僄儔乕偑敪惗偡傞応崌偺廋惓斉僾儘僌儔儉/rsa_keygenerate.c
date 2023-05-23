//
// 
// Name        : rsa_keygenerate.c
// Author      : Takashi Obi (obi@isl.titech.ac.jp)
// Version     : 1.10
// Description : RSAの鍵ペア生成プログラム
// Note        : コンパイル時に"-lssl -lcrypto"が必要
//
//
#include <stdio.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/pem.h>

static void printError(char *msg, unsigned long err);

int main(int argc, char *argv[])
{
  int size = 512; //鍵長(bit)の設定
  BIGNUM *expornent = BN_new();
  BN_set_word(expornent,65537); //一般的に利用されるeの値を設定
  FILE *privateKeyFile;
  FILE *publicKeyFile;

 // 鍵ペアの作成
  RSA *rsaKey;
  rsaKey = RSA_new();
  RSA_generate_key_ex(rsaKey, size, expornent, NULL);
//  RSA *rsaKey = RSA_generate_key(size, expornent, NULL, NULL);
  if (rsaKey == NULL)
    {
      printError("failed to generate RSA keys", ERR_get_error());
      exit(-1);
    }

// 作成したRSA鍵の表示
  RSA_print_fp(stdout, rsaKey, 0);

  
  // 公開鍵をPEM形式で書き出し
  if( (publicKeyFile = fopen("public.pem", "w") ) == NULL ) {
    printf("公開鍵オープンエラー\n");
    exit(-1);
  }
  if(PEM_write_RSAPublicKey(publicKeyFile, rsaKey) != 1)
    {
      printError("failed to write the public key.", ERR_get_error());
      exit(-1);
    }


  // 秘密鍵をPEM形式で書き出し
  if( (privateKeyFile = fopen("private.pem", "w") ) == NULL ) {
    printf("秘密鍵オープンエラー\n");
    return 1;
  }
  if(PEM_write_RSAPrivateKey(privateKeyFile, rsaKey,
                             NULL,
                             NULL, 0,
                             NULL, NULL) != 1)
    {
      printError("failed to write the private key.", ERR_get_error());
      exit(-1);
    }

  // 領域の開放
   RSA_free(rsaKey);

  fclose(privateKeyFile);
  fclose(publicKeyFile);

  return 0;
}

//エラー表示用
static void
printError(char *msg, unsigned long err)
{
  char *errmsg = ERR_error_string(err, NULL);
  fprintf(stderr, "%s(%s)\n",
          msg,
          errmsg);
}


