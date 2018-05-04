/*
    AUTHOR: JULIEN SAMSON
*/

#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/pem.h>

void EncryptSeal(const char pkey[], const char ptName[], const char ctName[]){

    EVP_PKEY *pubkey = EVP_PKEY_new();
    EVP_CIPHER_CTX ctx;
    unsigned char buff[1024], buff_out[1024];
    int res, len , len_out, eklen;
    unsigned char *ek = NULL;
    unsigned char iv[EVP_MAX_IV_LENGTH];

    /*  Read the rsa public key and assign it as the public key */
    FILE *rsa_pubkey_file = fopen(pkey, "r");
    PEM_read_PUBKEY(rsa_pubkey_file, &pubkey, NULL, NULL);
    fclose(rsa_pubkey_file);

    /*  Instantiate the context */
    EVP_CIPHER_CTX_init(&ctx);

    /*  initialize AES-256 CBC encryption  */
    printf("SEAL INIT....\n");
    ek = malloc(EVP_PKEY_size(pubkey));
    res = EVP_SealInit(&ctx, EVP_aes_256_cbc(), &ek, &eklen, iv, &pubkey, 1);
    if(res != 1) {fputs("SealInit failed.\n", stderr); exit(4);}

    /*  Write the header to the message_crypt.txt file with the encrypted key length
     *  the encryt key, and the IV.   */

    FILE * pt_file = fopen(ptName, "r");
    FILE * ct_file = fopen(ctName, "w");

    res = fwrite(&eklen, sizeof eklen, 1, ct_file);
    if (res != 1) { fputs("Failed to write key length to ct.\n", stderr); exit(5);}

    res = fwrite(ek, eklen, 1, ct_file);
    if (res != 1) { fputs("Failed to write key to ct.\n", stderr); exit(6);}

    res = fwrite(iv, EVP_CIPHER_iv_length(EVP_aes_256_cbc()), 1, ct_file);
    if (res != 1) { fputs("Failed to write IV to ct.\n", stderr); exit(7);}

    /*  Read the message.txt (PT) and encrypt the data into the output file */

    printf("SEAL UPDATE....\n");
    while ((len = fread(buff, 1, sizeof buff, pt_file)) > 0)  {

        res = EVP_SealUpdate(&ctx, buff_out, &len_out, buff, len);
        if (res != 1) { fputs("SealUpdate failed.\n", stderr); exit(8);}

        res = fwrite(buff_out, len_out, 1, ct_file);
        if (res != 1) { fputs("Failed to write to encerypt file.\n", stderr); exit(9);}
    }

    printf("SEAL FINAL....\n");
    res = EVP_SealFinal(&ctx, buff_out, &len_out);
    if (res != 1) { fputs("SealFinal Failed\n", stderr); exit(10);}

    res = fwrite(buff_out, len_out, 1, ct_file);
    if (res != 1) { fputs("Failed to write to encerypt file.\n", stderr); exit(11);}

    fclose(pt_file);
    fclose(ct_file);

    EVP_PKEY_free(pubkey);
    free(ek);
}

int main(void) {
  printf("Encrypting....\n");
  EncryptSeal("pubkey.pem", "message.txt", "message_crypt.bin");
  exit(0);
 }
