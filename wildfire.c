#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "wildfire.h"
#include "display.h"
#include <getopt.h>

#define _BSD_SOURCE
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void usage(){
	printf("usage: wildfire [-pN] size probability treeDensity proportionBurning\n");
	printf("The -pN option tells the simulation to print N cycles and stop.\n");
	printf("The probability is the probability a tree will catch fire.\n");
}

int getSize(char **argv){
	int i = 0;
	while(argv[i] != NULL){
		i++;
	}
	return i;
}

int main(int argc, char **argv){
	
	int size;
	int probTree;
	int probBurning;	
	int probCatch;
	int iteration = 0;
	int changedElements;
	char opt;
	int maxIterations = -1;
	
	if((opt = getopt(argc, argv, "p:")) != -1){
		if(argc == 6){
			switch (opt) {
				case 'p':
					maxIterations = (int)strtol(optarg, NULL, 10);
					break;
				default:
					printf("The -pN option was invalid!\n");
					usage();
					return -1;
			}
			
			if(maxIterations < 0){
				printf("The -pN value cannot benegative!");
				return -1;
			}
			
			size = (int)strtol(argv[2], NULL, 10); 
			if(size < 5  || size > 40){
				 printf("Size %d must be an integer in[5, 40].\n", size);
				return -1;
			}

			probCatch = (int)strtol(argv[3], NULL, 10);
			if(probCatch < 0 || probCatch > 100){
				printf("Probability %d must be between [0, 100].\n", probCatch);
				return -1;
			}

			probTree = (int)strtol(argv[4], NULL, 10); 
			if(probTree < 0 || probTree > 100){
				printf("Density %d must be between [0, 100].\n", probTree);
				return -1;
			}
			
			probBurning = (int)strtol(argv[5], NULL, 10); 
			if(probBurning < 0 || probCatch > 100){
				printf("Proportion %d must be between [0, 100].\n", probBurning);
				return -1;
			}
			
			char forest[size][size];
			generateForest(size, forest, probTree, probBurning);
		
			while(burning(size, forest) && (iteration < maxIterations)){
				printForest(size, forest);
				printf("Cycle: %d, size: %d, probability: 0.%d, density: 0.%d, proportion: 0.%d, Changed: %d, maximumCycles: %d\n", 
					iteration, size, probCatch, probTree, probBurning, changedElements, maxIterations);
				iteration++;
				changedElements = burnForest(size, forest, probCatch);
				usleep(1000000);
			}
			printForest(size, forest);
			printf("Cycle: %d, size: %d, probability: 0.%d, density: 0.%d, proportion: 0.%d, Changed: %d, maximumCycles: %d\n", 
				iteration, size, probCatch, probTree, probBurning, changedElements, maxIterations);
			return 0;
		}
		else{
			printf("Not enough program arguments!  Need 6, have %d\n", argc);
			return -1;
		}		
	}
	else{
		if(argc == 5){
			size = (int)strtol(argv[1], NULL, 10); 
			if(size < 5  || size > 40){
				 printf("Size %d must be an integer in[5, 40].\n", size);
				return -1;
			}

			probCatch = (int)strtol(argv[2], NULL, 10);
			if(probCatch < 0 || probCatch > 100){
				printf("Probability %d must be between [0, 100].\n", probCatch);
				return -1;
			}

			probTree = (int)strtol(argv[3], NULL, 10); 
			if(probTree < 0 || probTree > 100){
				printf("Density %d must be between [0, 100].\n", probTree);
				return -1;
			}
			
			probBurning = (int)strtol(argv[4], NULL, 10); 
			if(probBurning < 0 || probCatch > 100){
				printf("Proportion %d must be between [0, 100].\n", probBurning);
				return -1;
			}	

			char forest[size][size];
			generateForest(size, forest, probTree, probBurning);
		
			clear();
			while(burning(size, forest)){
				set_cur_pos(1,1);
				printForest(size, forest);
				printf("Cycle: %d, size: %d, probability: 0.%d, density: 0.%d, proportion: 0.%d, Changed: %d\n", 
					iteration, size, probCatch, probTree, 
					probBurning, changedElements);
				iteration++;
				changedElements = burnForest(size, forest, probCatch);
				usleep(1000000);
			}
			set_cur_pos(1,1);
			printForest(size, forest);
			printf("Cycle: %d, size: %d, probability: 0.%d, density: 0.%d, proportion: 0.%d, Changed: %d\n", 
				iteration, size, probCatch, probTree, probBurning, changedElements);
			return 0;
		}
		else{
			printf("Not enough program arguments!  Need 5, have %d\n", argc);
			return -1;
		}	
	}
}
