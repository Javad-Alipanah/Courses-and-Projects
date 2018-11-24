#include "All_headers.h"
int downMovement(int location[][SIZE], int *score){
    int i, j, m, flag = ZERO;
    for(j = ZERO; j < SIZE; j++){ /* Slides locations */
        m = SIZE - ONE;
        for(i = SIZE - ONE; i >= ZERO; i--){
            if(location[i][j] != ZERO){
                location[m][j] = location[i][j];
                if(m != i){
                    location [i][j] = ZERO;
					flag = ONE;
				}
                m--;
            }
        }
    }
   	for(i = SIZE - ONE; i >= ONE; i--) /* Merges equal locations */
		for(j = ZERO; j < SIZE; j++)
			if(location[i][j] == location[i - ONE][j]){
				location[i][j] += location[i - ONE][j];
				*score += location[i - ONE][j] * TWO;
				location[i - ONE][j] = ZERO;
				if(location[i][j] != ZERO)
					flag = ONE;
			}
    for(j = ZERO; j < SIZE; j++){ /* slides locations again */
        m = SIZE - ONE;
        for(i = SIZE - ONE; i >= ZERO; i--){
            if(location[i][j] != ZERO){
                location[m][j] = location[i][j];
                if(m != i)
                    location [i][j] = ZERO;
                m--;
            }
        }
    }
	return flag;
}