#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
#define SIZE	4
#define ZERO	0
#define ONE			1
#define TWO 		2
#define NULL		'\0'
#define THREE		3
#define FOUR		4
#define FIVE		5
#define SIX			6
#define FIFTEEN		15
#define SIXTEEN		16
#define SEVENTEEN	17
#define MAX			2048
#define TWO_DIGIT	10
#define THREE_DIGIT	100
#define FOUR_DIGIT	1000
int makeRandomLocation(int location[][SIZE], char locationStatus[SEVENTEEN]);
void refreshLocationStatus(int location[][SIZE], char locationStatus[SEVENTEEN]);
int endTestingFunction(int location[][SIZE],char locationStatus[SEVENTEEN]);
int doUserChoice(int endCondition, int location[][SIZE], char locationStatus[SEVENTEEN]);
void printTable(int location[][SIZE], int *score);
int upMovement(int location[][SIZE], int *score);
int downMovement(int location[][SIZE], int *score);
int leftMovement(int location[][SIZE], int *score);
int rightMovement(int location[][SIZE], int *score);
int menu(void);
int pauseMenu(void);