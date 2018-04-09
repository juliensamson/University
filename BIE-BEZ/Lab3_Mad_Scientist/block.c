/* AUTHOR: JULIEN SAMSON */
/*  READ BMP FILE:
    https://stackoverflow.com/questions/9296059/read-pixel-value-in-bmp-file#9296467
*/

#include <stdlib.h>
#include <openssl/evp.h>
#include <string.h>


unsigned char* readBMPFile(char* filename) {
  FILE file = fopen(filename, "rb");

  if (f == NULL)
    throw "Argument Exception";
  unsigned char fileInfo[54];
  fread(fileInfo, sizeof(unsigned char), 54, f);

  // Extract image height and width
  int width = *(int*)&fileInfo[18];
  int height = *(int*)&fileInfo[22];

  int size = 3 * width * height;
  //int row_padded = (width * 3 + 3) & (~3);
  unsigned char* data = new unsigned char [size]; 
  fread(data, sizeof(unsigned char), size, f);
  fclose(f);

  //BECAUSE WINDOW (SO MIGHt not be necessary)
  for(int i = 0; i < size; i+=3) {
    unsigned char temp = data[i];
    data[i] = data[i+2];
    data[i+2] = temp;
  }
  return data;
}
 
int main(void) {
 
  unsigned char pt[1024];
  unsigned char ct[1024];
  char fileName[40] = "Mad_scientist.bmp";
  unsigned char key[EVP_MAX_KEY_LENGTH] = "AHCMQLOY7JDOQHY1";  // encryption and decryption key
  unsigned char iv[EVP_MAX_IV_LENGTH] = "initial vector";  // initialization vector
  const char cipherName[] = "DESECB";
  const EVP_CIPHER * cipher;
 
  int ptLength = strlen((const char*) pt);
  int ctLength = 0;
  int tmpLength = 0;
  int res;
 
  OpenSSL_add_all_ciphers();
  /* ciphers and hashes could be loaded using OpenSSL_add_all_algorithms() */
 
  /* OPEN THE FILE AND CHECK IF FILE IS OPEN */
  FILE fileOpen = fopen(fileName, "r");
  if(!fileOpen) {
    char error[100] = "Cannot read the file";
    perror(strcat(error, fileName, "" ));
    return 1;
  }

  cipher = EVP_get_cipherbyname(cipherName);
  if(!cipher) {
    printf("Cipher %s not found.\n", cipherName);
    exit(1);
  }
 
  EVP_CIPHER_CTX *ctx; // context structure
  ctx = EVP_CIPHER_CTX_new();
  if(ctx == NULL) exit(2);
 
  printf("pt: %s\n", pt);
 
  /* Encryption */
  res = EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv);  // context init - set cipher, key, init vector
  if(res != 1) exit(3);
  res = EVP_EncryptUpdate(ctx,  ct, &tmpLength, pt, ptLength);  // encryption of pt
  if(res != 1) exit(4);
  ctLength += tmpLength;
  res = EVP_EncryptFinal_ex(ctx, ct + ctLength, &tmpLength);  // get the remaining ct
  if(res != 1) exit(5);
  ctLength += tmpLength;
 
  printf ("Encrypted %d characters.\n", ctLength);
 
  /* Decryption */
  res = EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv);  // context init for decryption
  if(res != 1) exit(3);
  res = EVP_DecryptUpdate(ctx, pt, &tmpLength,  ct, ctLength);  // decrypt ct
  if(res != 1) exit(4);
  ptLength += tmpLength;
  res = EVP_DecryptFinal_ex(ctx, pt + ptLength, &tmpLength);  // get the remaining plaintext
  if(res != 1) exit(5);
  ptLength += tmpLength;
 
   /* Clean up */
  EVP_CIPHER_CTX_free(ctx);
 
  /* Print out the encrypted and decrypted texts.
     Ciphertext will probably not be printable! */
  printf("CT: %s\nDT: %s\n", ct, pt);
 
  exit(0);
 }