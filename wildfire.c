#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "wildfire.h"
#include "display.h"
#include <getopt.h>

#define _BSD_SOURCE
#define SCHLEEP 1000000
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/**
	Prints the usage message
**/
void usage(){
	fprintf(stderr, "usage: wildfire [-pN] size probability treeDensity proportionBurning\nThe -pN option tells the simulation to print N cycles and stop.\nThe probability is the probability a tree will catch fire.\n");
}

/**
	Runs the simulation for Cursor Controlled Output
**/
void runSimulation(int size, int probTree, int probBurning, int probCatch){
	//initalize vars
	int iteration = 0;
	int changedElements = 0;
	int totalChanged = 0;
	float probCatchPrint = (float) probCatch / 100;
	float probTreePrint = (float)probTree / 100;
	float probBurningPrint = (float)probBurning / 100;
	char forest[size][size];
	generateForest(size, forest, probTree, probBurning);	//Initialize random forest

	
	//Inital Print
	printForest(size, forest);
	printf("cycle %d, size %d, probability %.2f, density %.2f, proportion %.2f, changes %d\n", 
		iteration, size, probCatchPrint, probTreePrint, probBurningPrint, changedElements);
	iteration++;
	
	//Control Simulation
	while(burning(size, forest)){
		clear();
		set_cur_pos(1,1);
		changedElements = burnForest(size, forest, probCatch);	//Burn forest once and store changed elements
		totalChanged += changedElements;			//Iterate total changed elements
		printForest(size, forest);
		printf("cycle %d, size %d, probability %.2f, density %.2f, proportion %.2f, changes %d\n", 
			iteration, size, probCatchPrint, probTreePrint, probBurningPrint, changedElements);
		iteration++;
		usleep(SCHLEEP);	//Delay
	}

	//Final print
	set_cur_pos(1,1);
	printForest(size, forest);
	printf("cycle %d, size %d, probability %.2f, density %.2f, proportion %.2f, changes %d\n", 
		iteration, size, probCatchPrint, probTreePrint, probBurningPrint, changedElements);
	printf("fires are out after %d cumulative changes.\n", totalChanged);
}

/**
	Runs the simulation for Printed Output
**/
void runSimulationPrint(int size, int probTree, int probBurning, int probCatch, int maxIterations){
	//Initialize vars
	int iteration = 0;
	int changedElements = 0;
	int totalChanged = 0;
	float probCatchPrint = (float) probCatch / 100;
	float probTreePrint = (float)probTree / 100;
	float probBurningPrint = (float)probBurning / 100;
	char forest[size][size];
	generateForest(size, forest, probTree, probBurning);		//Initialize random forest

	//Initial Print	
	printForest(size, forest);
	printf("cycle %d, size %d, probability %.2f, density %.2f, proportion %.2f, changes %d\n", 
		iteration, size, probCatchPrint, probTreePrint, probBurningPrint, changedElements);
	iteration++;
	
	//Control simulation
	while(burning(size, forest) && (iteration < maxIterations)){
		changedElements = burnForest(size, forest, probCatch);
		totalChanged += changedElements;
		printForest(size, forest);
		printf("cycle %d, size %d, probability %.2f, density %.2f, proportion %.2f, changes %d\n", 
			iteration, size, probCatchPrint, probTreePrint, probBurningPrint, changedElements);
		iteration++;	
		usleep(SCHLEEP);		//Delay
	}
	
	//Print final forest
	printForest(size, forest);
	printf("cycle %d, size %d, probability %.2f, density %.2f, proportion %.2f, changes %d\n", 
		iteration, size, probCatchPrint, probTreePrint, probBurningPrint, changedElements);
	printf("fires are out after %d cumulative changes.", totalChanged);
	iteration++;
}	

int main(int argc, char **argv){
	
	//Initialize parameters for forest
	int size;
	int probTree;
	int probBurning;	
	int probCatch;
	char opt;
	int maxIterations = -1;
	
	//If there are command line arguments present
	if((opt = getopt(argc, argv, "p:")) != -1){				
		if(argc == 6){	//A valid amount of arguments were passed in
			
			//Get command line arguments if they're valid, break otherwise
			switch (opt) {
				case 'p':
					maxIterations = (int)strtol(optarg, NULL, 10);
					break;
				default:
					fprintf(stderr, "The -pN option was invalid\n");
					return -1;
			}
			
			//Checks if maxIterations is a valid number (nonegative)
			if(maxIterations < 0){
				fprintf(stderr, "The -pN value was negative");
				return -1;
			}
			
			//Get size and determine if it is a valid input
			size = (int)strtol(argv[2], NULL, 10); 
			if(size < 5  || size > 40){
				fprintf(stderr, "The size (%d) must be an integer in [5...40].\n", size);
				usage();
				return -1;
			}
			
			//Get Probability and determine if it is a valid input
			probCatch = (int)strtol(argv[3], NULL, 10);
			if(probCatch < 0 || probCatch > 100){
				fprintf(stderr, "The probability (%d) must be an integer in [0...100].\n", probCatch);
				usage();
				return -1;
			}
			
			//Get Density and deterine if it is a valid input
			probTree = (int)strtol(argv[4], NULL, 10); 
			if(probTree < 0 || probTree > 100){
				fprintf(stderr, "The density (%d) must be an integer in [0...100].\n", probTree);
				usage();
				return -1;
			}

			//Get Proportion and determine if it is a valid input			
			probBurning = (int)strtol(argv[5], NULL, 10); 
			if(probBurning < 0 || probBurning > 100){
				fprintf(stderr, "The proportion (%d) must be an integer in [0...100].\n", probBurning);
				usage();
				return -1;
			}
			
			runSimulationPrint(size, probTree, probBurning, probCatch, maxIterations);

			return 0;	//End
		}
		else{		//Not enough arguments were passed in
			usage();	//Print usage args
			return -1;	//End
		}		
	}
	//If there are not command line arguments present
	else{
		if(argc == 5){		//Valid number of Command Line Args
			//Get Size and find out if it is a valid input
			size = (int)strtol(argv[1], NULL, 10); 
			if(size < 5  || size > 40){
				fprintf(stderr, "The size (%d) must be an integer in [5...40].\n", size);
				usage();
				return -1;
			}
			
			//Get Probability and find out if it is a valid input
			probCatch = (int)strtol(argv[2], NULL, 10);
			if(probCatch < 0 || probCatch > 100){
				fprintf(stderr, "The probability (%d) must be an integer in [0...100].\n", probCatch);
				usage();
				return -1;
			}

			//Get Density and find out if it is a valid input
			probTree = (int)strtol(argv[3], NULL, 10); 
			if(probTree < 0 || probTree > 100){
				fprintf(stderr, "The density (%d) must be an integer in [0...100].\n", probTree);
				usage();
				return -1;
			}
			
			//Get Proportion and find out if it is a valid input
			probBurning = (int)strtol(argv[4], NULL, 10); 
			if(probBurning < 0 || probBurning > 100){
				fprintf(stderr, "The proportion (%d) must be an integer in [0...100].\n", probBurning);
				usage();
				return -1;
			}	

 			runSimulation(size, probTree, probBurning, probCatch);
			return 0;	//End
		}
		else{			//Not a valid number of Command Line Arguments
			usage();	//Print usage args
			return -1;	//End
		}	
	}
}
