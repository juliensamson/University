all: hash stream

hash: hash.c
	gcc -Wall hash.c -lcrypto -o hash

stream: stream.c
	gcc -Wall stream.c -lcrypto -o stream

run:
	./hash
	./stream
