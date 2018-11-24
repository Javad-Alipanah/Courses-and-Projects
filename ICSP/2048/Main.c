#include "All_headers.h"
int main(){
	int location[SIZE][SIZE] = {{ZERO},{ZERO}}, flag;
	char locationStatus[SEVENTEEN] = {ZERO};
	int endCondition = endTestingFunction(location, locationStatus);
	while(ONE){ /* An infinit loop that continues until end of game or choice of exit by user */
		flag = menu(); /* If user choose "start" flag is ONE and game starts; else if exit is choosen flag is ZERO and game ends */
		if(flag)
			flag = doUserChoice(endCondition, location, locationStatus);
		else{
			printf("\n\tHave a good time...\n\n");
			break;
		}
		if(flag == MAX){ /* MAX is 2048 */
			printf("\n\tYOU WIN!\n\n");
			continue;
		}
		else if(flag == ZERO){
			printf("\n\t\t\t\tGAME OVER!\n\n");
			continue;
		}
		else if(flag == TWO) /* When user types "exit" doUserChoice function returns TWO */
			break;
	}
		return ZERO;
}
