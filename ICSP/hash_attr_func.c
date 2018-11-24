#include "dma.h"
#include "common_header.h"
#define THREE	3
#define EIGHT	8
int hash_attr_func(char *key) {
	
	int mod = MAX_NUM_OF_ATTRS;
	int sub_size, i, modulus = ZERO, temp = ZERO;
	
	sub_size = strlen(key) % THREE + TWO;
	
	for(i = ZERO; i < strlen(key); i++)	{
		temp += key[i];
		if( !((i + ONE) % sub_size == ZERO || i == strlen(key)- ONE) )
			temp <<= EIGHT;
		else {
			modulus += temp % mod;
			temp = ZERO;
		}
	}
	
	modulus %= mod;
	return modulus;

}