#include <assert.h>
#include <ctype.h>   
#include <stdio.h> 
#include <stdlib.h>  
#include <unistd.h>
#include <string.h>
#include <conio.h>

#define error_mine_found 1
#define error_space_found 2
#define no_error 3

#define R 5 //Rows
#define C 5 //Columns
#define undiscovered_space '.'
#define discovered_space ' '
#define mine '*'
#define amount_of_mines 5
#define DEBUG 0 //To show or hide the mines

//Return the value of nearby mines at a given coordinate
int get_nearby_mines (int x, int y, char board[R][C])
{
    int count=0;

    for (int i=x-1; i<=x+1 ; i++){
        for (int j=y-1; j<=y+1; j++){
            if ((i==x && j==y) || (i<0 || j<0) || (i>=R || j>= C)){
                continue;
            }
            if (board[i][j]== mine){
                count++;
            }
        }
    }
    return count;
}

//Return a random value between min and max
int create_random_value(int min, int max)
{
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

//Fill the board with undiscovered spaces
void create_board (char board[R][C])
{
    for (int i =0;i <R; i++){
        for (int j=0; j<C;j++){
            board [i][j] = undiscovered_space;
        }
    }
}

//Place the mine at a given coordinate
void place_mines (int row, int column, char board [R][C])
{
    board [row][column] = mine;
}

//Place the mines in a random way
void place_mines_randomly(char board [R][C])
{
    for (int i =0; i<amount_of_mines;i++){
        int row = create_random_value (0, R-1);
        int column = create_random_value (0, C-1);
        place_mines (row, column, board);   
    }
}

void print_header_separator()
{
    for (int i =0; i <=C ; i++) {
        printf("----");
        if ( i+2 == C) { 
            printf("-");
        }
    }
    printf("\n");
}

void print_row_separator()
{
    for (int i =0; i<= C; i++){
        printf("+---");
        if (i== C){
            printf("+");
        }
    }
    printf("\n");
}

void print_header ()
{
    print_header_separator();
    printf("|   ");
    for (int i =0; i<C; i++){
        printf ("| %d ", i + 1);
        if (i + 1 == C)
            printf("|");
    }
    printf("\n");
}

//Convert an int to a char
char int_to_char (int number)
{
    return number + '0';
}

void print_board (char board[R][C], int show_mines)
{   
    print_header();
    print_header_separator();
    char word = 'A';
    
    for (int i=0; i< R; i++){
        //Print the word of the row
        printf("| %c ",word);
        word++;
        for (int j=0; j<C ; j++){
            //Not showing to the user if there is a mine
            char truly_word = undiscovered_space;
            char actual_word = board [i][j];

            if (actual_word == mine) {
                truly_word = undiscovered_space;
            }
            else if (actual_word == discovered_space){
                //If the player opened it, we show the nearby mines
                int nearby_mines = get_nearby_mines(i, j, board);
                truly_word = int_to_char (nearby_mines); 
            }

            //If DEBUG is in 1, have to show the mines because the player won or lost
            //So we show the original mine
            if (actual_word == mine && (DEBUG || show_mines)){
                truly_word = mine;
            }
            printf("| %c ", truly_word);
            if (j+1 == C){
                printf("|");
            }
        }
        printf("\n");
        print_row_separator();
    }
}

int open_box(char row_word, int column, char board [R][C])
{
    row_word = toupper (row_word);
    column--;
    //We convert the word to a index
    int row= row_word - 'A';
    assert (column < C && column >=0);
    assert (row < R && row >= 0);
    if (board[row][column]== mine)
        return error_mine_found;

    if (board[row][column] == discovered_space)
        return error_space_found;

    //If there is not error we show the space as a discovered space
    board[row][column] = discovered_space;
    return no_error;
}

//To know if the player won
int there_are_no_unopened_boxes (char board [R][C])
{
    for (int i=0 ; i< R; i++){
        for (int j=0; j<C; j++){
            char actual = board[i][j];
            if (actual == undiscovered_space)
                return 0;
        }
    }
    return 1;
}

int main ()
{
    char board [R][C];
    int show_mines = 0;

    srand (getpid());
    create_board(board);
    place_mines_randomly(board);

    printf("\t\t------------------------------------\n");
    printf("\t\t+	      WELCOME              +\n");
    printf("\t\t+	        to                 +\n");
    printf("\t\t+           MINESWEEPER            +\n");
    printf("\t\t------------------------------------\n");
    printf("\n\t\tStudent Name : Danny\n");

    // Infinite loop. It breaks if you win or lose, and that is defined with
    // "show_mines"
    
    while (1) {
    print_board (board, show_mines);
        if (show_mines){
            break;
        }
        int column;
        char row;
        printf ("Enter the row: ");
        scanf(" %c", &row);
        printf("Enter the column: ");
        scanf("%d", &column);
        int status = open_box (row, column, board);
        if (there_are_no_unopened_boxes(board)){
            printf("\n\tYOU WIN\n");
            show_mines = 1;
        } 
        else if (status == error_space_found)
        {
            printf("You have already discovered this box\n");
        }
        else if (status == error_mine_found)
        {
            printf("\n\tYOU LOSE\n");        
            show_mines = 1;
        }
    }
        
    getch();
    return 0;
}