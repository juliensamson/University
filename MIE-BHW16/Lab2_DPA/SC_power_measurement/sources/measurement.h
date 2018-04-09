/*** Copyright: Petr Vyleta, Jiri Bucek 2012-2015 ***/
/*** vyletpet[at]fit[dot]cvut[dot]cz ***/
/*** bucekj@fit.cvut.cz ***/


#ifndef _MEASUREMENT_
#define _MEASUREMENT_

/*** Removes warnings regarding security of printf etc. ***/
#define _CRT_SECURE_NO_WARNINGS 1 

#include <cstdio>
#include <cstring>
#include <iostream>
#include <random>
#include <vector>
#include <sstream>

#include <conio.h>
#include <visa.h>

#include "instrument.h"
#include "scard.h"

class Measurement {
public:
	Measurement(Instrument &vi, Smartcard &card);

	void MainMenu			();
	void LoadSetupFile		();
	void SaveSetupFile		();
	void MeasurementRandom	();
	void TestRun			();

	/* NOT USED */
	void PrintScreen		();
	void GetPreamble		();
	void MeasurementCSV		(); 
	
private:
	/* Storing preamble values */
	int format_, type_, points_, avgCount_;
	double xIncrement_, xOrigin_, xReference_;
	double yIncrement_, yOrigin_, yReference_;

	Instrument &vi_;
	Smartcard  &card_;
};

#endif