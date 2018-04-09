/*
	This Project has been modified by Filip Stepanek <filip.stepanek@fit.cvut.cz>,
	FIT-CTU <www.fit.cvut.cz/en> for the purpose of smartcard education 
	using the SOSSE <http://www.mbsks.franken.de/sosse/html/index.html> 
	created by Matthias Bruestle and files 	from the Chair for Embedded Security (EMSEC), 
	Ruhr-University Bochum <http://www.emsec.rub.de/chair/home/>.
*/


#include "example_AES.h"
#include <avr/io.h>

/**
 *	set the trigger PIN
 */
#define set_pin(port, value) ((port)|=(value))
/**
 *	clear the trigger PIN
 */
#define clear_pin(port, value) ((port)&=(value))

/*...*/

unsigned char buffer[16];

const unsigned char sbox[256] =
{
	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

const unsigned char RC[15] = 
{ 
	0x01, 0x02, 0x04, 
	0x08, 0x10, 0x20, 
	0x40, 0x80, 0x1b, 
	0x36, 0x6c, 0xd8, 
	0xab, 0x4d, 0x9a 
};


const unsigned char mix_matrix[16] =
{
	0x02, 0x03, 0x01, 0x01,
	0x01, 0x02, 0x03, 0x01,
	0x01, 0x01, 0x02, 0x03,
	0x03, 0x01, 0x01, 0x02
};

void byte_substitution_layer(unsigned char state[4][4]) {

	unsigned char position = 0x00;

	for (uint32_t i = 0; i < 4; i++) {
		for (uint32_t j = 0; j < 4; j++) {
			position = state[i][j];
			state[i][j] = sbox[position];
		}
	}
}

void g_function(unsigned char *word_byte, uint32_t round) {

	//	Shifting by 1 on the left
	unsigned char t = word_byte[0];
	word_byte[0] = word_byte[1];
	word_byte[1] = word_byte[2];
	word_byte[2] = word_byte[3];
	word_byte[3] = t;

	//	SBox
	//	Get the right bytes from the word
	for(int i = 0; i < 4; i++) 
		word_byte[i] = sbox[word_byte[i]];

	//	XOR with RC
	//	XOR the first byte of the word with the RC function (
	//	For some reason the first position is the last position in temp
	word_byte[0] = word_byte[0] ^ RC[round];	
}

void add_round_key(unsigned char state[4][4], unsigned char *w) {

	for (int i = 0; i < 4; i++) {

		for (uint32_t j = 0; j < 4; j++) {
			state[i][j] ^= w[(4 * j) + i];
		}
	}
}

void key_scheduling(unsigned char *key, unsigned char *w) {
	
	unsigned char temp_word[4];
	
	//First key are the first 4 words
	for (int i = 0; i < 4; i++) {
		w[(i * 4) + 0] = key[(i * 4) + 0];
		w[(i * 4) + 1] = key[(i * 4) + 1];
		w[(i * 4) + 2] = key[(i * 4) + 2];
		w[(i * 4) + 3] = key[(i * 4) + 3];
	
	}
	
	 //	Create W[4] to W[44]
	 uint32_t round = 0;
	 for (int i = 4; i < 4 * (10 + 1); i++) 
	 {
		//Get previous word
		for(int j = 0; j < 4;j++)
			temp_word[j] = w[4 * (i - 1) + j];

		if ((i % 4) == 0) {
			 g_function(temp_word, round);
			 round++;
		}
		for(int j = 0; j < 4;j++) {
			w[(4 * i) + j] = w[4 * (i - 4) + j] ^ temp_word[j];
		}
	 }
}

void shift_row_layer(unsigned char state[4][4]) {

	// Shift left by 1
	uint32_t temp;
	temp = state[1][0];
	for (int i = 0; i < 4; i++) {
		if (i < 3)
			state[1][i] = state[1][i + 1];
		else 
			state[1][i] = temp;
	}

	// Shift left by 2
	temp = state[2][0];
	state[2][0] = state[2][2];
	state[2][2] = temp;
	temp = state[2][1];
	state[2][1] = state[2][3];
	state[2][3] = temp;

	// Shift left by 3
	temp = state[3][0];
	state[3][0] = state[3][3];
	state[3][3] = state[3][2];
	state[3][2] = state[3][1];
	state[3][1] = temp;
}

void mix_column_layer(unsigned char state[4][4]) {

	//	Assigne the state to a temporary state
	unsigned char temp_state[4][4];
	for (int i = 0; i < 4; i++) {
		for (uint32_t j = 0; j < 4; j++) {
			temp_state[i][j] = state[i][j];
		}
	}

	//	Perform the mix column function
	unsigned char col[4];
	for (int k = 0; k < 4; k++) 
	{
		for (int j = 0; j < 4; j++)
		{
			// Get the value from the state in array corresponding to the column which will be mixed
			for (int i = 0; i < 4; i++)
				col[i] = temp_state[i][k];

			//	Perform the multiplication between the column and the constant matrix
			for (int i = 0; i < 4; i++) {

				//	if 1 the value stay the same
				if (mix_matrix[(4 * j) + i] == 0x01) {
					col[i] = col[i];
				}
				//	if 2, we need to apply a left shift on the byte and XOR with 1B if the most significant bit is 1
				else if (mix_matrix[(4 * j) + i] == 0x02) {
					if (col[i] >= 0x80) {
						col[i] = col[i] << 1;
						col[i] = col[i] ^= 0x1b;
					}
					else {
						col[i] = col[i] << 1;
						col[i] = col[i] ^= 0x00;
					}
				}
				//	if 3, same as 2 but we need to apply a XOR 
				else if (mix_matrix[(4 * j) + i] == 0x03) {
					if (col[i] >= 0x80) {
						col[i] = col[i] << 1;
						col[i] = col[i] ^= 0x1b;
					}
					else {
						col[i] = col[i] << 1;
						col[i] = col[i] ^= 0x00;
					}
					col[i] = col[i] ^= temp_state[i][k];
				}
			}
			//Addition
			col[0] = col[0] ^= col[1];
			col[2] = col[2] ^= col[3];
			state[j][k] = col[0] ^= col[2];
		}
	}
}


void encrypt_aes_16(unsigned char *in, unsigned char *out, unsigned char *skey)
{

	//... Initialize ...
	unsigned char state[4][4];
	unsigned char round_key[176];
	
	// set trigger PIN
	set_pin(DDRB, 0b10100000);
	set_pin(PORTB, 0b10100000);

	//... Encrypt ...
	for (int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++)
			state[j][i] = in[(4 * i) + j];
	}

	key_scheduling(skey, round_key);
	add_round_key(state, round_key);

	for (uint32_t i = 1; i <= 10; i++) {
		if (i != 10) {
			byte_substitution_layer(state);
			shift_row_layer(state);
			mix_column_layer(state);
			add_round_key(state, round_key + 16 * i);
		}
		else {
			byte_substitution_layer(state);
			shift_row_layer(state);
			add_round_key(state, round_key + 16 * i);		
		}
	}

	// clear trigger PIN
	clear_pin(PORTB, 0b01011111);

	//... Copy output ...
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			out[(4 * i) + j] = state[j][i]; //change later
	}


}


