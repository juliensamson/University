#include <stdio.h>
#include <stdint.h>

#define mod 79
uint8_t A[10] = { 0x4A, 0x2E, 0x38, 0xA8, 0xF6, 0x6D, 0x7F, 0x4C, 0x38, 0x5F };
uint8_t B[10] = { 0x2C, 0x0B, 0xB3, 0x1C, 0x6B, 0xEC, 0xC0, 0x3D, 0x68, 0xA7 };

//Test data
uint8_t x_P[10] = { 0x30, 0xCB, 0x12, 0x7B, 0x63, 0xE4, 0x27, 0x92, 0xF1, 0x0F };
uint8_t y_P[10] = { 0x54, 0x7B, 0x2C, 0x88, 0x26, 0x6B, 0xB0, 0x4F, 0x71, 0x3B };
uint8_t x_Q[10] = { 0x00, 0x20, 0x2A, 0x9F, 0x03, 0x50, 0x14, 0x49, 0x73, 0x25 };
uint8_t y_Q[10] = { 0x51, 0x75, 0xA6, 0x48, 0x59, 0x55, 0x2F, 0x97, 0xC1, 0x29 };
uint8_t x_test[10] = { 0x30, 0xCB, 0x12, 0x7B, 0x63, 0xE5, 0x27, 0x92, 0xF1, 0x0F };
uint8_t y_test[10] = { 0x54, 0x7B, 0x2C, 0x88, 0x26, 0x6B, 0xB0, 0x4F, 0x71, 0x3B };

//Square table
uint16_t square_table[256];

void printhexa(uint8_t *a, size_t size) {

	for (int i = 0; i < size; i++) {
		if (a[i] < 0x10)
			printf("0%x ", a[i]);
		else
			printf("%x ", a[i]);
	}
	printf("\n");
}

uint8_t table_value(int init_val) {
	switch (init_val)
	{
	case 0:
		return 0x00;
	case 1:
		return 0x01;
	case 2:
		return 0x04;
	case 3:
		return 0x05;
	case 4:
		return 0x10;
	case 5:
		return 0x11;
	case 6:
		return 0x14;
	case 7:
		return 0x15;
	case 8:
		return 0x40;
	case 9:
		return 0x41;
	case 10:
		return 0x44;
	case 11:
		return 0x45;
	case 12:
		return 0x50;
	case 13:
		return 0x51;
	case 14:
		return 0x54;
	case 15:
		return 0x55;
	default:
		break;
	}
}

uint8_t mask(int init_val) {
	switch (init_val)
	{
	case 0:
		return 0x01;
	case 1:
		return 0x02;
	case 2:
		return 0x04;
	case 3:
		return 0x08;
	case 4:
		return 0x10;
	case 5:
		return 0x20;
	case 6:
		return 0x40;
	case 7:
		return 0x80;
	default:
		return 0x00;
	}
}

void compute_square_table() {
	for (int i = 0; i < 16; i++)
	{
		uint8_t msb = table_value(i);
		for (int j = 0; j < 16; j++)
		{
			uint8_t lsb = table_value(j);
			square_table[(16 * i) + j] = (msb << 8) | (lsb & 0xff);
		}
	}
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			if (square_table[(16 * i) + j] < 0x0010)
				printf("000%x ", square_table[(16 * i) + j]);
			else if (square_table[(16 * i) + j] < 0x0056)
				printf("00%x ", square_table[(16 * i) + j]);
			else if (square_table[(16 * i) + j] < 0x1000)
				printf("0%x ", square_table[(16 * i) + j]);
			else
				printf("%x ", square_table[(16 * i) + j]);
		}
		printf("\n");
	}

	//Square each 256 element, create atable with it 
	//00-->0000, 01-->0001, 02-->0004, 03-->0005
	//58 = 0101 1000 (x^6+x^4+x^3), (x^12 + x^8 + x^6) 0000 1000 0100 0000 
}

uint8_t shiftLeftByEight(uint8_t *arr, size_t len) {
	uint8_t temp_arr[20];
	for (int i = 0; i < 20; i++) {
		temp_arr[i] = arr[i];
	}

	int shift = len - 1;
	uint8_t t = temp_arr[len];

	for (int i = 0; i < 10; i++) {
		arr[shift + i] = temp_arr[len + i];
	}
	arr[shift + 10] = 0x00;
	/*
	arr[shift] = temp_arr[len];
	arr[shift + 1] = temp_arr[len + 1];
	arr[shift + 2] = temp_arr[len + 2];
	arr[shift + 3] = temp_arr[len + 3];
	arr[shift + 4] = temp_arr[len + 4];
	arr[shift + 5] = temp_arr[len + 5];
	arr[shift + 6] = temp_arr[len + 6];
	arr[shift + 7] = temp_arr[len + 7];
	arr[shift + 8] = temp_arr[len + 8];
	arr[shift + 9] = temp_arr[len + 9];
	arr[shift + 10] = 0x00;
	*/
	return arr;
}

void shiftLeftByOne(uint8_t *arr, size_t len) {
	//CHECK MSB. IF 0 SHIFT NEXT Byte to the righ 
	//IF 1 SHIFT BUT XOR TO GET A ONE
	for (int k = 0; k < len; k++) {
		arr[k] = arr[k] << 1;
		if ((k + 1) != len) {
			if (arr[k + 1] & mask(7)) {
				//printf(" K (%x) --> K + 1 (%x) mask is: %x\n", arr[k], arr[k + 1], arr[k + 1] & mask(7));
				arr[k] = arr[k] ^ 0x01;
			}
		}
	}
}

void shiftLeftByX(uint8_t *arr, int shift, size_t size) {
	//CHECK MSB. IF 0 SHIFT NEXT Byte to the righ 
	//IF 1 SHIFT BUT XOR TO GET A ONE
	for (int k = 0; k < size; k++) {
		arr[k] = arr[k] << shift;
		if ((k - 1) >= 0) {	
			arr[k - 1] = arr[k - 1] | arr[k] >> (8 - shift);
		}	
	}
}

int firstByte(uint8_t *arr) {

	int i = 0;
	while (arr[i] == 0x00) {
		i++;
	}
	return i;
}
 
void reduction20To10Bytes(uint8_t *arr20, uint8_t *arr10) {
	uint8_t temp;

	//ASSGIN FIRSTAKE FIRST 10 BYTES (79 bit
	//for (int i = 0; i < 10; i++) {
	//	temp[i] = arr20[i];
	//}
	//printhexa(temp, 10);
	int f_byte = firstByte(arr20);
	temp = arr20[f_byte];
	while (f_byte < 10) {
	
		arr20[f_byte] ^= temp;

		//SHOULD WE GET 
		arr20[f_byte + 8] ^= temp >> 6;
		arr20[f_byte + 9] ^= temp << 2;

		arr20[f_byte + 9]  ^= temp >> 7;
		arr20[f_byte + 10] ^= temp << 1;

		//printhexa(arr20, 20);

		f_byte = firstByte(arr20);
		//printf("%i\n", f_byte);
		temp = arr20[f_byte];
		//printf("%x\n", temp);
	}
	if (f_byte == 10) {

		temp &= 0x80;
		//printf("%x\n", temp);
		arr20[f_byte] ^= temp;

		//SHOULD WE GET 
		arr20[f_byte + 8] ^= temp >> 6;
		arr20[f_byte + 9] ^= temp << 2;

		arr20[f_byte + 9] ^= temp >> 7;
		arr20[f_byte + 10] ^= temp << 1;

		//printhexa(arr20, 20);
		//printf("%x\n", temp);
	}

	//XOR BY ITSELF

	//XOR BY SHIFT UNTIL ALPHA 9
	//	SHIFT by 8 bytes
	//	8e byte shift right by 6
	//9 byte shift left by 2
	//XOR BY SHIFT UNTIL Aplha 0

	/*
	for (int i = 0; i < 20; i+= 2) {
		temp = arr20[i];
		for (int j = 0; j < 3; j++) {
			//printf("Round %i\n", j);
			calculate(temp, pol_7, pol_1, pol_0, j);
			//printhexa(pol_7, 2);
			//printhexa(pol_1, 2);
			//printhexa(pol_0, 2);
			//printf("%x \n", arr20[i]);

			if (pol_7[0] == 0x00 && j == 1) {
				break;
			} else if (pol_7[0] == 0x00 && j == 2) {
				arr20[i + 1] ^= irr;
			}
			else 
			{
				for (int j = 0; j < 2; j++) {
					
					arr20[i + j] ^= pol_7[j];
					arr20[i + j] ^= pol_1[j];
					arr20[i + j] ^= pol_0[j];
				}
			}
			//printhexa(arr20, 20);
		}
	}
	*/
	for (int i = 0; i < 10; i++)
		arr10[i] = arr20[i + 10];

	printhexa(arr10, 10);
}

void inverseOrderOfBytes(uint8_t *b) {
	uint8_t temp[10];
	for (int i = 0; i < 10; i++) {
		temp[i] = b[i];
	} 
	
	for (int i = 0; i < 10; i++) {
		b[i] = temp[9 - i];
	}/*
	b[0] = temp[9];
	b[1] = temp[8];
	b[2] = temp[7];
	b[3] = temp[6];
	b[4] = temp[5];
	b[5] = temp[4];
	b[6] = temp[3];
	b[7] = temp[2];
	b[8] = temp[1];
	b[9] = temp[0];*/
}

void convert10To20Bytes(uint8_t *arr10, uint8_t *arr20) {
	for (int i = 0; i < 20; i++) {
		if (i < 10) {
			arr20[i] = 0x00;
		}
		else {
			arr20[i] = arr10[i - 10];
		}
	}
}

void convert1To2Bytes(uint8_t *arr1, uint8_t *arr2) {
	arr2[0] = 0x00;
	arr2[1] = arr1[0];
}

void compareArrays(uint8_t *a, uint8_t *b, size_t size) {
	//for (int i = 0; i < size; i++ ) {
	//	if ()
	
	//}

}

int getDegree(uint8_t *arr) {

	int j = firstByte(arr);
	int i = 7;
	while (!(arr[j] & mask(i))) {


		i--;
	}
	//if (arr[0] & mask(7))
		//return 80;
	//else
		return ((8 * (9 - j)) + i);
}

void getIrreductable(uint8_t *f) {
	for (int i = 0; i < 10; i++) {
		f[i] = 0x00;
	}
	f[0] = 0x80;
	f[8] = 0x02;
	f[9] = 0x01;
	printhexa(f, 10);
}

void multi(uint8_t *result, uint8_t *a, uint8_t *b) {
	for (int j = 0; j < getDegree(b); j++) {
		for (int i = 0; i < 10; i++) {
			result[i] = result[i] ^ (b[i] & mask(j));
		}
		shiftLeftByX(a, 1, 10);
	}
	
}

void addition(uint8_t *buff, uint8_t *a, uint8_t *b) {
	//Addition for each bytes. 
	for (int i = 0; i < 10; i++) {
		buff[i] = a[i] ^ b[i];
	}
}

void multiplication(uint8_t *buff, uint8_t *a, uint8_t *b) {
	
	//printhexa(a, 10);
	//printhexa(b, 10);
	//Inverse the order of byte, so that we go from LSB to MSB of B
	//printhexa(b, 10);

	uint8_t temp[20], shift_a_8[20], shift_a_1[20];
	for (int i = 0; i < 20; i++) {
			temp[i] = 0x00;
	}
	//ASSIGNE TEMPORARY VARIABLE AND CONVERT A TO 20 BYTES
	convert10To20Bytes(a, shift_a_1);
	//printhexa(shift_a_1, 20);
	/*if (b == 0x03 || b == 0x02) {
		printf("GRANFDEUR 1");
		for (int i = 0; i < 10; i++) {
			if (0x03 & mask(7)) {
				buff[i] = a[i] * 0x03;
			}
			else
				buff[i] = a[i] * 0x03;
		}
		printhexa(buff, 10);
		uint8_t temp_const[2];
		convert1To2Bytes(a, shift_a_1);
		for (int i = 0; i < 10; i++) {
			for (int k = 0; k < 2; k++) {
				temp_const[k] = temp_const[k] ^ ((b[0] & mask(k)) * shift_a_1[i]);
				shiftLeftByOne(shift_a_1, 2);
			}
		}
	}
	else {*/
		//ASSIGNE TEMPORARY VARIABLE AND CONVERT A TO 20 BYTES
		//convert10To20Bytes(a, shift_a_1);
		inverseOrderOfBytes(b);

		for (int j = 0; j < 8; j++) {
			//Assign shift by 1 to the A which will be shift by 8
			for (int k = 0; k < 20; k++) {
				shift_a_8[k] = shift_a_1[k];
			}
			///printf("-------\n");
			//printhexa(shift_a_8, 20);
			//Make the shifting and operation on the 10 bytes of A 
			for (int i = 0; i < 10; i++) {
				//Shift the the array A by 8 
				//Mask the byte i of B with the bit corresponding to the step (Give 0 or 1)
				//	Multiply by the shift array
				//	Addition with the temporariry array (which the result)
				//printhexa(shift_a_8, 20);
				//printf("value of mask %i, %x\n", j, (b[i] & mask(j)));
				//addition(temp, temp, shift_a_8);
				//printhexa(temp, 20);
				for (int k = 0; k < 20; k++) {
					temp[k] = temp[k] ^ (((b[i] & mask(j))) * shift_a_8[k]);
					//printf("value of mask i%, %x\n", j, mask(j));
					//temp[k] = temp[k] * shift_a_8[k];
				}
				//printhexa(temp, 20);

				shiftLeftByEight(shift_a_8, 10 - i);
			}
			shiftLeftByOne(shift_a_1, 20);
		}
		printf("A * B = \n");
		printhexa(temp, 20);
		//MODULO ReducTion by a irreticutble
		reduction20To10Bytes(temp, buff);
		//printhexa(buff, 10);
	//}
}

void square(uint8_t *buff, uint8_t *a) {
	//Square for each bytes. 
	uint16_t byte;
	uint8_t temp[20], msb, lsb;
	uint8_t pos_line, pos_col;
	for (int i = 0; i < 20; i+=2) {
		pos_line = (a[i/2] & 0xF0) >> 4;
		pos_col = (a[i/2] & 0x0F);
		//printf("%x %x\n", pos_line, pos_col);
		byte = square_table[(16 * pos_line) + pos_col];
		msb = byte >> 8;
		lsb = byte ;
		temp[i] = msb;
		temp[i + 1] = lsb;
		//printf("%x\n", byte);
		//printf("%x %x\n", temp[i], temp[i+1]);
	}
	printhexa(temp, 20);
	reduction20To10Bytes(temp, buff);
	printhexa(buff, 10);
}

void division(uint8_t *a, uint8_t *f, uint8_t *r, uint8_t *q ) {
	uint8_t temp[10];
	int size = 10;
	for (int i = 0; i < size; i++) {
		temp[i] = 0x00;
	}
	temp[9] = 0x01;

	//printhexa(temp_a, size);
	//printhexa(a,10);
	//printf("%i\n", getDegree(a));

	int diff;
	while (getDegree(f) >= getDegree(a)) {		//DEG F >= DEG A
		diff = getDegree(f) - getDegree(a);
		shiftLeftByX(a, diff, size);
		//printhexa(a, size);
		addition(r, a, f);
		//printhexa(r, size);
		if (getDegree(r) >= getDegree(a)) {
			shiftLeftByX(q, getDegree(f) - getDegree(r), size);
			q[9] ^= 0x01;
			//printf("IF:   "); printhexa(q, size);
		}
		else {
			//printf("%i\n", getDegree(f) - getDegree(a));
			shiftLeftByX(q, diff, size);
			//printf("Else:   "); printhexa(q, size);
		}

		//printhexa(q, size);
		for (int i = 0; i < size; i++) {
			f[i] = r[i];
		}
	}
}

void inverse(uint8_t *buff, uint8_t *a) {

	uint8_t temp_a;
	uint8_t r[10], q[10], f[10],t[10], s[10];
	int size = 10;

	getIrreductable(f);
	for (int i = 0; i < size; i++) {
		r[i] = q[i] = t[i] = s[i] = 0x00;
		if (i == 9) {
			r[i] = q[i] = s[i] = 0x01;
		}
	}

	do {

		printf("\nA      :  ");
		printhexa(a, size);
		printf("IRREDUC:  ");
		printhexa(f, size);
		printf("REMINDER: ");
		printhexa(r, size);
		printf("QUOTION:  ");
		printhexa(q, size);
		printf("T      :  ");
		printhexa(t, size);
		printf("S:        ");
		printhexa(s, size);
		division(a, f, r, q);



		for (int i = 0; i < size; i++) {
			f[i] = a[i];
			a[i] = r[i];
			t[i] = s[i];

			//for (int j = 0; j < size; j++) 
			int j = size - 1;
			do {
				printf("%x ", q[j]);
				s[j] ^= (q[j] * s[j]);
				j--;
			} while ((q[j] * s[j]) & 0xFF); 

			/*for (int j = size - 1; j >= 0; j--) {
				printf("%x ", q[j]);
				s[j] = (q[j] * s[j]);


			}
			/*if ((q[i] * s[i]) & 0xFF) {
				
				//if (s[i - 1] & 0xFF) {
					s[i - 1] = (q[i - 1] * s[i ]);
					s[i - 1] ^= (q[i] * s[i] & 0xFF) >> 4;
				/*}
				else {
					s[i - 1] = (q[i - 1] * s[i]);
					s[i - 1] ^= (q[i] * s[i] & 0xFF) >> 4;
				}*/
				//s[i] = (q[i - 1] * s[i - 1]);
			//}
			//s[i] = t[i] ^ (q[i] * s[i]);
		}
		//multiplication(s, s, q);
		//addition(s, s, t);
		//printf("%x", r[9]);
	
	
	} while (r[9] != 0x00);


}

void getLambda(uint8_t *buff, uint8_t *x_p, uint8_t *y_p, uint8_t *x_q, uint8_t * y_q) {
	
	uint8_t temp_sup[10], temp_div[10], temp_inv;
	if (x_p == x_q && y_p == y_q) {
		//Do something
		printf("EQUAL\n");
		printhexa(x_q, 10);
		multiplication(temp_sup, x_q, 0x03);
		printhexa(temp_sup, 10);
	}
	else {
		addition(temp_sup, y_p, y_q);
		addition(temp_div, x_p, x_q);
		//inverse(temp_inv, temp_div);
		//multiplication(buff, temp_sup, temp_inv);

		printf("NOT EQUAL\n");
	}

}

void elliptic_curve() {
	//compute_square_table();
	
	//printhexa(A, 10);
	//shiftLeftByEight(A, 20);
	
	//printhexa(A);
	//printhexa(B);
	/*uint8_t q[10], s[10];
	for (int i = 0; i < 10; i++) {
		q[i] = s[i] = 0x00;
		if (i == 9) {
			q[i] = 0x10;
			s[i] = 0x03;
		}
	}*/
	uint8_t C[10];
	//printhexa(q, 10);
	//printhexa(s, 10);
	//multi(C, q, s);
	//C[9] = q[9] * s[9];
	//printhexa(C, 10);*/
	//addition(C, A, B);
	//printhexa(C);
	//division(A, C);
	inverse(C, A);
	//multiplication(C, A, B);
	//square(C, A);
	//getLambda(C, x_P, y_P, x_P, y_P);
}





int main() {

	elliptic_curve();
	return 0;
}