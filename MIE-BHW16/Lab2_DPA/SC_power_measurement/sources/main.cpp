/*** Copyright: Petr Vyleta, Jiri Bucek 2012-2015 ***/
/*** vyletpet[at]fit[dot]cvut[dot]cz ***/
/*** bucekj@fit.cvut.cz ***/

/*** Removes warnings regarding security of printf etc. ***/
#define _CRT_SECURE_NO_WARNINGS 1 

#include <cstdio>
#include <cstring>

#include <iostream>
#include <vector>

#include <visa.h>
#include <winscard.h>

#include "instrument.h" 
#include "scard.h"
#include "measurement.h"

using namespace std;

void main() {

	cout << endl;
	cout << "***************  Smart card DPA  *****************" << endl;
	cout << "*      Czech Technical University in Prague      *" << endl;
	cout << "*       Faculty of Information Technology        *" << endl;
	cout << "**************************************************" << endl;
	cout << "* Configuring osciloscope and smart card..."        << endl;
	
	/* Connect to osciloscope */
	Instrument scope;
	scope.Connect();
	
	/* Connect to smart card */
	Smartcard card;
    card.Connect(0); // Use card no. 0. [Connect() to let user choose]
 
	/* Check card status, retrieve ATR */
	card.Status();

	cout << "* Osciloscope and smartcard configured" << endl;
	cout << "* Please, set osciloscope manually or load configuration" << endl;

	/* Program running */
	Measurement measurement(scope, card);
	while (true) { measurement.MainMenu(); };
}