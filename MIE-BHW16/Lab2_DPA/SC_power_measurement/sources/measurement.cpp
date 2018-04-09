/*** Copyright: Petr Vyleta 2012 ***/
/*** vyletpet[at]fit[dot]cvut[dot]cz ***/

#include <iomanip>

#include "measurement.h"

using namespace std;

Measurement::Measurement(Instrument &vi, Smartcard &card) : vi_(vi), card_(card) {}

void Measurement::MainMenu() {

	cout << "****************** Main Menu *********************" << endl;
	cout << "*  [1] Measuremnt random input                   *" << endl;
	cout << "*  [2] Load osciloscope configuration            *" << endl;
	cout << "*  [3] Save osciloscope configuration            *" << endl;
	cout << "*  [4] Test run (infinite until key pressed)     *" << endl;
	cout << "*  [r] Reset osciloscope                         *" << endl;
	cout << "*  [q] Quit                                      *" << endl;
	cout << "**************************************************" << endl;
	cout << "* Enter your choice: ";

	while (true) {
		char    menuChoice;
		cin >>  menuChoice;
		switch (menuChoice) {
			case '1': return MeasurementRandom(); break;
			case '2': return LoadSetupFile    (); break;
			case '3': return SaveSetupFile    (); break;
			case '4': return TestRun          (); break;
			case 'p': return PrintScreen      (); break;
			case 'r': vi_.Command("*RST"); break;			/* Reset */ 
			case 'q': exit(EXIT_SUCCESS);					/* Quit  */
			default: cout << "* Illegal choice. Try again: "; break;
		}
	}
}

void Measurement::LoadSetupFile() {
	cout << "* Load setup file from program directory" << endl;
	cout << "* Enter file name: ";
	while (true) {
		string filename;
		cin >> filename;
		if ( vi_.LoadConf(filename.c_str()) == 0) { break ; }
		else { cout << "* File load failed. Enter file name again: "; }
	}
	cout << "* File succesfully loaded" << endl;
}

void Measurement::SaveSetupFile() {
	unsigned int lReturn;

	cout << "* Save setup file to program directory" << endl;
	cout << "* Enter file name: ";
	string filename;
	cin >> filename;
	lReturn = vi_.SaveConf(const_cast<const char *>(filename.c_str()));
	if (lReturn == 0) { cout << "* File succesfully saved" << endl; }
	else              { cout << "* File save failed"       << endl; }
}

/*** Send constant data to card untill key is pressed ***/
void Measurement::TestRun() {

	APDU apdu(APDU_DATA_SIZE);
	apdu.cla = 0x80;
	apdu.ins = 0x60;
	apdu.p1  = 0x00;
	apdu.p2  = 0x00;
	apdu.lc  = 0x10;
	apdu.le  = 0x10;
	for (int i=0;i<APDU_DATA_SIZE;i++) { apdu.data[i] = 0x11; }

	/* Check whether card is ready, reconnects if necesary */
	card_.Status();  

	/* Infinite loop, ends when key is pressed */
	/* _kbhit() is ISO c++ conformant, POSIX name is kbhit() */
	while (_kbhit() == 0) { card_.Send(apdu); }
}

void Measurement::PrintScreen() { 
	/* Download the screen image */
	vi_.Command          (":HARDcopy:INKSaver OFF");
	/* Read screen image. */
	vector<unsigned char> rawData;
	vi_.QueryIEEEBlock   (":DISPlay:DATA? PNG, COLor", rawData);

	/* Write screen image to file. */
	ofstream screen;
	screen.open ("printscreen.png", ios::out | ios::trunc | ios::binary); 
	screen.write(reinterpret_cast<char *>(&rawData[0]),rawData.size());
	screen.close();
}

/*** Sends specified numbers of random values and performs measurement ***/
void Measurement::MeasurementRandom () {
	
	/* Output files and streams */
	ofstream tracesFile("traces.bin", ios::out | ios::trunc | ios::binary);
	stringstream plaintext, ciphertext;
	
	/* Random number generator for plaintext data */
	unsigned long seed = 1;
	default_random_engine rnd(seed);

	int numOfMeasurements;
	cout << "Enter number of requested measurements: ";
	cin >> numOfMeasurements;

	/* APDU header for AES */
	APDU apdu(APDU_DATA_SIZE);
	apdu.cla = 0x80;
	apdu.ins = 0x60;
	apdu.p1  = 0x00;
	apdu.p2  = 0x00;
	apdu.lc  = 0x10;
	apdu.le  = 0x10;

	/* Set the necesary measurement parameters */

	/* Using Average instead of Normal to lower sample rate */
	/* Sample rate otherwise not directly controlable       */
//	vi_.CommandQueryPrint(":ACQuire:TYPE",         "NORMal");
	vi_.CommandQueryPrint(":ACQuire:TYPE",         "AVERage");
	vi_.CommandQueryPrint(":ACQuire:COUNt",        "2");
	vi_.CommandQueryPrint(":TIMebase:MODE",        "MAIN");
	vi_.CommandQueryPrint(":WAVeform:UNSigned",    "1");
	vi_.CommandQueryPrint(":WAVeform:BYTeorder",   "LSBFirst");
	vi_.CommandQueryPrint(":WAVeform:FORMat",      "BYTE");
	vi_.CommandQueryPrint(":WAVeform:SOURce",      "CHANnel2");
	vi_.CommandQueryPrint(":WAVeform:POINts:MODE", "RAW");
	vi_.CommandQueryPrint(":ACQuire:COMPlete",     "100");	
		
	/* Check whether card is ready, reconnects if necesary */
	card_.Status(); 
	/* First dummy measurement */
	card_.Send(apdu);
	
	/* Received measurement traces buffer */
	vector<unsigned char> rawData;

	/* Performs requested number of measurements and encryptions */
	for (int i=0;i<numOfMeasurements;i++) {
		for (int j=0;j<APDU_DATA_SIZE;j++) { apdu.data[j] = rnd() % 256; }
		/* Actual measurement command for single waveform starting at trigger */
		/* (same as pressing single on the osciloscope )                      */
		vi_.Command       (":SINGle");
		card_.Send(apdu);	
		/* Get the number of waveform points available. */
		vi_.QueryPrint    (":WAVeform:POINts");
		/* Read waveform data. */
		vi_.QueryIEEEBlock(":WAVeform:DATA?", rawData);
		tracesFile.write(reinterpret_cast<char *>(&rawData[0]), rawData.size());

		/* Writes to log (send + received in ascii, each measurement new line) */
		for (int i=0;i<APDU_DATA_SIZE;i++) { plaintext  << hex << setw(2) << setfill('0') << static_cast<int>(apdu.data[i])       << " "; }	
		for (int i=0;i<APDU_DATA_SIZE;i++) { ciphertext << hex << setw(2) << setfill('0') << static_cast<int>(card_.recvBuffer[i]) << " "; }
		plaintext  << endl;
		ciphertext << endl;
	}

	tracesFile.close();
		
	/* Save plaintext and ciphertext to file */
	ofstream  plaintextFile("plaintext.txt",  ios::out | ios::trunc );
	ofstream ciphertextFile("ciphertext.txt", ios::out | ios::trunc );
	plaintextFile  << plaintext.rdbuf()  << endl;
	ciphertextFile << ciphertext.rdbuf() << endl;
	plaintextFile. close();
	ciphertextFile.close();

	/* Save trace length in points to file for future use */
	vector<char> traceLength;
	vi_.QueryString(":WAVeform:POINts?", traceLength);
	ofstream traceLenghtFile("traceLength.txt", ios::out | ios::trunc );
	traceLenghtFile.write(&traceLength[0], traceLength.size());
	traceLenghtFile.close();
}

/*** NOT USED ***/

/*** Get waweform preamble ***/	
void Measurement::GetPreamble() {
	vector<char> preamble;
	vi_.QueryString      (":WAVeform:PREamble?", preamble);
	/* Using sscanf, because parsing through stream is complicated */
	sscanf(&preamble[0]	, "%d,%d,%d,%d,%lf,%lf,%lf,%lf,%lf,%lf"
						, &format_,     &type_,    &points_,    &avgCount_
						, &xIncrement_, &xOrigin_, &xReference_
						, &yIncrement_, &yOrigin_, &yReference_);
}

/*** Retrieves measurement and stores it in a .csv file ***/
/*** Very slow. Only shows how to use preamble data.    ***/
void Measurement::MeasurementCSV() {
	/* Get the number of waveform points available. */
	vi_.QueryPrint       (":WAVeform:POINts");
			
	GetPreamble();

	switch(format_) {
		case 0:	cout << "Waveform format: BYTE"  << endl; break;
		case 1:	cout << "Waveform format: WORD"  << endl; break;
		case 2:	cout << "Waveform format: ASCii" << endl; break;
		default: break;
	}
	switch(type_) {
		case 0:	cout << "Acquire type: NORMal"      << endl; break;
		case 1:	cout << "Acquire type: PEAK"        << endl; break;
		case 2:	cout << "Acquire type: AVERage"     << endl; break;
		case 3:	cout << "Acquire type: HRESolution" << endl; break;
		default: break;
	}
	cout << "Waveform points:      " << points_		<< endl;
	cout << "Waveform avg count:   " << avgCount_	<< endl;
	cout << "Waveform X increment: " << xIncrement_	<< endl;
	cout << "Waveform X origin:    " << xOrigin_	<< endl;
	cout << "Waveform X reference: " << xReference_	<< endl;
	cout << "Waveform Y increment: " << yIncrement_	<< endl;
	cout << "Waveform Y origin:    " << yOrigin_	<< endl;
	cout << "Waveform Y reference: " << yReference_	<< endl;
	
	/* Download waveform data. */

	/* Read waveform data. */
	vector<unsigned char> rawData;
	vi_.QueryIEEEBlock(":WAVeform:DATA?", rawData);
	
	/* Output waveform data in CSV format. */
	stringstream wave( ios::out | ios::in | ios::binary );
	for (unsigned int i=0; i < rawData.size(); i++) {
		/* Write time value, voltage value. */
		wave << xOrigin_ + (static_cast<double>(i) * xIncrement_) << ", "; 
		wave << ((static_cast<double>(rawData[i]) - yReference_) * yIncrement_) + yOrigin_ << endl;
	}
	
	/* Open file for output. */
	ofstream wavefile("waveform_data.csv", ios::out | ios::trunc | ios::binary );
	wavefile << wave.rdbuf();
	wavefile.close();

	cout << "Waveform format BYTE data written to waveform_data.csv" << endl;
}