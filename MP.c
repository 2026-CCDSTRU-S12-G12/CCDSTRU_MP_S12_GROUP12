#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define BOARD_SIZE 3
#define MAX_ELEM (BOARD_SIZE * BOARD_SIZE)

//ANSI Escape Sequences
#define CLEAR "\033[2J"
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_BLUE "\033[34m"
#define COLOR_PURPLE "\033[35m"
#define COLOR_SRED "\033[91m"
#define COLOR_SBLUE "\033[94m"
#define COLOR_SPURPLE "\033[95m"
#define COLOR_YELLOW_BG "\033[103m"

typedef const char *sprite[5];

typedef struct {
    int x; // x-coordinate
    int y; // y-coordinate
} Coord;

typedef struct {
    Coord coord[MAX_ELEM]; // array of coordinates
    int num_coord;         // number of coordinates in the set
} Set;

// HELPER FUNCTIONS
/*
    Purpose: checks if the given pos is a valid coordinate in set M
    Return: either (a) 1 if pos is valid (b) 0 if not
*/
int isValidPos(Coord pos)
{
    return (pos.x >= 1 && pos.x <= BOARD_SIZE && pos.y >= 1 && pos.y <= BOARD_SIZE);
}

/*
    Purpose: checks if a given coordinate exists in a set
    Return: either (a) 1 if found; (b) 0 if not
    Example: A = {(1,2), (2,1), (3,3)}, see if (3,3) exists in the set, then:
             this function returns 1
             see if (3,1) exists in the set, then:
             this function returns 0
*/
int isElement(Set A, Coord pos)
{
    int isFound = 0;
    int i;
    int n = A.num_coord;

    i = 0;
    while (i < n && !isFound)
    {
        if (A.coord[i].x == pos.x && A.coord[i].y == pos.y)
            isFound = 1;

        i++;
    }

    return isFound;
}

/*
    Purpose: removing a coordinate in a set
    Return: none (modifies the set in the parameter indirectly)
    Example: A = {(1,2), (2,1)} & num_coord = 2, remove (1,2) in Set A, then:
             A = {(2,1)} and num_coord = 1
*/ 
void removeCoord(Set *A, Coord pos)
{
    int i;
    int n = A->num_coord;
    Set temp;
    temp.num_coord = 0;

    for (i = 0; i < n; i++)
    {
        if (!(A->coord[i].x == pos.x && A->coord[i].y == pos.y))
        {
            temp.coord[temp.num_coord] = A->coord[i];
            temp.num_coord++;
        }
    }

    *A = temp;
}

/*
    Purpose: adding a coordinate in a set
    Return: none (modifies the set in the parameter indirectly)
    Example: A = {(1,2), (2,1)} & num_coord = 2, add (3,3) in Set A, then:
             A = {(1,2), (2,1), (3,3)} and num_coord = 3
*/ 
void addCoord(Set *A, Coord pos)
{
    int n = A->num_coord;

    if (n < MAX_ELEM && !isElement(*A, pos)) // avoids duplicate
    {
        A->coord[n] = pos;
        A->num_coord++;
    }
}

/*
    Purpose: checks if the game is over
    Return: 1 if over, and 0 if not
*/ 
int isOver(Set R, Set B, int start, int val)
{
    int F_size; // cardinality of F

    // since R and B will never share the same positions/coordinates, size of F can be computed as:
    F_size = MAX_ELEM - (R.num_coord + B.num_coord);

    return (F_size == 3 || val >= 20 || (!start && ((R.num_coord > 0 && B.num_coord == 0) || (R.num_coord == 0 && B.num_coord > 0))));
}

/*
    Purpose: prints the current board state (in debug form) to the terminal
    Return: none
*/ 
void printBoardDebug(int size, Set *R,Set *B, Set *S, Set *T)
{
    int i, j;
    Coord temp;
    char cR;
    char cB;
    char cS;
    char cT;

    for (i = 1; i < size + 1; i++)
    {
        for(j = 1; j < size + 1; j++)
        {
            temp.x = i;
            temp.y = j;
            cR = ' ';
            cB = ' ';
            cS = ' ';
            cT = ' ';

            if (isElement(*R, temp)) cR = 'R';
            if (isElement(*B, temp)) cB = 'B';
            if (isElement(*S, temp)) cS = 'S';
            if (isElement(*T, temp)) cT = 'T';

            printf("%c%c%c%c", cS, cR, cB, cT);

            if (j < size)
            {
                printf("|");
            }
        }

        if(i < size)
        {
            printf("\n--------------\n");
        }
    }

    printf("\n");
}

/*
    Purpose: prints the current board state to the terminal
    Return: none
*/ 
void printBoard(int size, Set *R, Set *B, Set *S, Set *T, sprite RED, sprite SRED, sprite BLUE, sprite SBLUE, sprite PURPLE, sprite SPURPLE, sprite EMPTY)
{
    int i, j, k;
    int cR[size];
    int cB[size];
    int cS[size];
    Coord temp;
    printf(COLOR_BLUE"\n-----------------\n"COLOR_RESET);

    for (i = 0; i < size; i++)
    {
        temp.x = i + 1;

        for(j = 0; j < size; j++)
        {
            temp.y = j + 1;
            cR[j] = 0;
            cB[j] = 0;
            cS[j] = 0;

            if (isElement(*R, temp)) cR[j] = 1;
            if (isElement(*B, temp)) cB[j] = 1;
            if (isElement(*S, temp)) cS[j] = 1;
        }

        for (k = 0; k < 5; k++)
        {
            for (j = 0; j < size; j++)
            {
                if (cS[j])
                {
                    if (cR[j] && cB[j])
                    {
                        printf(SPURPLE[k]);
                    }
                    else if (cR[j])
                    {
                        printf(SRED[k]);
                    }
                    else if (cB[j])
                    {
                        printf(SBLUE[k]);
                    }
                    else
                    {
                        printf(EMPTY[k]);
                    }
                }
                else
                {
                    if(cR[j] && cB[j])
                    {
                        printf(PURPLE[k]);
                    }
                    else if(cR[j])
                    {
                        printf(RED[k]);
                    }
                    else if(cB[j])
                    {
                        printf(BLUE[k]);
                    }
                    else
                    {
                        printf(EMPTY[k]);
                    }
                }
                if (j < size - 1)
                {
                    printf("|");
                }
            }

            if ( k < 4)
            {
                printf("\n");
            }
        }

        if (i < size - 1)
        {
            printf("\n-----------------\n");
        }
    }

    printf(COLOR_RED"\n-----------------\n"COLOR_RESET);
    printf("\n");
}

// MAIN FUNCTIONS 
/*
    Purpose: Remove's a player's figure after expand was run on it
    Return: none
*/ 
void Remove(Coord pos, int *go, Set *R, Set *B, Set *S, Set *T)
{
    if (*go)
        removeCoord(R, pos);
    else
        removeCoord(B, pos);
    
    removeCoord(S, pos);
    removeCoord(T, pos);
}

//Skeleton declared for Replace to read
void Expand(Coord pos, int *go, int *found, Set *R, Set *B, Set *S, Set *T);

/*
    Purpose: Replaces a cell with the current player's figure
    Return: none
*/ 
void Replace(Coord pos, int *go, int *found, Set *R, Set *B, Set *S, Set *T)
{
    *found = 0;

    if (*go)
    {
        if (isElement(*B, pos))
        {
            removeCoord(B, pos);
            *found = 1;
        }
        if (isElement(*R, pos))
            *found = 1;
        else
            addCoord(R, pos);
    }
    else
    {
        if (isElement(*R, pos))
        {
            removeCoord(R, pos);
            *found = 1;
        }
        if (isElement(*B, pos))
            *found = 1;
        else
            addCoord(B, pos);
    }

    if (*found)
    {
        if (!isElement(*S, pos))
        {
            addCoord(S, pos);
            *found = 0;
        }
        else if (!isElement(*T, pos))
        {
            addCoord(T, pos);
            Expand(pos, go, found, R, B, S, T);
        }
    }
}

/*
    Purpose: Runs Replace on 3 adjacent positions
    Return: none
*/ 
void Expand(Coord pos, int *go, int *found, Set *R, Set *B, Set *S, Set *T)
{
    Coord u, d, k, r;
    u = d = k = r = pos;

    u.x--;
    d.x++;
    k.y--;
    r.y++;

    Remove(pos, go, R, B, S, T);

    if (*go && isValidPos(u))
        Replace(u, go, found, R, B, S, T);

    if (!*go && isValidPos(d))
        Replace(d, go, found, R, B, S, T);
    
    if (isValidPos(k))
        Replace(k, go, found, R, B, S, T);

    if (isValidPos(r))
        Replace(r, go, found, R, B, S, T);
   
}

/*
    Purpose: Updates a player's figure, either marking it in set S or running Expand on it
    Return: none
*/ 
void Update(Coord pos, int *go, int *good, int *found, Set *R, Set *B, Set *S, Set *T)
{
    *good = 0;

    if (!isElement(*S, pos))
    {
        addCoord(S, pos);
        *good = 1;
    }
    else if (!isElement(*T, pos)) // omitted !*good in the condition
    {
        addCoord(T, pos);
        Expand(pos, go, found, R, B, S, T);
    }
}

/*
    Purpose: Parses the next player's move
    Return: none
*/ 
void NextPlayerMove(Coord pos, int *start, int *go, int *good, int *found, int *val, Set *R, Set *B, Set *S, Set *T)
{   
    int over = isOver(*R, *B, *start, *val);

    if (!over)
    {
        if (*start)
        { 
            if (*go)
            {
                addCoord(R, pos);
                addCoord(S, pos);
                *good = 1;
            }
            else
            {
                addCoord(B, pos);
                addCoord(S, pos);
                *good = 1;
            }
        }
        else
        {
            if ((*go && isElement(*R, pos)) || (!*go && isElement(*B, pos)))
            {
                Update(pos, go, good, found, R, B, S, T);
                *good = 1;
            }
        }
    }

    if (*start && R->num_coord == 1 && B->num_coord == 1)
    {
        *start = 0;
    }

    //Im not sure if the order the equations appear in matter or not so I'll just give this its own !over if statement
    if (!over && *good)
    {
        *good = !*good;
        *go = !*go;
        (*val)++;
    }
}

/*
    Purpose: Checks which player won the game
    Return: None; Places the result indirectly in the result string
*/ 
void GameOver(Set *R, Set *B, char *result)
{
    if (R->num_coord > B->num_coord) 
    {
        strcpy(result,"R wins");
    } 
    else if (B->num_coord > R->num_coord) 
    {
        strcpy(result,"B wins");
    } 
    else 
    {
        strcpy(result,"draw");
    }
}

/*
    Purpose: Resets all the values for a new game
    Return: None
*/ 
void InitializeValues(Set *R, Set *B, Set *S, Set *T, int *good, int *go, int *start, int *found, int *val, char *result, int *over, int *V){
    Set Empty = {0};
    *R = Empty;
    *B = Empty;
    *S = Empty;
    *T = Empty;

    *good = V[0];
    *go = V[1];
    *start = V[1];
    *found = V[0];
    *val = 0;
    strcpy(result, "");
    *over = 0;
}

int main()
{
    int i, j;

    // APPLICABLE SETS
    // Set C
    int C[BOARD_SIZE] = {0};
    for(i=1;i<BOARD_SIZE+1;i++){
        C[i] = i;
    }
    // Set M
    int size = sizeof(C)/sizeof(int);
    Set M = {0};
    Coord temp;
    for(i = 1; i < size + 1; i++)
    {
        for(j = 1; j < size + 1; j++)
        {
            temp.x = i;
            temp.y = j;
            addCoord(&M,temp);
        }
    }
    //Set N, Unused
    // int N[17] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    
    //Set V
    //V[0] = False/0
    //V[1] = True/1
    int V[2] = {0,1};

    //System Initialization
    //System Variables
    int good = V[0];
    int go = V[1];
    int start = V[1];
    int found = V[0];
    int val = 0;
    char result[10] = "";

    //Student Initialized
    int over = 0;
    
    Set R = {0};
    Set B = {0};
    Set S = {0};
    Set T = {0};

    //Cell States
    sprite RED = {COLOR_RED "\\   /" COLOR_RESET,
                  COLOR_RED " \\ / " COLOR_RESET,
                  COLOR_RED "  X  " COLOR_RESET,
                  COLOR_RED " / \\ " COLOR_RESET,
                  COLOR_RED "/   \\" COLOR_RESET};

    sprite SRED = {COLOR_SRED COLOR_YELLOW_BG "\\   /" COLOR_RESET,
                   COLOR_SRED COLOR_YELLOW_BG " \\ / " COLOR_RESET,
                   COLOR_SRED COLOR_YELLOW_BG "  X  " COLOR_RESET,
                   COLOR_SRED COLOR_YELLOW_BG " / \\ " COLOR_RESET,
                   COLOR_SRED COLOR_YELLOW_BG "/   \\" COLOR_RESET};

    sprite BLUE = {COLOR_BLUE " /-\\ " COLOR_RESET,
                   COLOR_BLUE "|   |" COLOR_RESET,
                   COLOR_BLUE "|   |" COLOR_RESET,
                   COLOR_BLUE "|   |" COLOR_RESET,
                   COLOR_BLUE " \\-/ " COLOR_RESET};

    sprite SBLUE = {COLOR_SBLUE COLOR_YELLOW_BG " /-\\ " COLOR_RESET,
                    COLOR_SBLUE COLOR_YELLOW_BG "|   |" COLOR_RESET,
                    COLOR_SBLUE COLOR_YELLOW_BG "|   |" COLOR_RESET,
                    COLOR_SBLUE COLOR_YELLOW_BG "|   |" COLOR_RESET,
                    COLOR_SBLUE COLOR_YELLOW_BG " \\-/ " COLOR_RESET};

    sprite PURPLE = {COLOR_PURPLE " /+ /" COLOR_RESET,
                     COLOR_PURPLE "| |/ " COLOR_RESET,
                     COLOR_PURPLE "| K  " COLOR_RESET,
                     COLOR_PURPLE "| |\\ " COLOR_RESET,
                     COLOR_PURPLE " \\+ \\" COLOR_RESET};

    sprite SPURPLE = {COLOR_SPURPLE COLOR_YELLOW_BG " /+ /" COLOR_RESET,
                      COLOR_SPURPLE COLOR_YELLOW_BG "| |/ " COLOR_RESET,
                      COLOR_SPURPLE COLOR_YELLOW_BG "| K  " COLOR_RESET,
                      COLOR_SPURPLE COLOR_YELLOW_BG "| |\\ " COLOR_RESET,
                      COLOR_SPURPLE COLOR_YELLOW_BG " \\+ \\" COLOR_RESET};

    sprite EMPTY = {"     ","     ","     ","     ","     "};

    //Menu Variables
    int Active = 1;
    //Set DebugMode to 0 before submitting
    int DebugMode = 0;
    int MenuScreen = 1;
    int InGame = 0;
    char Input = ' ';
    int xInput = 0, yInput = 0;
    char waitInput = ' ';
    int invalidInputMsg = 0;
    int oldVal = 0;
    int RWins = 0;
    int BWins = 0;
    int Draws = 0;
    Coord cInput;

    while (Active)
    {
        while (MenuScreen)
        {
            printf("Welcome to " COLOR_SRED "RED" COLOR_RESET " vs " COLOR_SBLUE "BLUE" COLOR_RESET "!\n");
            if (RWins || BWins || Draws)
            {
                printf("Win Tally\n"COLOR_SRED"RED: %d"COLOR_RESET"\n"COLOR_SBLUE"BLUE: %d"COLOR_RESET"\n"COLOR_SPURPLE"DRAWS: %d"COLOR_RESET"\n",RWins,BWins,Draws);
            }

            printf("[S] - Start a Match\n");
            printf("[H] - How to Play\n");
            printf("[X] - Exit the Program\n");
            printf("Input: ");
            scanf(" %c",&Input);

            if (toupper(Input) == 'S')
            {
                MenuScreen = 0;
                InGame = 1;
                InitializeValues(&R,&B,&S,&T,&good,&go,&start,&found,&val,result,&over,V);
            }
            else if (toupper(Input) == 'H')
            {
                printf(CLEAR);
                printf(COLOR_PURPLE "\n===========================================================\n" COLOR_RESET);
                printf(COLOR_PURPLE "||" COLOR_RESET "             " COLOR_SRED COLOR_YELLOW_BG " HOW TO PLAY: RED VS BLUE " COLOR_RESET "              " COLOR_PURPLE "  ||\n" COLOR_RESET);
                printf(COLOR_PURPLE "===========================================================\n\n" COLOR_RESET);

                printf(COLOR_SPURPLE "GOAL\n" COLOR_RESET);
                printf("Dominate the 3x3 grid! The player with the most territory\n");
                printf("when the game ends is the winner.\n\n");

                printf(COLOR_SPURPLE "RULES\n" COLOR_RESET);
                printf("1. On Turn 1, players pick any space to drop\n");
                printf("   their first " COLOR_SRED COLOR_YELLOW_BG " CHARGED " COLOR_RESET " piece.\n");
                printf("2. On all following turns, you MUST choose\n");
                printf("   a coordinate you " COLOR_SPURPLE "ALREADY OWN" COLOR_RESET ".\n");
                printf("   - Choosing a " COLOR_BLUE "Normal" COLOR_RESET " piece upgrades it to " COLOR_SBLUE COLOR_YELLOW_BG " Charged " COLOR_RESET ".\n");
                printf("   - Choosing a " COLOR_SRED COLOR_YELLOW_BG " Charged " COLOR_RESET " piece causes it to " COLOR_SRED "EXPAND!" COLOR_RESET "\n\n");

                printf(COLOR_SPURPLE "THE TWIST\n" COLOR_RESET);
                printf("When a Charged piece expands, it shoots into adjacent cells:\n");
                printf(" > " COLOR_SRED "RED " COLOR_RESET "pieces expand strictly " COLOR_SRED "UP, LEFT, and RIGHT." COLOR_RESET "\n");
                printf(" > " COLOR_SBLUE "BLUE " COLOR_RESET "pieces expand strictly " COLOR_SBLUE "DOWN, LEFT, and RIGHT." COLOR_RESET "\n\n");

                printf(" - Expanding into an empty space claims it as a Normal piece.\n");
                printf(" - Expanding into an enemy captures it as a Charged piece.\n");
                printf(" - Expanding into a Charged piece triggers a " COLOR_SRED "CHAIN REACTION!" COLOR_RESET "\n\n");

                printf(COLOR_SPURPLE "GAME OVER CONDITIONS\n" COLOR_RESET);
                printf("The battle ends immediately if:\n");
                printf(" 1. There are exactly 3 empty spaces left on the board.\n");
                printf(" 2. 20 total turns have passed.\n");
                printf(" 3. One player is completely wiped off the board.\n\n");

                printf(COLOR_PURPLE "===========================================================\n" COLOR_RESET);
                printf("Enter anything to return to Menu: ");
                scanf(" %c", &waitInput);
                printf(CLEAR);
            }
            else if (toupper(Input) == 'D')
            {
                DebugMode = !DebugMode;

                if (DebugMode)
                {
                    printf(COLOR_PURPLE "Debug Mode On" COLOR_RESET "\n");
                }
                else
                {
                    printf(COLOR_PURPLE "Debug Mode Off" COLOR_RESET "\n");
                }
            }
            else if (toupper(Input) == 'X')
            {
                MenuScreen = 0;
                Active = 0;
            }
        }

        //Temporary UI
        while (InGame)
        {
            if (DebugMode)
            {
                //Does not clean up previous lines to make debugging easier
                printf("Good: %d, Go: %d, Start: %d, Found: %d, Val: %d, Over: %d\n", good, go, start, found, val, over);
                printf("Board State:\n");
                printBoardDebug(size, &R, &B, &S, &T);
            }
            else
            {
                //Cleans up previous lines
                printf(CLEAR);
                printf("Board State:\n");
                printBoard(size, &R, &B, &S, &T, RED, SRED, BLUE, SBLUE, PURPLE, SPURPLE, EMPTY);
            }

            if (go)
            {
                printf(COLOR_SRED "RED" COLOR_RESET " Player's Turn\n");
            }
            else
            {
                printf(COLOR_SBLUE "BLUE" COLOR_RESET " Player's Turn\n");
            }

            if (invalidInputMsg)
            {
                if (invalidInputMsg == 1)
                {
                    printf("Invalid Position, (%d,%d).\nPlease enter numbers between 1 and %d.\n",xInput,yInput,size);
                }
                else if (invalidInputMsg == 2)
                {
                    if (go)
                    {
                        printf("Invalid Choice, (%d,%d).\nPlease enter a position " COLOR_SRED "YOU" COLOR_RESET " control.\n",xInput,yInput);
                    }
                    else
                    {
                        printf("Invalid Choice, (%d,%d).\nPlease enter a position " COLOR_SBLUE "YOU" COLOR_RESET " control.\n",xInput,yInput);
                    }
                }

                invalidInputMsg = 0;
            }

            printf("Input Row: ");
            scanf("%d", &xInput);
            printf("Input Column: ");
            scanf("%d", &yInput);
            cInput.x = xInput;
            cInput.y = yInput;

            if (isValidPos(cInput))
            {
                oldVal = val;
                NextPlayerMove(cInput, &start, &go, &good, &found, &val, &R, &B, &S, &T);
                //If the turn counter does not increment, an existing position was chosen but it was invalid for the current player to use.
                if(val == oldVal)
                {
                    invalidInputMsg = 2;
                }
            }
            else
            {
                invalidInputMsg = 1;
            }
             // Check for game over after move
            over = isOver(R, B, start, val);

            if (over) 
            {
                //Show the board one last time
                if (DebugMode) 
                {
                    printf("Board State:\n");
                    printBoardDebug(size, &R, &B, &S, &T);
                } 
                else 
                {
                    printf(CLEAR);
                    printf("Board State:\n");
                    printBoard(size, &R, &B, &S, &T, RED, SRED, BLUE, SBLUE, PURPLE, SPURPLE, EMPTY);
                }

                GameOver(&R,&B,result);

                if (!strcmp(result,"R wins")) 
                {
                    printf("Game Over! " COLOR_SRED "Player 1" COLOR_RESET " wins with more figures!\n");
                    RWins++;
                }
                else if(!strcmp(result,"B wins"))
                {
                    printf("Game Over! " COLOR_SBLUE "Player 2" COLOR_RESET " wins with more figures!\n");
                    BWins++;
                }
                else
                {
                    printf("Game Over! It's a " COLOR_SPURPLE "Draw!" COLOR_RESET " - equal figures!\n");
                    Draws++;
                }

                printf("Enter anything to return to Menu: ");
                scanf(" %c", &waitInput);
                InGame = 0;  // Exit the game loop
                MenuScreen = 1;
            }
        }
    }

    return 0;
}
