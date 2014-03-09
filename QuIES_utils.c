#include "QuIES_utils.h"

unsigned char getHigh(uint_fast16_t int16) {
	unsigned char aux;
	aux  = (unsigned char) ((int16 >> 8) & 0xFF);
	return aux;
}

unsigned char getLow(uint_fast16_t int16) {
	unsigned char aux;
	aux  = (unsigned char) (int16 & 0xFF);
	return aux;
}
