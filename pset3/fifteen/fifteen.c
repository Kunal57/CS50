/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// track blank_space
int blank_row;
int blank_column;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // initial blank_space location
    blank_row = d - 1;
    blank_column = d - 1;

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void)
{
    // initialize variables for numbers of tiles on board and numbers to insert into the 2D array
    int tiles = (d * d) - 1;
    int numbers = tiles;
    // loop through the rows
    for(int a = 0; a < d; a++)
    {
        // loop through each position in the row
        for(int b = 0; b < d; b++)
        {
            // if odd amount of tiles, replace 2 with 1
            if ((tiles % 2) != 0 && numbers == 2)
            {
                board[a][b] = 1;
                numbers -= 1;
            }
            // if odd amount of tiles, replace 1 with 2
            else if ((tiles % 2) != 0 && numbers == 1)
            {
                board[a][b] = 2;
                numbers -= 1;
            }
            // if even amount of tiles or numbers aren't 2 or 1
            else
            {
                board[a][b] = numbers;
                numbers -= 1;
            }
        }
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    printf("\n");
    for (int c = 0; c < d; c++)
    {
        for (int e = 0; e < d; e++)
        {
            // if the value is a single digit number, print an space before the number to help align visually
            if (board[c][e] < 10)
            {
                // if the value is zero, space with a _ to signify blank space
                if (board[c][e] == 0)
                {
                    printf(" _  ");
                }
                else
                {
                    printf(" %i  ", board[c][e]);
                }
            }
            // if the value is a double digit number, print no space inbetween the number
            else
            {
                printf("%i  ", board[c][e]);
            }
        }
        // print another row on a new line + a added line for spacing
        printf("\n");
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
bool move(int tile)
{
    for (int f = 0; f < d; f++)
    {
        for (int g = 0; g < d; g++)
        {
            // if the value of the tile equals the tile entered by the user
            if (board[f][g] == tile)
            {
                // (If the blank space row index equals the user input row index) and (adding or subtracting 1
                // from the user input column index equals the blank space column index)
                // OR
                // (If the blank space column index equals the user input column index) and (adding or subtracting 1
                // from the user input row index equals the blank space row index)
                if ((blank_row == f && (blank_column == g + 1 || blank_column == g - 1)) || ((blank_row == f + 1 || blank_row == f - 1) && blank_column == g))
                {
                    // replace the blank row position with the user input tile
                    // replace the user input tile with the blank space
                    // reassign the blank space variables with the new blank space position
                    board[blank_row][blank_column] = tile;
                    board[f][g] = 0;
                    blank_row = f;
                    blank_column = g;
                    return true;
                }
            }
        }
    }
    // return false if tile is not found or tile can't be swapped with the blank space
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
bool won(void)
{
    // set variables for the amount of tiles and a counter to see if the tiles are in order
    int tiles = (d * d) - 1;
    int counter = 1;

    for (int h = 0; h < d; h++)
    {
        for (int k = 0; k < d; k++)
        {
            // if the board value does not equal the counter value
            // AND
            // the counter is still below the number of tiles on the board
            if (board[h][k] != counter && counter <= tiles)
            {
                return false;
            }
            counter += 1;
        }
    }
    // if no errors are found in the checking of the board, return true
    return true;
}
