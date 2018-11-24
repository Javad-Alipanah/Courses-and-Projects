#include "All_headers.h"
int rightMovement(int location[][SIZE], int *score){
    int i, j, m, flag = ZERO;
    for(i = ZERO; i < SIZE; i++){ /* Slides locations */
        m = SIZE - ONE;
        for(j = SIZE - ONE; j >= ZERO; j--){
            if(location[i][j] != ZERO){
                location[i][m] = location[i][j];
                if(m != j){
					flag = ONE;
                    location [i][j] = ZERO;
				}
                m--;
            }
        }
    }
	for(j = SIZE - ONE; j >= ONE; j--) /* Merges equal locations */
		for(i = ZERO; i < SIZE; i++)
			if(location[i][j] == location[i][j - ONE]){
				location[i][j] += location[i][j - ONE];
				*score += location[i][j - ONE] * TWO;
				location[i][j - ONE] = ZERO;
				if(location[i][j] != ZERO)
					flag = ONE;
			}
    for(i = ZERO; i < SIZE; i++){ /* slides locations again */
        m = SIZE - ONE;
        for(j = SIZE - ONE; j >= ZERO; j--){
            if(location[i][j] != ZERO){
                location[i][m] = location[i][j];
                if(m != j)
                    location [i][j] = ZERO;
                m--;
            }
        }
    }
	return flag;
}