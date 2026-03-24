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
    Purpose: removing a coordinate in a set
    Return: none (modifies the set in the parameter indirectly)
    Example: A = {(1,2), (3,4)} & num_coord = 2, remove (1,2) in Set A, then:
             A = {(3,4)} and num_coord = 1
*/ 
void
removeCoord(Set *A, Coord pos)
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
    Example: A = {(1,2), (3,4)} & num_coord = 2, add (5,6) in Set A, then:
             A = {(1,2), (3,4), (5,6)} and num_coord = 3
*/ 
void
addCoord(Set *A, Coord pos)
{
    int n = A->num_coord;

    if (n < MAX_ELEM)
    {
        A->coord[n] = pos;
        A->num_coord++;
    }
}

/*
    Purpose: checks if a given coordinate exists in a set
    Return: either (a) 1 if found; (b) 0 if not
    Example: A = {(1,2), (3,4), (5,6)}, see if (3,4) exists in the set, then:
             this function returns 1
             see if (7,8) exists in the set, then:
             this function returns 0
*/
int
isElement(Set A, Coord pos)
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

// very draft only, not tested
int
isOver(Set R, Set B, int start, int val)
{
    // getting cardinality of F
    Set F;
    int i;
    int n = B.num_coord;

    F = R;

    for (i = 0; i < n; i++)
    {
        if (!isElement(F, B.coord[i]))
            addCoord(&F, B.coord[i]);
    }

    return (F.num_coord == 3 || val >= 20 || (!start && (R.num_coord > 0 && B.num_coord == 0) || (R.num_coord == 0 && B.num_coord > 0)));
}


// MAIN FUNCTIONS 
void
Remove(Coord pos, int go, Set *R, Set *B, Set *S, Set *T)
{
    if (go)
        removeCoord(R, pos);
    else
        removeCoord(B, pos);
    
    removeCoord(S, pos);
    removeCoord(T, pos);
}



int main(){
    int i, j;

    //Applicable Sets
    //Set C
    int C[3] = {1,2,3};
    //Set M
    int size = sizeof(C)/sizeof(int);
    int M[size][size][2];
    for(i=1;i<size+1;i++){
        for(j=1;j<size+1;j++){
            M[i][j][0] = i;
            M[i][j][1] = j;
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
    
    Set R = {0};
    Set B = {0};
    Set S = {0};
    Set T = {0};
    Set F;

    return 0;
}