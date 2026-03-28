#include <stdio.h>
#include <string.h>

#define MAX_ELEM 9

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
    return (pos.x >= 1 && pos.x <= 3 && pos.y >= 1 && pos.y <= 3);
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

// checks if the game is over; returns 1 if over, and 0 if not
int isOver(Set R, Set B, int start, int val)
{
    int F_size; // cardinality of F
    int i;
    int n = B.num_coord;

    // since R and B will never share the same positions/coordinates, size of F can be computed as:
    F_size = MAX_ELEM - (R.num_coord + B.num_coord);

    return (F_size == 3 || val >= 20 || (!start && ((R.num_coord > 0 && B.num_coord == 0) || (R.num_coord == 0 && B.num_coord > 0))));
}

/*
    Purpose: prints the current board state to the terminal
    Return: none
    Example: 
*/ 
void printBoard(int size, Set *R,Set *B, Set *S, Set *T)
{
    int i, j;
    Coord temp;
    char cR;
    char cB;
    char cS;
    char cT;

    for (i = 1; i < size + 1; i++){
        for(j = 1; j < size + 1; j++){
            temp.x = j;
            temp.y = i;
            cR = ' ';
            cB = ' ';
            cS = ' ';
            cT = ' ';
            if (isElement(*R, temp)) cR = 'R';
            if (isElement(*B, temp)) cB = 'B';
            if (isElement(*S, temp)) cS = 'S';
            if (isElement(*T, temp)) cT = 'T';
            printf("%c%c%c%c", cS, cR, cB, cT);
            if (j < size){
                printf("|");
            }
        }
        if(i < size){
            printf("\n--------------\n");
        }
    }
    printf("\n");
}

// MAIN FUNCTIONS 
void Remove(Coord pos, int *go, Set *R, Set *B, Set *S, Set *T)
{
    if (*go)
        removeCoord(R, pos);
    else
        removeCoord(B, pos);
    
    removeCoord(S, pos);
    removeCoord(T, pos);
}

void Expand(Coord pos, int *go, int *found, Set *R, Set *B, Set *S, Set *T);

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
    Example: 
*/ 
void NextPlayerMove(Coord pos, int *start, int *go, int *good, int *found, int *val, Set *R, Set *B, Set *S, Set *T)
{   
    int over = isOver(*R, *B, *start, *val);

    if (!over){
        if (*start){ 
            if (*go){
                addCoord(R, pos);
                addCoord(S, pos);
                *good = 1;
            }else{
                addCoord(B, pos);
                addCoord(S, pos);
                *good = 1;
            }
        }else{
            if ((*go && isElement(*R, pos)) || (!*go && isElement(*B, pos)))
            {
                Update(pos, go, good, found, R, B, S, T);
                *good = 1;
            }
        }
    }
    if (*start && R->num_coord > 0 && B->num_coord > 0){
        *start = 0;
    }
    //Im not sure if the order the equations appear in matter or not so I'll just give this its own !over if statement
    if (!over && *good){
        *good = !*good;
        *go = !*go;
        (*val)++;
    }
}


int main(){
    int i, j;

    //Applicable Sets
    //Set C
    int C[3] = {1,2,3};
    //Set M
    int size = sizeof(C)/sizeof(int);
    Set M = {0};
    Coord temp;
    for(i=1;i<size+1;i++){
        for(j=1;j<size+1;j++){
            temp.x = j;
            temp.y = i;
            addCoord(&M,temp);
        }
    }
    //Set N
    int N[17] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
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

    //Student Initialized
    int over = 0;
    
    Set R = {0};
    Set B = {0};
    Set S = {0};
    Set T = {0};
    Set F;

    //Menu Variables
    int Active = 1;
    int MenuScreen = 1;
    int InGame = 0;
    char Input = ' ';
    int xInput = 0, yInput = 0;
    Coord cInput;
    while(Active){
        while(MenuScreen){
            printf("Welcome to the Game! {Title Screen in Progress}\n");
            printf("[S] - Start a Match\n");
            printf("[X] - Exit the Program\n");
            printf("Input: ");
            scanf(" %c",&Input);
            if (Input == 'S'){
                MenuScreen = 0;
                InGame = 1;
            }else if (Input == 'X'){
                MenuScreen = 0;
                Active = 0;
            }
        }
        //Temporary UI
        while(InGame){
            printf("Board State:\n");
            printBoard(size, &R, &B, &S, &T);
            printf("Input x: ");
            scanf("%d", &xInput);
            printf("Input y: ");
            scanf("%d", &yInput);
            cInput.x = xInput;
            cInput.y = yInput;
            // printf("%d %d\n",xInput,yInput);
            // printf("%d %d %d %d\n",xInput>0 , xInput<size+1 , yInput>0 , yInput<size+1);
            // printf("%d %d %d\n",R.num_coord,B.num_coord,S.num_coord);
            if (isValidPos(cInput)){
                NextPlayerMove(cInput, &start, &go, &good, &found, &val, &R, &B, &S, &T);
            }
        }
    }

    return 0;
}