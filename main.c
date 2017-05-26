#include <stdio.h>
#include <stdlib.h>
// Complete the game of Connect Four from this starter code to recieve
// bonus points!

#define MAX_ROW 6
#define MAX_COL 7
#define HAS_SEEDED
typedef enum boolean
{
    NAY=0, YES=1
} Bool;

typedef enum occupied
{
	FALSE, TRUE
} Occupied;

typedef struct location
{
	int row;
	int col;
} Location;

typedef struct cell
{
	char color;
	Occupied isOccupied;
	Location place;
} Cell;

typedef struct player
{
    char color;
    int score;

} Player;

void init_board(Cell board[][MAX_COL], int rows, int cols)
{
	int row_index = 0, col_index = 0;

	for (; row_index < rows; ++row_index)
	{
		for (col_index = 0; col_index < cols; ++col_index)
		{
			board[row_index][col_index].color = '0';
			board[row_index][col_index].isOccupied = FALSE;
			//board[row_index][col_index].place.row = row_index;
			//board[row_index][col_index].place.col = col_index;
			// this portion doesn't work for my implementation
		}
	}
}

// determines if a player has won the game
// Returns 1 and 0 if pp has won or has not won respectively
Bool hasWon(Cell board[][MAX_COL], Player pp)
{
    // I need 3 algorithms for 3 possibilities
    // 1: Horizontal victory, 2: Vertical Victory, 3: Diagonal Victory
    Bool determined = NAY;
    int rows = 0, column = 0, tally = 0;

    // Vertical check
    while (column < MAX_COL){
        rows = 0, tally = 0;
        while (rows < MAX_ROW){

            if (board[rows][column].color == pp.color){
                tally += 1; // tallies up how many pieces are "contiguous"
            } else {
                tally = 0;
            }
            if (tally >= 4){ // if 4 or more pieces are contiguous horizontally,
                //printf("\n\nPlayer **%c**won, 4 pieces connect vertically!\n", pp.color); // then we have a connect four!
                return YES;
            }
            rows++;
        }
        column++;
    }

    // Horizontal Check
    tally = 0;
    for (rows = 0; rows < MAX_ROW; rows++){
        for (column = 0, tally = 0; column < MAX_COL; column++){
            if (board[rows][column].color == pp.color){
                tally += 1;
            } else {
                tally = 0; // this prevents program from falsely awarding a win to pieces that aren't contiguous
            }
            if (tally >= 4){
                //printf("\n\nPlayer **%c** won horizontally!\n", pp.color);
                return YES;
            }
        }
    }

    // Diagonal Check
    /*
     * Diagonal Check will be performed in a right-up (northeast), right-down(southeast) fashion
    */
    //***** southeast fashion check ****
    rows = 0, column = 3;
    // run is how many times we'll check the matrix, and I've predetermined that run will be 6
    // but I'm sure there's a better way of automating this rather than hard-coding it
    int i = 0, j = 3, run = 6; // i and j are helper variables, as is run.
    printf("\n\n");
    while (run >= 0){

        while (rows < MAX_ROW && column < MAX_COL) {
//          printf("%c ", board[rows][column].color);  // **UNCOMMENT THIS LINE TO SEE INTERNALS OF THE SOUTHEAST DIAGONAL ALGORITHM**
            if (board[rows][column].color == pp.color){
                tally += 1; // tallies up how many pieces are "contiguous"
            } else {
                    tally = 0;
            }
            if (tally >= 4){ // if 4 or more pieces are contiguous horizontally,
                //printf("\n\nPlayer **%c**won, 4 pieces connect vertically!\n", pp.color); // then we have a connect four!
                //return YES;
            }
            rows++;
            column++;
        }
//        putchar('\n'); // ** UNCOMMENT THIS LINE TOO **
        if (run < 4){   // if at the origin
            i++;        // start incrementing the row
            j = 0;      // but keep column static to index 0
        } else {    // otherwise if we haven't reached the middle
            i = 0;  // keep row static
            j--;    // but decrement our column
                    /* Column is decremented because we're moving right->left initially, row is incremented because we're moving top-down*/
        }
        rows = i, column = j;
        run--;
    }

    // northeast algorithm for our matrix
    rows = 5, column = 3;
    // run is how many times we'll check the matrix, and I've predetermined that run will be 6
    // but I'm sure there's a better way of automating this rather than hard-coding it
    i = 5, j = 3, run = 6; // i and j are helper variables, as is run.
    printf("\n\n");
    while (run >= 0){
        tally = 0;
        while (rows >= 0 && column < MAX_COL) {
            //printf("%c ", board[rows][column].color);  // **UNCOMMENT THIS LINE TO SEE INTERNALS OF THE SOUTHEAST DIAGONAL ALGORITHM**
            if (board[rows][column].color == pp.color){
                tally += 1; // tallies up how many pieces are "contiguous"
            } else {
                    tally = 0;
            }
            if (tally >= 4){ // if 4 or more pieces are contiguous horizontally,
                //printf("\n\nPlayer **%c**won, 4 pieces connect vertically!\n", pp.color); // then we have a connect four!
                return YES;
            }
            // printf("Row: %d, Column: %d\n", rows, column);    /* UNCOMMENT THIS LINE TO SEE THE INTERNALS */
            rows--;
            column++;
        }
        //putchar('\n'); // ** UNCOMMENT THIS LINE TOO **
        if (run >= 4){   // if we haven't reached the middle
            i = 5;  // keep row static
            j--;
        } else if (run < 4){    // If at the origin
            i--;        // start decrementing the row
            j = 0;      // but keep column static to index 0
                    /* Column is decremented because we're moving right->left initially, row is incremented because we're moving top-down*/
        }
        rows = i, column = j;
        run--;
    }



    return NAY;
}

/* Drops the imaginary pieces at location in that column
    When we drop pieces, we drop only from the column and
    physics (through gravity and electromagnetism) will decide the row,
    so a pre-determined row is unnecessary

*/
void drop_piece(Cell board[][MAX_COL], Player pp, int column)
{
    Bool drop_determined = NAY; // determine if drop is possible or impossible
    int rows = MAX_ROW - 1;


    while (!drop_determined){
        if (board[rows][column].isOccupied == TRUE){
            rows--; // because the bottom most row's index value is 5, we count backwards
                    // If there's a piece already at row 5, then we can't reoccupy that space

            drop_determined = NAY; // drop cannot be determined (inconclusive)
        } else if (rows < 0){
            // the space at this column is fully occupied, can't drop anymore!
            printf("This column is full!\n");

            drop_determined = YES; // drop has been determined to be impossible
        } else if (column > (MAX_COL - 1)) {
            printf("You can't drop a piece on a non-existent column\n");
        } else {
            board[rows][column].color = pp.color;
            board[rows][column].isOccupied = TRUE;
			board[rows][column].place.row = rows;
			board[rows][column].place.col = column;

			drop_determined = YES; // drop has been determined to be possible
            return 0;
        }
    }
}

void displayBoard(Cell board[][MAX_COL])
{
    int row = 0, column = 0;
    printf("Column\n");
    printf("0 1 2 3 4 5 6\n");
    printf("=============\n\n");
    while (row < MAX_ROW) {
        column = 0;
        while (column < MAX_COL) {
            printf("%c ", board[row][column].color);
            column++;
        }
        putchar('\n');
        row++;
    }
}

// returns what column the computer wants to drop its pieces
int computer_player(Cell board[][MAX_COL]){
    // It would be awesome to make the computer smart but time is not luxury, there it would have to be stupid
    int decision = rand() % 6;
    return decision;
}

int game(Cell board[][MAX_COL], Player pp[], int *round){
    system("cls");
	printf("YOU ARE PLAYER h!\n");
	printf("When prompted, enter the column of where you want your pieces dropped\n");

    Bool playerWins = FALSE;
    int dropColumn = 0, successfullyDropped = 0;
    do {
        displayBoard(board);
        successfullyDropped = 1; // the assumption is that the human player will successfully drop within range
        printf("Enter column (99 to exit): ");
        scanf(" %d", &dropColumn);
        if (dropColumn >= MAX_COL || dropColumn < 0){
            if (dropColumn == 99){
                exit(1);
            }
            system("cls");
            printf("**Can't drop at a nonexistent column**\n");
            successfullyDropped = 0;    // if the assumption is not true, this is not true.
            system("pause");
        } else {
            drop_piece(board, pp[1], dropColumn);
            playerWins = hasWon(board, pp[1]);
        }
        if (playerWins){
            pp[1].score += 1;
            displayBoard(board);
            printf("You won!\n");
            return 0;
        }

        if (successfullyDropped){
            printf("\n\nComputer's turn\n");
            dropColumn = rand() % 7;
            system("cls");
            printf("Computer will drop its piece at column %d\n\n", dropColumn);
            drop_piece(board, pp[0], dropColumn);

            playerWins = hasWon(board, pp[0]);
            if (playerWins){
                displayBoard(board);
                pp[0].score += 1;
                printf("Computer won round %d\n", *round);
                return 0;
            }
            //system("pause");
            //system("cls");
        }
    } while (!playerWins);

    displayBoard(board);

    *round += 1;
}

int main(void)
{
	Cell board[MAX_ROW][MAX_COL] = { {/*row 0*/{'\0', FALSE, {0, 0}}} };
    init_board(board, MAX_ROW, MAX_COL);
    srand((int) time(NULL)); // seed

Player pp[2];
	pp[0].color = 'C';  // c for computer
	pp[0].score = 0;
	pp[1].color = 'H';  // h for human
	pp[1].score = 0;
    int round = 1, exit = 0;
    char again = '\0';
    printf("Press Enter to start playing\n");
    getchar();
    do {
        exit = game(board, pp, &round);
        if (exit == 0) {
            printf("\tYou won: %d rounds out of %d\n", pp[1].score, round);
            printf("\tComputer won: %d rounds out of %d\n", pp[0].score, round);
            printf("Would you like to play again? (y/n): ");
            scanf(" %c", &again);

            if (again == 'y'){
                init_board(board, MAX_ROW, MAX_COL);
            } else {
                exit = 1;
            }
        }
    } while (exit != 1);
    printf("\n\nBefore you go, here's the game statistics:\n");
    printf("\tYou won: %d rounds out of %d\n", pp[1].score, round);
    printf("\tComputer won: %d rounds out of %d\n", pp[0].score, round);
    if (pp[0].score > pp[1].score){
        printf("Skynet is either learning or you did not know the rules\n");
    }
	return 0;
}

