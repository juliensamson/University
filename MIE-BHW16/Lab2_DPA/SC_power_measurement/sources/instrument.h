/*** Copyright: Petr Vyleta, Jiri Bucek 2012-2015 ***/
/*** vyletpet[at]fit[dot]cvut[dot]cz ***/
/*** bucekj@fit.cvut.cz ***/


#ifndef _INSTRUMENT_
#define _INSTRUMENT_

/*** Removes warnings regarding security of printf etc. ***/
#define _CRT_SECURE_NO_WARNINGS 1 

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>

#include <vector>
#include <fstream>

#include <visa.h>

/*** Program constants ***/ 
/* Size of response string */
#define STR_RESULT_SIZE 256

using namespace std;

/*** Instrument class. Connects to the osciloscope using VISA.                   ***/
/*** Is capable of sending commands and receiving data back in various formats.  ***/
class Instrument {
public:
	Instrument();
	~Instrument();
	int  Connect();
	void Connect(int const instrumentNumber);

	int SaveConf(char const *fileName);
	int LoadConf(char const *fileName);

	void Command          (char const *command);
	int  CommandIEEEBlock (char const *command, vector<unsigned char> &ieeeBlock);
	int  QueryString      (char const *query,   vector<char> &response);
	int  QueryIEEEBlock   (char const *query,   vector<unsigned char> &response);
	int  CommandQueryPrint(char const *command, char const *commandValue);
	int  QueryPrint       (char const *command);

	/* NOT USED */
	void QueryDouble      (char const *query,   double &response);

private:
	void Init();
	void Safely(ViStatus functionOutput);
	int CheckInstrumentErrors();
	ViSession defaultRM_;
	ViSession vi_;
};

#endif