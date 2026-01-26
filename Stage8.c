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
    int active;
} Player;

void dispGrid(char **grid, int N);
void playerPosition(Player *p, char symbol, char **grid, int N);
void objectPosition(char **grid, int N);
void playerMovement(Player *p, char **grid, int N, char move);
void logGame(Player *p, char **grid, int N);

//main program
int main(){

    Player p1, p2, p3;

    //seed a random value
    srand(time(0));

    int N;
    //condition to validate grid input size
    do{
        printf("Enter grid size(5-15) : ");
        scanf("%d",&N);
        if(N<5 || N>15)
            printf("Invalid grid size\n");
    } while(N<5 || N>15);

    //allocate memory for the entire grid
    char **grid = malloc(N*sizeof(char*));
    
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
    for(int i=0;i<N;i++)
        for(int j=0; j<N;j++)
            grid[i][j] = '.';

    int mode;
    int subMode = 0;

    //select game mode
    printf("\nGame Modes:\n1. Single Player\n2. Two Player\n3. Three Player\nEnter mode: ");
    scanf("%d", &mode);

    if(mode == 2){
        printf("\nTwo Player Mode:\n1. Human vs Human\n2. Human vs Computer\nEnter choice: ");
        scanf("%d", &subMode);
    }

    //if mode is 1 other players are removed from grid
    if(mode == 1){
        playerPosition(&p1, '@', grid, N);
        p1.active = 1;
        p2.active = 0;
        p3.active = 0;
    }
    else if(mode == 2){
        playerPosition(&p1, '@', grid, N);
        playerPosition(&p2, '&', grid, N);
        p1.active = p2.active = 1;
        p3.active = 0;
    }
    else if(mode == 3){
        playerPosition(&p1, '@', grid, N);
        playerPosition(&p2, '&', grid, N);
        playerPosition(&p3, '$', grid, N);
        p1.active = p2.active = p3.active = 1;
    }

    objectPosition(grid, N);

    int turn = 0;

    while(1){
        char move;

        //1 player mode
        if(mode == 1 && p1.active){
            printf("\nPlayer @ | Lives: %d | Intel: %d\n", p1.lives, p1.intel);
            dispGrid(grid, N);
            printf("\nMove (W/A/S/D) or Q to quit: ");
            scanf(" %c", &move);
            playerMovement(&p1, grid, N, move);
            logGame(&p1, grid, N);

            if(!p1.active){
                if(p1.intel == 3)
                    printf("Player @ won!\n");
                else
                    printf("Player @ eliminated!\n");
                break;
            }
        }

        //2 player mode
        else if(mode == 2){

	//turns are taken cyclicaly based on remainder values
            if(turn % 2 == 0 && p1.active){
                printf("Player @ turn | Lives: %d | Intel: %d\n", p1.lives, p1.intel);
                dispGrid(grid, N);
                printf("\nMove (W/A/S/D) or Q to quit: ");
                scanf(" %c", &move);
                playerMovement(&p1, grid, N, move);
                logGame(&p1, grid, N);
            }
            else if(turn % 2 == 1 && p2.active){
                if(subMode == 1){
                    printf("Player & turn | Lives: %d | Intel: %d\n", p2.lives, p2.intel);
                    dispGrid(grid, N);
                    printf("\nMove (W/A/S/D) or Q to quit: ");
                    scanf(" %c", &move);
                }
                else{
                    char moves[4] = {'W','A','S','D'};
                    move = moves[rand() % 4];
                    printf("Computer chose: %c\n", move);
                }
                playerMovement(&p2, grid, N, move);
                logGame(&p2, grid, N);
            }

            //check winners
            if(!p1.active && !p2.active){
                printf("Both players eliminated!\n");
                break;
            }
            if(!p1.active && p2.active){
                printf("Player & wins!\n");
                break;
            }
            if(!p2.active && p1.active){
                printf("Player @ wins!\n");
                break;
            }
        }

        //3 player mode
        else if(mode == 3){

            if(turn % 3 == 0 && p1.active){
                printf("Player @ turn | Lives: %d | Intel: %d\n", p1.lives, p1.intel);
                dispGrid(grid, N);
                printf("\nMove (W/A/S/D) or Q to quit: ");
                scanf(" %c", &move);
                playerMovement(&p1, grid, N, move);
                logGame(&p1, grid, N);
            }
            else if(turn % 3 == 1 && p2.active){
                printf("Player & turn | Lives: %d | Intel: %d\n", p2.lives, p2.intel);
                dispGrid(grid, N);
                printf("\nMove (W/A/S/D) or Q to quit: ");
                scanf(" %c", &move);
                playerMovement(&p2, grid, N, move);
                logGame(&p2, grid, N);
            }
            else if(turn % 3 == 2 && p3.active){
                printf("Player $ turn | Lives: %d | Intel: %d\n", p3.lives, p3.intel);
                dispGrid(grid, N);
                printf("\nMove (W/A/S/D) or Q to quit: ");
                scanf(" %c", &move);
                playerMovement(&p3, grid, N, move);
                logGame(&p3, grid, N);
            }

            //check winners
            int activeCount = p1.active + p2.active + p3.active;
            if(activeCount == 1){
                if(p1.active) printf("Player @ wins by elimination!\n");
                if(p2.active) printf("Player & wins by elimination!\n");
                if(p3.active) printf("Player $ wins by elimination!\n");
                break;
            }
            else if(activeCount == 0){
                printf("All players eliminated!\n");
                break;
            }
        }

        turn++;
    }

    //free memory
    for(int i = 0; i < N; i++)
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

    //prevents players starting on same posiotion
    do{
    p->row = rand() % N;
    p->col = rand() % N;
    }while(grid[p->row][p->col] != '.');

    grid[p->row][p->col] = p->symbol;
}

//function to set objects randomly
void objectPosition(char **grid, int N){
    int count,row,col;

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

}

//function to accept user input and move player position
void playerMovement(Player *p, char **grid, int N, char move){
    int newRow = p->row;
    int newCol = p->col;

    //player quits game
    if(move == 'q' || move == 'Q'){
        printf("Player %c quit the game.\n",p->symbol);
        p->active = 0;
        return;
    }

    if(move == 'w' || move == 'W') newRow--;
    else if(move == 's' || move == 'S') newRow++;
    else if(move == 'a' || move == 'A') newCol--;
    else if(move == 'd' || move == 'D') newCol++;
    else{
        printf("Invalid move\n");
        p->lives--;
        if(p->lives == 0){
            printf("Player %c ran out of lives.\n",p->symbol);
            p->active = 0;
        }
        return;
    }

 	//condition to see if player moves out of the grid
    if(newRow < 0 || newRow >= N || newCol < 0 || newCol >= N){
        printf("Cannot move outside the grid!\n");
        p->lives--;
        if(p->lives == 0){
            printf("Player %c ran out of lives.\n",p->symbol);
            p->active = 0;
        }
        return;
    }

    ///condition when user hits a wall
    if(grid[newRow][newCol] == '#'){
         printf("You hit a wall\n");
        p->lives--;
        if(p->lives == 0){
            printf("Player %c ran out of lives.\n",p->symbol);
            p->active = 0;
        }
        return;
    }

    //player collects intel
    if(grid[newRow][newCol] == 'I'){
        p->intel++;
        grid[newRow][newCol] = '.';
    }

    //player collects lives
    if(grid[newRow][newCol] == 'L'){
        p->lives++;
        grid[newRow][newCol] = '.';
    }

    //player reaches extraction point
    if(grid[newRow][newCol] == 'X'){
        if(p->intel == 3){
            printf("Congratulations!\n");
            p->active = 0;
            return;
        } else {
            printf("Player %c reached X without collecting all intel.\n",p->symbol);
            p->active = 0;
            return;
        }
    }

    grid[p->row][p->col] = '.';
    p->row = newRow;
    p->col = newCol;
    grid[p->row][p->col] = p->symbol;
}

//function to log game state
void logGame(Player *p, char **grid, int N){
    
    //append to file(else overwrites)
    FILE *fp = fopen("grid.txt","a");
   
    if(fp == NULL){
        printf("Error opening file");
        return;
    }

    fprintf(fp,"\n--- LOG ---\n");
    fprintf(fp,"Player %c | Lives: %d | Intel: %d\n",p->symbol, p->lives, p->intel);

    
    for(int i=0; i<N; i++){

        for(int j=0; j<N; j++)
            fprintf(fp, "+---");
        fprintf(fp, "+\n");

        for(int j=0; j<N; j++)
            fprintf(fp, "| %c ", grid[i][j]);
        fprintf(fp, "|\n");
    }
    
    for(int j=0; j<N; j++)
        fprintf(fp, "+---");
    fprintf(fp, "+\n\n");


    fclose(fp);
}
