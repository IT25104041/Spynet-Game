#include <stdio.h>
#include <stdlib.h>

//structure for player
typedef struct{
	char symbol;
	int intel;
	int lives;
	int row;
	int col;
}Player;

void dispGrid(int N);

int main(){
	
	int N;

	//condition to valid grid input size
	do{

	printf("Enter grid size(5-15) : ");
	scanf("%d",&N);

	if(N<5 || N>15)
		printf("Invalid grid size\n");
			
	}while(N<5 || N>15); 

	dispGrid(N);


return 0;
}

//function to display the map
void dispGrid(int N){
	char **grid = malloc(N * sizeof(char*));

	if(grid == NULL){
		printf("Memory allocation error.");
		return;
	}

	//allocate memory for each row
	for(int i=0; i<N; i++){
		grid[i] = malloc(N * sizeof(char));

		if(grid[i] == NULL){
			printf("Memory allocation error.");
			return;
		}
	}

	//create grid
	for(int i=0;i<N;i++){
		for(int j=0; j<N;j++)
			grid[i][j] = '.';
	}

	
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

	for(int i=0;i<N;i++)
		free(grid[i]);
	free(grid);
}


