/*** Copyright: Petr Vyleta, Jiri Bucek 2012-2015 ***/
/*** vyletpet[at]fit[dot]cvut[dot]cz ***/
/*** bucekj@fit.cvut.cz ***/


/* Reference: http://msdn.microsoft.com/EN-US/library/aa379793.aspx */

#include "scard.h"

using namespace std;

Smartcard::Smartcard() {}

/*** Connect creates context for smartcard communication ***/
/*** Establishes connection to the smartcard             ***/

/*** Establish connection to the smartcard reader chosen by user ***/
void Smartcard::Connect() {
	Connect(-1);
}

/*** Establish connection to the smartcard reader chosen by parameter ***/
void Smartcard::Connect(int no) {
	LPTSTR          pReader;
	vector<LPTSTR>  readerList;
	DWORD			readerNumber = 0;
	DWORD			autoAllocate = SCARD_AUTOALLOCATE;	
	DWORD			activeProtocol;
	DWORD			lReturn;

	SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &hContext);
	lReturn = SCardListReaders(hContext, SCARD_ALL_READERS, (LPTSTR)&pmszReaders, &autoAllocate);

	switch(lReturn)	{
    case SCARD_S_SUCCESS:
        pReader = pmszReaders;
        while ( '\0' != *pReader ) {
            printf("Reader %d: %s\n", readerNumber++, pReader);
			readerList.push_back(pReader);
            pReader = pReader + strlen(pReader) + 1;
        }
		readerNumber = no;
		if (readerNumber == -1) {
			printf("Choose reader number: ");
			scanf("%d", &readerNumber);
		}	
		printf("Using reader %d\n", readerNumber);
        break;
	case SCARD_E_NO_READERS_AVAILABLE:
        printf("No reader available\n");
		exit(EXIT_FAILURE);
        break;
	default:
		printf("Failed SCardListReaders\n");
		break;
	}	

	lReturn = SCardConnect(hContext, readerList[readerNumber], SCARD_SHARE_EXCLUSIVE, SCARD_PROTOCOL_T1, &hCard, &activeProtocol);
	if ( SCARD_S_SUCCESS != lReturn ) {	
		printf("Failed SCardConnect\n"); 
		exit(EXIT_FAILURE); 
	}

	switch (activeProtocol)	{
		case SCARD_PROTOCOL_T0:	printf("Active protocol T0\n");	break;
		case SCARD_PROTOCOL_T1:	printf("Active protocol T1\n"); break;
		case SCARD_PROTOCOL_UNDEFINED:
		default:				printf("Active protocol unnegotiated or unknown\n"); break;
	}

	lReturn = SCardBeginTransaction (hCard);
	if ( SCARD_S_SUCCESS != lReturn ) {	printf("Failed SCardBeginTransaction\n"); }	

	recvBuffer.resize(RECV_BUFFER_SIZE,0); // Length for AESblock + SW1 + SW2
	recvLength = recvBuffer.size();
}

/*** Destructor performs requested dealocation of autoalocated memory and releases the context. ***/
Smartcard::~Smartcard () {
	DWORD lReturn;
	lReturn = SCardEndTransaction(hCard, SCARD_LEAVE_CARD);
	if ( SCARD_S_SUCCESS != lReturn ) {	printf("Failed SCardEndTransaction\n");	}
	lReturn = SCardDisconnect(hCard, SCARD_LEAVE_CARD);
	if ( SCARD_S_SUCCESS != lReturn ) {	printf("Failed SCardDisconnect\n");	}
	lReturn = SCardFreeMemory(hContext, pmszReaders );
	if ( SCARD_S_SUCCESS != lReturn ) { printf("Failed SCardFreeMemory\n"); }
	lReturn = SCardReleaseContext(hContext);
	if ( SCARD_S_SUCCESS != lReturn ) { printf("Failed SCardReleaseContext\n"); }
}

/*** Check card status and retrieves ATR ***/
DWORD Smartcard::Status() {
	BYTE		atr[32];
	DWORD		atrLength = 32;
	DWORD		state;
	DWORD		lReturn;

	lReturn = SCardStatus(hCard, NULL, NULL, &state, NULL, atr, &atrLength); 
	if ( SCARD_S_SUCCESS != lReturn ) {	
		lReturn = SCardReconnect(hCard, SCARD_SHARE_EXCLUSIVE, SCARD_PROTOCOL_T1, SCARD_LEAVE_CARD, NULL);
		if ( SCARD_S_SUCCESS != lReturn ) { 
			printf("Failed SCardStatus, SCardReconnect failed\n");
			exit(EXIT_FAILURE);
		} else { 
			printf("Failed SCardStatus, SCardReconnect succesful\n"); 
			lReturn = SCardStatus(hCard, NULL, NULL, &state, NULL, atr, &atrLength); 
		}
	}

	switch ( state ) {
		case SCARD_ABSENT:		printf("Card absent.\n"); break;
		case SCARD_PRESENT:		printf("Card present.\n"); break;
		case SCARD_SWALLOWED:	printf("Card swallowed.\n"); break;
		case SCARD_POWERED:		printf("Card has power.\n"); break;
		case SCARD_NEGOTIABLE:	printf("Card reset and waiting PTS negotiation.\n"); break;
		case SCARD_SPECIFIC:	printf("Card has specific communication protocols set.\n"); break;
		default:				printf("Unknown or unexpected card state.\n"); break;
	}

	printf("Returned ATR:");
	for (DWORD i=0;i<atrLength;i++) { printf(" 0x%02X", atr[i]); }
	printf("\n");

	return state;
}

/*** Send previously constructed APDU ***/
DWORD Smartcard::Send(APDU const &apdu) {
	DWORD lReturn;

	lReturn = SCardTransmit(hCard, SCARD_PCI_T1, &apdu.byte[0], apdu.lc + APDU_FIELDS_SIZE + 1, NULL, &recvBuffer[0], &recvLength);

	if ( SCARD_S_SUCCESS != lReturn ) { printf("Failed SCardTransmit\n"); }
	
	printf("Returned Data: ");
	for (DWORD i=0;i<recvLength;i++) { printf(" 0x%02X", recvBuffer[i]); }
	printf("\n");

	return lReturn;
}

	