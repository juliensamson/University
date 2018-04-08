#include <stdio.h>
#include <string.h>
//#include <openssl/evp.h>
#include <openssl/evp.h>
 
void rand_str(char *dest, size_t length) {
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (length-- > 0) {
        size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}

int main(int argc, char *argv[]){
 
  int i, res;
  char text[] = "";			//a give ca,97
  char hashFunction[] = "sha256";  // chosen hash function ("sha1", "md5" ...)
  EVP_MD_CTX *ctx;  // context structure
  const EVP_MD *type; // hash fznction type
  unsigned char hash[EVP_MAX_MD_SIZE]; // char array for hash - 64 bytes (max for sha 512)
  int length;  // resulting hash length
  int check_size;
 
  /* Initialization of OpenSSL hash function list */
  OpenSSL_add_all_digests();
  /* Lookup of the needed hash function */
  type = EVP_get_digestbyname(hashFunction);
 
  /* If NULL returned, hash does not exist */
  if(!type) {
    printf("Hash %s does not exist.\n", hashFunction);
    exit(1);
  }

  do {
    rand_str(text, 4);

    ctx = EVP_MD_CTX_create(); // create context for hashing
    if(ctx == NULL) exit(2);
   
    /* Hash the text */
    res = EVP_DigestInit_ex(ctx, type, NULL); // context setup for our hash type
    if(res != 1) exit(3);
    res = EVP_DigestUpdate(ctx, text, strlen(text)); // feed the message in
    if(res != 1) exit(4);
    res = EVP_DigestFinal_ex(ctx, hash, (unsigned int *) &length); // get the hash
    if(res != 1) exit(5);
   
    EVP_MD_CTX_destroy(ctx); // destroy the context
    check_size++;

  } while (hash[1] != 0xFE || hash[0] != 0xCA) ;

  /* Print the resulting hash */
  printf("Text : %s ", text);
  printf("\nText : " );
  for (i = 0; i < 4; i++) {
    printf("%x", text[i]);
  }
  printf("\nHash : ");
  for(i = 0; i < length; i++){
    printf("%02x", hash[i]);
  }
  printf("\n");
 
  exit(0);
}
