all: crypt decrypt

crypt: crypt.c
	gcc -Wall crypt.c -lcrypto -o crypt

decrypt: decrypt.c
	gcc -Wall decrypt.c -lcrypto -o decrypt

run:
	./crypt
	./decrypt
