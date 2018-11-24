#include "All_headers.h"
void printTable(int location[][SIZE], int *score){
	int i, j;
	printf("\n          Score: %d\n", *score);
	for(i = ZERO; i < SIZE; i++){
		if(i == ZERO)
		printf(" ____ ____ ____ ____ \n");
		else
			printf("|____|____|____|____| \n");
		printf("|    |    |    |    |\n|");
		for(j = ZERO; j < SIZE; j++)
			if(location[i][j] < TWO_DIGIT && location[i][j] != ZERO)
				printf(" %d  |",location[i][j]);
			else if(location[i][j] < TWO_DIGIT && location[i][j] == ZERO)
				printf("    |",location[i][j]);
			else if(location[i][j] > TWO_DIGIT && location[i][j] < THREE_DIGIT)
				printf(" %d |",location[i][j]);
			else if(location[i][j] > THREE_DIGIT && location[i][j] < FOUR_DIGIT)
				printf("%d |",location[i][j]);
			else
				printf("%d|",location[i][j]);
		printf("\n");
	}
	printf("|____|____|____|____|\n");
}