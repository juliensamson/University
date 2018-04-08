#include <stdlib.h>
#include <openssl/evp.h>
#include <string.h>
 
int main(void) {
 
  unsigned char spt[1024] = "This is a original secret text";  // plaintext
  unsigned char sct[1024];  // ciphertext
  unsigned char key[EVP_MAX_KEY_LENGTH] = "KQLEHCJUFGAZNMQP";  // encryption and decryption key
  unsigned char iv[EVP_MAX_IV_LENGTH] = "initial vector";  // initialization vector
  const char cipherName[] = "RC4";
  const EVP_CIPHER * cipher;
 
  int sptLength = strlen((const char*) spt);
  int sctLength = 0;
  int tmpLength = 0;
  int res;
 
  OpenSSL_add_all_ciphers();
  /* ciphers and hashes could be loaded using OpenSSL_add_all_algorithms() */
 
  cipher = EVP_get_cipherbyname(cipherName);
  if(!cipher) {
    printf("Cipher %s not found.\n", cipherName);
    exit(1);
  }
 
  EVP_CIPHER_CTX *ctx; // context structure
  ctx = EVP_CIPHER_CTX_new();
  if(ctx == NULL) exit(2);
 
  printf("pt: %s\n", spt);
 
  /* Encryption FOR THE SECRET MESSAGE*/
  res = EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv);  // context init - set cipher, key, init vector
  if(res != 1) exit(3);
  res = EVP_EncryptUpdate(ctx,  sct, &tmpLength, spt, sptLength);  // encryption of pt
  if(res != 1) exit(4);
  sctLength += tmpLength;
  res = EVP_EncryptFinal_ex(ctx, sct + sctLength, &tmpLength);  // get the remaining ct
  if(res != 1) exit(5);
  sctLength += tmpLength;
 
  printf ("Encrypted %d characters.\n", sctLength);
 
  /* Decryption FOR THE SECRET MESSAGE*/
  res = EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv);  // context init for decryption
  if(res != 1) exit(3);
  res = EVP_DecryptUpdate(ctx, spt, &tmpLength,  sct, sctLength);  // decrypt ct
  if(res != 1) exit(4);
  sptLength += tmpLength;
  res = EVP_DecryptFinal_ex(ctx, spt + sptLength, &tmpLength);  // get the remaining plaintext
  if(res != 1) exit(5);
  sptLength += tmpLength;
 
  unsigned char kpt[1024] = "abcdefghijklmnopqrstuvwxyz0123";  // plaintext
  unsigned char kct[1024];  // ciphertext

  int kptLength = strlen((const char*) kpt);
  int kctLength = 0;
  tmpLength = 0;

  printf("pt: %s\n", kpt);

  /* Encryption FOR THE KNOWN MESSAGE*/
  res = EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv);  // context init - set cipher, key, init vector
  if(res != 1) exit(3);
  res = EVP_EncryptUpdate(ctx,  kct, &tmpLength, kpt, kptLength);  // encryption of pt
  if(res != 1) exit(4);
  kctLength += tmpLength;
  res = EVP_EncryptFinal_ex(ctx, kct + kctLength, &tmpLength);  // get the remaining ct
  if(res != 1) exit(5);
  kctLength += tmpLength;
 
  printf ("Encrypted %d characters.\n", kctLength);
 
  /* Decryption FOR THE KNOWN MESSAGE*/
  res = EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv);  // context init for decryption
  if(res != 1) exit(3);
  res = EVP_DecryptUpdate(ctx, kpt, &tmpLength,  kct, kctLength);  // decrypt ct
  if(res != 1) exit(4);
  kptLength += tmpLength;
  res = EVP_DecryptFinal_ex(ctx, kpt + kptLength, &tmpLength);  // get the remaining plaintext
  if(res != 1) exit(5);
  kptLength += tmpLength;
 
   /* Clean up */
  EVP_CIPHER_CTX_free(ctx);



 
  /* Print out the encrypted and decrypted texts.
     Ciphertext will probably not be printable! */
  printf("Secret CT (str): %s\n", sct);
  printf("Secret CT (hex): ");
  for (int i = 0; i < sctLength; i++) {
    printf("%x", sct[i]);
  } 
  printf("\nSecret PT (str): %s\n", spt);
  printf("Secret PT (hex): ");
  for (int i = 0; i < sctLength; i++) {
    printf("%x", spt[i]);
  } 


  printf("\n\nKnown CT (str): %s\n", kct);
  printf("Known CT (hex): ");
  for (int i = 0; i < kctLength; i++) {
    printf("%x", kct[i]);
  } 
  printf("\nKnown PT (str): %s\n", kpt);
  printf("Known PT (hex): ");
  for (int i = 0; i < kctLength; i++) {
    printf("%x", kpt[i]);
  } 
  printf("\n");

  //printf("CT: %s\nDT: %s\n", sct, spt);
 
  exit(0);
 }
