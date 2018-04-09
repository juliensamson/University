/*** Copyright: Petr Vyleta, Jiri Bucek 2012-2015 ***/
/*** vyletpet[at]fit[dot]cvut[dot]cz ***/
/*** bucekj@fit.cvut.cz ***/


#ifndef _SCARD_
#define _SCARD_

/*** Removes warnings regarding security of printf etc. ***/
#define _CRT_SECURE_NO_WARNINGS 1 

#include <cstdio>
#include <cstring>
#include <vector>

#include <winscard.h>

/*** APDU related constants, set to case 4 APDU and AES block data ***/
#define APDU_FIELDS_SIZE       5
#define APDU_DATA_SIZE        16

/*** Receive buffer size, set to AES block + SW1 + SW2 ***/
#define RECV_BUFFER_SIZE ( APDU_DATA_SIZE + 2 ) 

using namespace std;

/*** APDU defined as struct with references for two data input posibilities ***/
/***  A. Input separate fields of APDU  (for user input data)               ***/
/***  B. Input whole APDU as array      (for data loaded from file)         ***/
class APDU {
public:
	vector<BYTE> byte;
	BYTE &cla;
	BYTE &ins;
	BYTE &p1;
	BYTE &p2;
	BYTE &lc;
	BYTE &le;
	BYTE * const data;

	/* Constructor, copy constructor and assignement operator defined because of references used */
	APDU(int dataSize) : byte(dataSize + APDU_FIELDS_SIZE + 1), cla(byte[0]), ins(byte[1]), p1(byte[2]), p2(byte[3]), lc(byte[4]), data(&byte[5]), le(byte[byte.size() - 1]) {}
	APDU(APDU const &rAPDU) : byte(rAPDU.byte.size()),            cla(byte[0]), ins(byte[1]), p1(byte[2]), p2(byte[3]), lc(byte[4]), data(&byte[5]), le(byte[rAPDU.byte.size() - 1]) {
		for (BYTE i=0;i<rAPDU.byte.size();i++) byte[i] = rAPDU.byte[i];
	}
	APDU &operator=(APDU const &rAPDU) {
		for (BYTE i=0;i<rAPDU.byte.size();i++) { byte[i] = rAPDU.byte[i]; }
		return *this;
	}
};

/*** Smartcard class. Creates connection with user specified reader and the card in it. ***/
class Smartcard {
public:
	Smartcard();
	~Smartcard();
	void Connect();
	void Connect(int no);

	DWORD Status();
	DWORD Send(APDU const &apdu);

	vector<BYTE>	recvBuffer;
private:
	SCARDHANDLE		hCard;
	SCARDCONTEXT	hContext;
	LPTSTR          pmszReaders;
	DWORD			recvLength;
};

#endif