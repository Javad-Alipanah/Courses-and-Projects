#include "dma.h"
#include "common_header.h"
#define THREE	3
#define EIGHT	8
int ds_hash_func(char *key, int mod) {
	
	int sub_size, i, modulus = ZERO, temp = ZERO;
	
	sub_size = strlen(key) % THREE + TWO; /* For making sub_size dynamic (from two to four) */
	
	for(i = ZERO; i < strlen(key); i++)	{
		temp += key[i];
		if( !((i + ONE) % sub_size == ZERO || i == strlen(key)- ONE) )
			temp <<= EIGHT; /* Reading sub_string(s) from string in a way that its (their) siaze is equal to sub_size */
		else{
			modulus += temp % mod;
			temp = ZERO;
		}
	
	}
	modulus %= mod; /* To annihilate probability of returning a number that is more than mod */
	return modulus;

}