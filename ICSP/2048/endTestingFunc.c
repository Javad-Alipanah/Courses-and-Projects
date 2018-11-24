#include "All_headers.h"
int endTestingFunction(int location[][SIZE],char locationStatus[SEVENTEEN]){
	int i, j, flag = ONE;
	if(strlen(locationStatus) == ZERO){
		flag = ZERO; /* No free location exists */
	}
	if(flag == ZERO){ /* If there is no free location we must check whether game ended or not */
		for(i = ZERO; i < SIZE - ONE; i++){
			for(j = ZERO; j < SIZE; j++)
				if(location[i][j] == location[i + ONE][j]){
					flag = TWO; /* UP & DOWN are valid */
					break;
				}
			if(flag == TWO)
				break;
		}
		for(i = ZERO; i < SIZE - ONE; i++){
			for(j = ZERO; j < SIZE; j++)
				if(location[j][i] == location[j][i + ONE]){
					flag += THREE; /* LEFT & RIGHT are valid */
					break;
				}
			if(flag == FIVE || flag == THREE)
				break;
		}
	}
	for(i = ZERO; i < SIZE; i++){
		for(j = ZERO; j < SIZE; j++)
			if(location[i][j] == MAX){
				flag = MAX; /* Game ended with reaching 2048 */
				break;
			}
		if(flag == MAX)
			break;
	}
	return flag;
}
