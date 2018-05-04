/*
 *    AUTHOR: JULIEN SAMSON
 */

#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/pem.h>

void DecryptSeal(const char pkName[], const char ctName[], const char ptName[]){

    EVP_PKEY *privkey = EVP_PKEY_new();
    EVP_CIPHER_CTX ctx;
    unsigned char buff[1024], buff_out[1024];
    int res, len, len_out, eklen;
    unsigned char *ek = NULL;
    unsigned char iv[EVP_MAX_IV_LENGTH];


    /*  Read the rsa public key and assign it as the public key */
    FILE *rsa_privkey_file = fopen(pkName, "r");
    PEM_read_PrivateKey(rsa_privkey_file, &privkey, NULL, NULL);
    fclose(rsa_privkey_file);

    /*  Instantiate the context */
    EVP_CIPHER_CTX_init(&ctx);

    /*  Get the header (key length, key, and IV)  */

    printf("READ PUBLIC INFORMATION...\n");
    ek = malloc(EVP_PKEY_size(privkey));

    FILE * ct_file = fopen(ctName, "rb");
    FILE * pt_file = fopen(ptName, "w");

    res = fread(&eklen, sizeof eklen, 1, ct_file);
    if (res != 1) { fputs("Failed to read key length to ct.\n", stderr); exit(5);}

    res = fread(ek, eklen, 1, ct_file);
    if (res != 1) { fputs("Failed to read key to ct.\n", stderr); exit(6);}

    res = fread(iv, EVP_CIPHER_iv_length(EVP_aes_256_cbc()), 1, ct_file);
    if (res != 1) { fputs("Failed to read IV to ct.\n", stderr); exit(7);}

    /*  Use OpenInit for the decryption */

    printf("OPEN INIT...\n");
    res = EVP_OpenInit(&ctx, EVP_aes_256_cbc(), ek, eklen, iv, privkey);
    if(res != 1) {fputs("OpenInit failed.\n", stderr); exit(4);}

    /*  Read the message.txt (PT) and encrypt the data into the output file */
    printf("OPEN UPDATE....\n");
    while ((len = fread(buff, 1, sizeof buff, ct_file)) > 0)  {

        res = EVP_OpenUpdate(&ctx, buff_out, &len_out, buff, len);
        if (res != 1) { fputs("OpenUpdate failed.\n", stderr); exit(8);}

        res = fwrite(buff_out, len_out, 1, pt_file);
        if (res != 1) { fputs("Failed to write to new file.\n", stderr); exit(9);}
    }
    printf("OPEN FINAL....\n");
    res = EVP_OpenFinal(&ctx, buff_out, &len_out);
    if (res != 1) { fputs("OpenFinal Failed\n", stderr); exit(10);}

    res = fwrite(buff_out, len_out, 1, pt_file);
    if (res != 1) { fputs("Failed to write to new file.\n", stderr); exit(11);}

    fclose(pt_file);
    fclose(ct_file);

    EVP_PKEY_free(privkey);
    free(ek);
}

int main(void) {
  printf("Decrypting....\n");
  DecryptSeal("privKey.pem", "message_crypt.bin", "message_decrypt.txt");
  exit(0);
}
