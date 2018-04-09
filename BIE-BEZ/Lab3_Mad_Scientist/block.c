/* AUTHOR: JULIEN SAMSON */

/*  READ BMP FILE:
    https://stackoverflow.com/questions/9296059/read-pixel-value-in-bmp-file#9296467

    STRING CONCATENATION
    https://stackoverflow.com/questions/2218290/concatenate-char-array-in-c

    ECB 
  With ECB we can see that the pixels are encoded in a way their initial colors
  Which means that each colored pixel correspond to another color. So it is still
  possible to see the form of the initial image, but all the color are mix up.


    CBC
  With CBC, all the pixel are mixed randomdly on the image which create "noisy" image. 
  With that kind of encryption, it is impossible what the initial image was.

*/
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <string.h>

char* fileName(char* name, char* ext, char *out) {
  out = malloc(strlen(name)+1+20);
  strcpy(out, name);
  strcat(out, ext);
  return out;
}


int main(void) {

  char filename[40] = "Mad_scientist";
  char tempName[60] = "";
  unsigned char key[EVP_MAX_KEY_LENGTH] = "AHCMQLOY7JDOQHY1";  // encryption and decryption key
  unsigned char iv[EVP_MAX_IV_LENGTH] = "initial vector";  // initialization vector
  const char cipherName[] = "DES-CBC";
  const EVP_CIPHER * cipher;
 
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
 
  
  /* 

	ENCRYPTION 

  */

  res = EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv);  // context init - set cipher, key, init vector
  if(res != 1) exit(3);

  /* READ BMP FILE DATA */
  FILE * img = fopen(fileName(filename, (char *)".bmp", tempName), "r");

	//Check if file is open
	if(!img) {
    	   char error[100] = "Cannot read the file";
   	   printf(error);
  	} else {
    	   printf("file is open\n");
  	}

	//Get header information
	unsigned char header[54];
	fread(header, sizeof(unsigned char), 54, img);
	int width = *(int*)&header[18]; printf("Width:    %i \n", width);
	int height = *(int*)&header[22]; printf("Height:   %i \n", height);
	int ptLength = 3 * width * height; printf("FileSize: %i \n", ptLength);

	//Read the data
	unsigned char pt[ptLength];
	fread(pt, sizeof(unsigned char), ptLength, img);
  fclose(img);

  /* WRITE NEW BMP FILE WITH CT */
	
	//Encrypt Pt
	unsigned char ct[ptLength];
	res = EVP_EncryptUpdate(ctx,  ct, &tmpLength, pt, ptLength);  // encryption of pt
  	if(res != 1) exit(4);
	ctLength += tmpLength;

  FILE * img_ct = fopen(fileName(filename, (char *)"_cbc.bmp", tempName), "w");

  	res = EVP_EncryptFinal_ex(ctx, ct + ctLength, &tmpLength);  // get the remaining ct
 	 if(res != 1) exit(5);
  	ctLength += tmpLength;

	//Write header and data
  	fwrite(header, sizeof(unsigned char), 54, img_ct);
	fwrite(ct, sizeof(unsigned char), ctLength, img_ct);
  fclose(img_ct);

  /* 

	DECRYPTION 

  */
	
  res = EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv);  // context init for decryption
  if(res != 1) exit(3);

  /* READ CRYPTED BMP FILE */

  FILE * read_img_ct = fopen(fileName(filename, (char *)"_cbc.bmp", tempName), "r");

	//Check if file is open
	if(!img) {
    	   char error[100] = "Cannot read the file";
   	   printf(error);
  	} else {
    	   printf("file is open\n");
  	}

	//Get header information
	unsigned char ct_header[54];
	fread(ct_header, sizeof(unsigned char), 54, read_img_ct);
	
	//Read the data
	fread(ct, sizeof(unsigned char), ctLength, read_img_ct);
  fclose(read_img_ct);

  /* WRITE NEW BMP FILE WITH CT */
	
	//Decrypt Ct
 	res = EVP_DecryptUpdate(ctx, pt, &tmpLength,  ct, ctLength);  // decrypt ct
 	if(res != 1) exit(4);
 	ptLength += tmpLength;

  FILE * w_img_pt = fopen(fileName(filename, (char *)"_cbc_dec.bmp", tempName), "w");

	//Write header and data
  	fwrite(ct_header, sizeof(unsigned char), 54, w_img_pt);
	fwrite(pt, sizeof(unsigned char), ctLength, w_img_pt);

 	res = EVP_DecryptFinal_ex(ctx, pt + ptLength, &tmpLength);  // get the remaining plaintext
  	if(res != 1) exit(5);
  	ptLength += tmpLength;

  fclose(w_img_pt);


   /* Clean up */
  EVP_CIPHER_CTX_free(ctx);
  exit(0);
 }
