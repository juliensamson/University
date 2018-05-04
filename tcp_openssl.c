#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <openssl/evp.h>

int main(void) {
 
  char host[1024] = "fit.cvut.cz";
  int port = 443;

  int socketFd;
  char receveidBuff[1024];
  struct SocketAddress_in serverAddress;

  printf("Host: %s\n", host);
  printf("Port: %i\n", port);


  //SSL_library_init();
  socketFd = socket(AF_INET, SOCK_STREAM, 0);
  bzero(&serverAddress, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = inet_addr(host);
  serverAddress.sin_port = htons(port);

  if (0 != connect(socketFd, (struct SocketAddress *)&serverAddress, sizeof(serverAddress))) {
    printf("Connection error...\n");
    exit(1);
  }
  /*
  unsigned char pt[1024] = "Text for rc4.";  // plaintext
  unsigned char ct[1024];  // ciphertext
  unsigned char key[EVP_MAX_KEY_LENGTH] = "My key";  // encryption and decryption key
  unsigned char iv[EVP_MAX_IV_LENGTH] = "initial vector";  // initialization vector
  const char cipherName[] = "RC4";
  const EVP_CIPHER * cipher;
 
  int ptLength = strlen((const char*) pt);
  int ctLength = 0;
  int tmpLength = 0;
  int res;
 
  OpenSSL_add_all_ciphers();
  
 
  cipher = EVP_get_cipherbyname(cipherName);
  if(!cipher) {
    printf("Cipher %s not found.\n", cipherName);
    exit(1);
  }
 
  EVP_CIPHER_CTX *ctx; // context structure
  ctx = EVP_CIPHER_CTX_new();
  if(ctx == NULL) exit(2);
 
  printf("pt: %s\n", pt);
 
  
  res = EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv);  // context init - set cipher, key, init vector
  if(res != 1) exit(3);
  res = EVP_EncryptUpdate(ctx,  ct, &tmpLength, pt, ptLength);  // encryption of pt
  if(res != 1) exit(4);
  ctLength += tmpLength;
  res = EVP_EncryptFinal_ex(ctx, ct + ctLength, &tmpLength);  // get the remaining ct
  if(res != 1) exit(5);
  ctLength += tmpLength;
 
  printf ("Encrypted %d characters.\n", ctLength);
 
  
  res = EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv);  // context init for decryption
  if(res != 1) exit(3);
  res = EVP_DecryptUpdate(ctx, pt, &tmpLength,  ct, ctLength);  // decrypt ct
  if(res != 1) exit(4);
  ptLength += tmpLength;
  res = EVP_DecryptFinal_ex(ctx, pt + ptLength, &tmpLength);  // get the remaining plaintext
  if(res != 1) exit(5);
  ptLength += tmpLength;
 
   
  EVP_CIPHER_CTX_free(ctx);
 

  printf("CT: %s\nDT: %s\n", ct, pt);
  */
  exit(0);
 }