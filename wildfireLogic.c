/*
	FILE: 	WildfireAssets
	AUTHOR: Cameron E Hudson (ceh7606)
	DESCRIPTION:
		A FIle that contains the asset files which make the Wildfire Simulation
		tool work.
*/
#include <stdio.h>
#include <stdlib.h>
#include "wildfire.h"

#define EMPTY ' '
#define LIVINGTREE 'Y'
#define BURNEDTREE '_'
#define BURNING	'*'
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_GRAY    "\x1b[90m"
#define COLOR_RESET   "\x1b[0m"

int generateRandInt(){
	int randPaul = rand();
	randPaul = randPaul % 100;
	return randPaul;
}

void printForest(int size, char forest[size][size]){
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			if(forest[i][j] == BURNING){
				printf(COLOR_RED "%c" COLOR_RESET, forest[i][j]);

			}		
			if(forest[i][j] == LIVINGTREE){
				printf(COLOR_GREEN "%c" COLOR_RESET, forest[i][j]);

			}
			if(forest[i][j] == BURNEDTREE){
				printf(COLOR_GRAY "%c" COLOR_RESET, forest[i][j]);

			}
			if(forest[i][j] == EMPTY){
				printf("%c", forest[i][j]);

			}
				
			
		}
		printf("\n");
	}
}

void copyForest(int size, char forestOrig[size][size], char  forestCpy[size][size]){
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			forestCpy[i][j] = forestOrig[i][j];
		}
	}
}

void  generateForest(int size, char forest[size][size], int probTree, int probBurning){
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			int randInt1 = generateRandInt();
			if(randInt1 < probTree){			//Probability tree exists
				int randInt2 = generateRandInt();	
				if(randInt2 < probBurning){		//Probability tree is burning
					forest[i][j] = BURNING;
				}
				else{
					forest[i][j] = LIVINGTREE;
				}
			}
			else{
				forest[i][j] = EMPTY;
			}
		}
	}
}

/**
	Returns true if a burning tree exists in the forest
	False otherwise
**/
int  burning(int size, char forest[size][size]){
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			if(forest[i][j] == '*'){
				return 1;
			}
		}
	}
	return 0;
}

/**
	Determines wether to burn this space or not
**/
int applySpread(int size, char forest[size][size], int i, int j, int  probCatch){	
	int randInt = generateRandInt();
	if(randInt < probCatch){
		forest[i][j] = BURNING;
		return 1;
	}
	else{
		forest[i][j] = LIVINGTREE;
		return 0;
	}
	
}

/**
	Determines if any neighbors of forest[row][col] are burning
**/
int findBurningNeighbor(int size, char forest[size][size], int row, int col){
        int currRow, currCol;
	
	//Iterate through values
        for(int i = -1; i < 2; i++){
                for(int j = -1; j < 2; j++){
                        currRow = row+i;
                        currCol = col+j;
			
			//Ignore neighbor is out of bounds
                        if(currRow < 0){
                                break;
                        }
                        if(currRow > size-1){
                                break;
                        }
                        if(currCol < 0){
                                break;
                        }
                        if(currCol > size-1){
                                break;
                        }
			
			//Add fireNeighbor if a '*' is found
                        if(forest[currRow][currCol] == BURNING && (row != currRow || col != currCol)){
                        	return 1;	
                        }
                }
        }
        return 0;
}



/**
	Method to simulate the burning of the forest given a generated forest
**/
int burnForest(int size, char forest[size][size], int  probCatch){
	int changedElements = 0;
	char tempForest[size][size];
	copyForest(size, forest, tempForest);
	
	for(int i =0; i < size; i++){
		for(int j = 0; j < size; j++){
			if(tempForest[i][j] == LIVINGTREE && findBurningNeighbor(size, tempForest, i, j)){
				//Space is good for burning, if the probability works out
				if(applySpread(size, forest, i, j, probCatch)){
					changedElements++;
				}
			}
			if(tempForest[i][j] == BURNING){
				forest[i][j] = BURNEDTREE;
				changedElements++;
			}
		}
	}
	return changedElements;
}
