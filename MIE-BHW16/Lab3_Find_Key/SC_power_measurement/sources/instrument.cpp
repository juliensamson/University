/*** Copyright: Petr Vyleta, Jiri Bucek 2012-2015 ***/
/*** vyletpet[at]fit[dot]cvut[dot]cz ***/
/*** bucekj@fit.cvut.cz ***/

/*** Reference: http://www.home.agilent.com/upload/cmc_upload/All/3000_series_prog_guide.pdf ***/

#include <iostream>
#include <sstream>

#include "instrument.h"

using namespace std;

Instrument::Instrument() {}

/*** Opens session manager and a session to the osciloscope ***/
void Instrument::Connect(int const instrumentNumber) {
	/* Open the default resource manager session. */
	Safely(viOpenDefaultRM(&defaultRM_));
	
	/* Open the session using the oscilloscope's VISA address. */
	stringstream instrumentName;
	instrumentName << "USBInstrument" << instrumentNumber;
	Safely(viOpen(defaultRM_, const_cast<char *>(instrumentName.str().c_str()), VI_NULL, VI_NULL, &vi_));
	
	Init();

}

/*** Opens session manager and a session to the osciloscope. ***/
/*** Automaticaly finds the first VISA instrument (hopefully the osciloscope) ***/
int Instrument::Connect() {
	ViChar buffer [VI_FIND_BUFLEN];
	ViRsrc matches = buffer;
	ViUInt32 nmatches = 0;
	ViFindList list;

	/* Open the default resource manager session. */
	Safely(viOpenDefaultRM(&defaultRM_));

	/* FIND any USB instrument*/
	viFindRsrc(defaultRM_, "USB?*INSTR", &list, &nmatches, matches); 
	if (nmatches > 0) cout << "Found: " << buffer << endl;
	viClose(list);

	if (nmatches == 0) {
		cout << "No instrument found! " << endl; 
		exit(EXIT_FAILURE);
	}
	
	/* Open the session using the oscilloscope's VISA address. */
	Safely(viOpen(defaultRM_, buffer, VI_NULL, VI_NULL, &vi_));

	Init();

	return 0;
}

/*** Closes session to the osciloscope as well as the resource manager ***/
Instrument::~Instrument() {
	Safely(viClose(vi_));
	Safely(viClose(defaultRM_));
}

/*** Initialization of the osciloscope. ***/
/*** Checking instrument IDN and resetting it ***/
void Instrument::Init() {
	/* Set the I/O timeout to fifteen seconds. */
	Safely(viSetAttribute(vi_, VI_ATTR_TMO_VALUE, 15000));

	/* Clear the interface. */
	Safely(viClear(vi_));

	/* Get and display the device's *IDN? string. */
	vector<char> idnString(256,0);
	QueryString("*IDN?", idnString);
	printf("Oscilloscope *IDN? string: %s\n", &idnString[0]);
}

/*** Saves osciloscope configuration to the specified file ***/
int Instrument::SaveConf(char const *fileName) {
	/* Read system setup. */
	vector<unsigned char> rawData;
	int numBytesRead = QueryIEEEBlock(":SYSTem:SETup?", rawData);

	/* Write setup string to file. */
	FILE *fp = fopen (fileName, "wb");
	int numBytesWritten = fwrite(&rawData[0], sizeof(rawData[0]), numBytesRead, fp);
	fclose (fp);
	return numBytesRead - numBytesWritten;
}

/*** Loads osciloscope configuration from the specified file  ***/
/*** Uses c++ dynamic structures to avoid index out-of-bounds ***/
int Instrument::LoadConf(char const *fileName) {
	/* Read setup string from file. */
	ifstream file(fileName, ios::binary | ios::ate);
	if (file.fail()) { return 1; }

	/* Determines the file size */
	int fileSize = static_cast<int>(file.tellg());
	/* Return cursor to begining and read the data to vector */
	file.seekg(ios_base::beg);
	vector<unsigned char> rawData(fileSize,0);
	file.read(reinterpret_cast<char *>(&rawData[0]), fileSize);
	file.close();

	/* Restore setup string. */
	int numBytes = CommandIEEEBlock(":SYSTem:SETup", rawData);
	return fileSize - numBytes;
}

/*** Sends specified SCPI command ***/
void Instrument::Command(char const *command) {
	Safely(viPrintf(vi_, "%s\n", command));
	CheckInstrumentErrors();
}

/*** Sends specified SCPI command plus IEEE block of data ***/
int Instrument::CommandIEEEBlock(char const *command, vector<unsigned char> &rawData) {
	int transfered;
	Safely(viPrintf(vi_, "%s #8%08d", command, rawData.size()));
	Safely(viBufWrite(vi_, &rawData[0], rawData.size(), reinterpret_cast<ViPUInt32>(&transfered)));
	CheckInstrumentErrors();
	return transfered;
}

/*** Queries instrument, response expected to be a string ***/
int Instrument::QueryString(char const *query, vector<char> &response) {
	if (response.size() < 255) { response.resize(256,0); }
	int responseLength = response.size();
	Safely(viQueryf(vi_, "%s\n", "%#t\n", query, &responseLength, &response[0]));
	CheckInstrumentErrors();
	return responseLength;
}

/*** Queries instrument, response expected to be a double precision number ***/
void Instrument::QueryDouble(char const *query, double &response) {
	Safely(viQueryf(vi_, "%s\n", "%lf\n", query, &response));
	CheckInstrumentErrors();
}

/*** Queries instrument, response expected to be an IEEE block of data         ***/
/*** Uses viRead to determine the size of the data block to prevent oveflowing ***/
int Instrument::QueryIEEEBlock(char const *query, vector<unsigned char> &response) {
	unsigned char header[2];
	unsigned char sizeString[10];
	unsigned char endChar;
	
	Safely(viPrintf(vi_, "%s\n", query));
	Safely(viRead(vi_, header, 2, VI_NULL));
	int numberOfDigits = header[1] - 48;
	Safely(viRead(vi_, sizeString, numberOfDigits, VI_NULL));
	sizeString[numberOfDigits] = 32; // space sign
	int size = strtol(reinterpret_cast<char*>(sizeString), NULL, 10);
	response.resize(size);
	Safely(viRead(vi_, &response[0], size, VI_NULL));
	Safely(viRead(vi_, &endChar, 1, VI_NULL)); // for reading out whole buffer including ending character
	CheckInstrumentErrors();
	return response.size();
}

/*** Send command, query its status and print it ***/
int Instrument::CommandQueryPrint(char const *command, char const *value) {
	char response[256];
	int responseLength = 256;
	Safely(viPrintf(vi_, "%s %s\n", command, value));
	Safely(viQueryf(vi_, "%s?\n", "%#t\n", command, &responseLength, &response));
	printf("%s %s", command, response);
	CheckInstrumentErrors();
	return responseLength;
}

/*** Send query and print it ***/
int Instrument::QueryPrint(char const *command) {
	char response[256];
	int responseLength = 256;
	Safely(viQueryf(vi_, "%s?\n", "%#t\n", command, &responseLength, &response));
	printf("%s %s", command, response);
	CheckInstrumentErrors();
	return responseLength;
}
	
/*** Instrument error handler ***/
/*** Used after every sent instruction ***/
int Instrument::CheckInstrumentErrors() {
	int const STR_OUT_LEN = 1024;
	int const STR_ERR_LEN = STR_RESULT_SIZE;

	char strErrVal[STR_RESULT_SIZE] = {0};

	Safely(viQueryf(vi_, ":SYSTem:ERRor?\n", "%#t", &STR_ERR_LEN, strErrVal));
	if (strncmp(strErrVal, "+0,\"No error", 3) == 0) { return 0; }
	else {
		char strOut[STR_OUT_LEN] = "";
		while(strncmp(strErrVal, "+0,No error", 3) != 0 ) {
			strcat(strOut, ", ");
			strcat(strOut, strErrVal);
			Safely(viQueryf(vi_, ":SYSTem:ERRor?\n", "%#t", &STR_ERR_LEN, strErrVal));
		}
		if (strcmp(strOut, "") != 0) {
			printf("INST Error%s\n", strOut);
			Safely(viFlush(vi_, VI_READ_BUF));
			Safely(viFlush(vi_, VI_WRITE_BUF));
		}
		return 1;
	}
}
 
/*** VISA Error handler as a wrapper fo VISA functions ***/ 
void Instrument::Safely(ViStatus functionOutput) {
	/* Check for errors */
	if (functionOutput < VI_SUCCESS) {
		/* Array size from documentation */
		char errMsg[256] = {0};
		viStatusDesc(vi_, functionOutput, errMsg);
		printf("VISA Error: %s\n", errMsg);
		if (functionOutput < VI_SUCCESS) { exit(EXIT_FAILURE); }
	}
	/* Check for warnings */
	if (functionOutput > VI_SUCCESS) {
		/* Array size from documentation */
		char errMsg[256] = {0};
		viStatusDesc(vi_, functionOutput, errMsg);
		/* For debugging purposes only */
		//printf("VISA Warning: %s\n", errMsg);
	}
}