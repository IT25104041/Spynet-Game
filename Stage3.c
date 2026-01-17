#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//structure for player
typedef struct{
	char symbol;
	int intel;
	int lives;
	int row;
	int col;
}Player;

void dispGrid(char **grid, int N);
void playerPosition(Player *p, char symbol, char **grid, int N);
void objectPosition(char **grid, int N);
void playerMovement(Player *p, char **grid, int N);

int main(){
	
	Player p1;

	//seed a random value
	srand(time(0));

	int N;


	//condition to valid grid input size
	do{
		printf("Enter grid size(5-15) : ");
		scanf("%d",&N);

		if(N<5 || N>15)
			printf("Invalid grid size\n");
			
	}while(N<5 || N>15);


	
	//allocate memory for the entire grid
	char**grid = malloc(N*sizeof(char*));
	
	if(grid == NULL){
		printf("Memory allocation error.");
		return 1;
	}

	//allocate memory for each row
	for(int i=0; i<N; i++){
		grid[i] = malloc(N * sizeof(char));
		
		if(grid[i] == NULL){
			printf("Memory allocation error.");
			return 1;
		}
	}

	//initialize grid
	for(int i=0;i<N;i++){
		for(int j=0; j<N;j++)
			grid[i][j] = '.';
	
	}


	playerPosition(&p1, '@', grid, N);
	objectPosition(grid, N);

	//Loop to play
    	while(1){
	
		printf("Lives: %d | Intel: %d\n", p1.lives, p1.intel);
		
		dispGrid(grid, N);

		playerMovement(&p1, grid, N);
	}
	
	//free grid	
	for(int i=0;i<N;i++)
		free(grid[i]);
	free(grid);
	
	return 0;
}



//function to display the map
void dispGrid(char **grid, int N){
	
	for(int i=0;i<N;i++){
		for(int j = 0; j < N; j++)
			printf("+---");
		printf("+\n");

		for(int j = 0 ; j < N; j++)
			printf("| %c ",grid[i][j]);
		printf("|\n");
	}

	for(int j=0;j<N;j++)
		printf("+---");
	printf("+");

}



//function to set random player position
void playerPosition(Player *p, char symbol, char**grid, int N){

	p->symbol = symbol;
	p->lives = 3;
	p->intel = 0;
	p->row = rand() % N;
	p->col = rand() % N;

	grid[p->row][p->col] = p->symbol;

}


//function to set objects randomly
void objectPosition(char **grid, int N){

	int count,row,col;


	//place 3 intel on map
	count = 0;
	while(count < 3){
		row = rand() % N;
		col = rand() % N;

		if(grid[row][col] == '.'){
			grid[row][col] = 'I';
			count++;
		}
	}

	//place 2 lives on map
	count = 0;
	while(count < 2){
		row = rand() % N;
		col = rand() % N;

		if(grid[row][col] == '.'){
			grid[row][col] = 'L';
			count++;
		}
	}

	//place 1 extraction point on map
	count = 0;
	while(count < 1){
		row = rand() % N;
		col = rand() % N;

		if(grid[row][col] == '.'){
			grid[row][col] = 'X';
			
			break;
		}
	}

	//place walls on map
	count = 0;
	//wall count on map changes as N changes(20% of map used)
	int wallCount = (N*N*20)/100;
	while(count < wallCount){
		row = rand() % N;
		col = rand() % N;

		if(grid[row][col] == '.'){
			grid[row][col] = '#';
			count++;
		}
	}
}

//function to accept user input and move player position
void playerMovement(Player *p, char **grid, int N){

	int newRow = p->row;
    	int newCol = p->col;

	char move;
    	printf("Move (W/A/S/D) or Q to quit: ");
    	scanf(" %c",&move);
    
    	if(move == 'q' || move == 'Q')
        	exit(0);

    	if(move == 'w' || move == 'W')
        	newRow--;
    	else if(move == 's' || move == 'S')
        	newRow++;
    	else if(move == 'a' || move == 'A')
        	newCol--;
    	else if(move == 'd' || move == 'D')
        	newCol++;

    
	//condition to see if player moves out of the grid
	if(newRow < 0 || newRow >= N || newCol < 0 || newCol >= N){
    		printf("Cannot move outside the grid!\n");
	       	return;
	}
    
    	if(grid[newRow][newCol] == '#'){
        	p->lives--;

    	if(p->lives == 0){
        	printf("Game over");
        	exit(0);
    	}
	return;
	}

    	if(grid[newRow][newCol] == 'I'){
        	p->intel++;
		grid[newRow][newCol] = '.';
	}

    	if(grid[newRow][newCol] == 'L'){
        	p->lives++;
		grid[newRow][newCol] = '.';
	}

    	if(grid[newRow][newCol] == 'X'){
        	printf("Congratulations! you won.");
        	exit(0);
   	 }
    
    	grid[p->row][p->col] = '.';
    	p->row = newRow;
    	p->col = newCol;
    	grid[p->row][p->col] = p->symbol;
}
