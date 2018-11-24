#include "All_headers.h"
int makeRandomLocation(int location[][SIZE], char locationStatus[SEVENTEEN]){
	int random, i, j;
	char sign;
	random = rand() % TWO + ONE;
	if(random == ONE)
		random *= TWO;
	else{
		random = rand() % TWO + ONE;
		random *= TWO;
	}
		if (strlen(locationStatus) != ZERO){
		sign = locationStatus[rand() % strlen(locationStatus)]; /* Chooses an element of string that is not NULL */
		for(i = ZERO; i < SIZE; i++)
			for(j = ZERO; j < SIZE; j++){
				if(sign == ('A' + ((SIZE * i) + j))){ /* Decodes choosen element which is a sign of one location */
					location[i][j] = random;
					return i * SIZE + j;
				}
			}
	}
}
