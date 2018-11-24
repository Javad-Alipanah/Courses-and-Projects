#include "All_Headers.h"
void refreshLocationStatus(int location[][SIZE], char locationStatus[SEVENTEEN]){
	int k, i, j;
	char sign;
	sign = 'A';
	k = ZERO;
	for(i = ZERO; i <= SIXTEEN; i++)
		locationStatus[i] = NULL;
	for(i = ZERO; i < SIZE; i++)
		for(j= ZERO; j < SIZE; j++){
			sign = ('A' + ((SIZE * i) + j)); /* Encodes free location specifications as an alphabetical element */
			if(location[i][j] == ZERO){
				locationStatus[k] = sign;
				k += ONE;
			}
		}
}