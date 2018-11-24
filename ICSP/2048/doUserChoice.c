#include "All_headers.h"
int doUserChoice(int endCondition, int location[][SIZE], char locationStatus[SEVENTEEN]){
	int i = ZERO, j = ZERO, k = ONE, movement, scoreTemp = ZERO, flag, temp, restartFlag = ONE;
	int *score = &scoreTemp;
	char userChoice[FIFTEEN];
	srand(time(NULL));
	while(restartFlag){ /* If restart is choosen this loop runs once more */
		restartFlag = ZERO;
		for(i = ZERO; i < TWO; i++){           /* For making initial random numbers */
			refreshLocationStatus(location, locationStatus);
			temp = makeRandomLocation(location, locationStatus);
			location[(temp - temp%SIZE)/SIZE][temp%SIZE] = TWO;
		}
		while(endCondition != ZERO && endCondition != MAX){ /* Game body that does user choices */
			printTable(location, score);
			printf("Your choice: ");
			scanf("%s", userChoice);
			if(!strcmp(userChoice , "up"))
				movement = upMovement(location, score);
			else if(!strcmp(userChoice , "down"))
				movement = downMovement(location, score);
			else if(!strcmp(userChoice , "left"))
				movement = leftMovement(location, score);
			else if(!strcmp(userChoice , "right"))
				movement = rightMovement(location, score);
			else if(!strcmp(userChoice , "pause")){
				flag = pauseMenu(); /* If user choose "start" flag is ONE and game continues, If exit is choosen flag is ZERO and game ends and if restart is choosen flag is TWO and a new game starts */
				if(flag == ONE)
					continue;
				else if(flag == TWO){
					for(i = ZERO; i < SIZE; i++)
						for(j = ZERO; j < SIZE; j++)
							location[i][j] = ZERO;
					refreshLocationStatus(location, locationStatus);
					*score = ZERO;
					restartFlag = ONE;
					break;
				}
				else if(flag == ZERO){
					printf("\n\tHave a good time...\n\n");
					return TWO;
				}
			}
			else if(!strcmp(userChoice , "exit")){
				printf("\n\tHave a good time...\n\n");
				return TWO;
			}
			else{
				printf("\nWrong input!\n");
				continue;
			}
			refreshLocationStatus(location, locationStatus);
			if(!movement && endTestingFunction(location, locationStatus) != ZERO) /* If there wasn't any movement and game doesn't ended we must prevent from making random numbers */
				continue;
			makeRandomLocation(location, locationStatus);
			refreshLocationStatus(location, locationStatus);
			endCondition = endTestingFunction(location, locationStatus);
		}
	}
	printTable(location, score);
	return endCondition;
}