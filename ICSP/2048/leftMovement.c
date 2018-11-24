#include "All_headers.h"
int leftMovement(int location[][SIZE], int *score){
    int i, j, m, flag = ZERO;
    for(i = ZERO; i < SIZE; i++){ /* Slides locations */
        m = ZERO;
        for(j = ZERO; j < SIZE; j++){
            if(location[i][j] != ZERO){
                location[i][m] = location[i][j];
                if(m != j){
                    location [i][j] = ZERO;
					flag = ONE;
				}
                m++;
            }
        }
    }
	for(j = ZERO; j < SIZE - ONE; j++) /* Merges equal locations */
		for(i = ZERO; i < SIZE; i++)
			if(location[i][j] == location[i][j + ONE]){
				location[i][j] += location[i][j + ONE];
				*score += location[i][j + ONE] * TWO;
				location[i][j + ONE] = ZERO;
				if(location[i][j] != ZERO)
					flag = ONE;
			}
    for(i = ZERO; i < SIZE; i++){ /* slides locations again */
        m = ZERO;
        for(j = ZERO; j < SIZE; j++){
            if(location[i][j] != ZERO){
                location[i][m] = location[i][j];
                if(m != j)
                    location [i][j] = ZERO;
                m++;
            }
        }
    }
    return flag;
}
