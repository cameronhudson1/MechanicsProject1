#ifndef WILDFIRE_H_ //Include guard
#define WILDFIRE_H_

#define BURNEDTREE '_'
#define LIVINGTREE 'Y'
#define FIRE '*'
#define EMPTY ' '
#define HEIGHT 20
#define WIDTH 20

void generateForest(int size, char forest[size][size], int probTree, int probBurning);
void printForest(int size, char forest[size][size]); 
int burnForest(int size, char forest[size][size], int probCatch);
int burning(int size, char forest[size][size]);

#endif 
