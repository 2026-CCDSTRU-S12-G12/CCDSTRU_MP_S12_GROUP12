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

/*
    Purpose: prints the current board state to the terminal
    Return: none
    Example: 
*/ 
void printBoard(int size, Set* R,Set *B,Set *S,Set *T){
    int i, j;
    Coord temp;
    char cR;
    char cB;
    char cS;
    char cT;
    for(i=1;i<size+1;i++){
        for(j=1;j<size+1;j++){
            temp.x = j;
            temp.y = i;
            cR = ' ';
            cB = ' ';
            cS = ' ';
            cT = ' ';
            if(isElement(*R,temp)) cR = 'R';
            if(isElement(*B,temp)) cB = 'B';
            if(isElement(*S,temp)) cS = 'S';
            if(isElement(*T,temp)) cT = 'T';
            printf("%c%c%c%c",cS,cR,cB,cT);
            if(j<size){
                printf("|");
            }
        }
        if(i<size){
            printf("\n--------------\n");
        }
    }
    printf("\n");
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

void Replace(Coord pos, int *go, Set *R, Set *B, Set *S, Set *T, Set *M){

}

void Expand(Coord pos, int *go, Set *R, Set *B, Set *S, Set *T, Set *M){

}

void Update(Coord pos, int *go, int* good, Set *R, Set *B, Set *S, Set *T, Set *M){

}

/*
    Purpose: Parses the next player's move
    Return: none
    Example: 
*/ 
void NextPlayerMove(Coord pos, int* over, int* start, int* go, int* good, int* val, Set *R, Set *B, Set *S, Set *T, Set *M){    
    if(!*over){
        if(*start){
            if(*go){
                addCoord(R,pos);
                addCoord(S,pos);
                *good = 1;
            }else{
                addCoord(B,pos);
                addCoord(S,pos);
                *good = 1;
            }
        }else{
            if( (*go && isElement(*R,pos)) || (!*go && isElement(*B,pos))){
                Update(pos,go,good,R,B,S,T,M);
                *good = 1;
            }
        }
    }
    if(*start && R->num_coord > 0 && B->num_coord > 0){
        *start = 0;
    }
    //Im not sure if the order the equations appear in matter or not so I'll just give this its own !over if statement
    if(!*over && *good){
        *good = !*good;
        *go = !*go;
        *val++;
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
            if(Input == 'S'){
                MenuScreen = 0;
                InGame = 1;
            }else if(Input == 'X'){
                MenuScreen = 0;
                Active = 0;
            }
        }
        //Temporary UI
        while(InGame){
            printf("Board State:\n");
            printBoard(size,&R,&B,&S,&T);
            printf("Input x: ");
            scanf("%d",&xInput);
            printf("Input y: ");
            scanf("%d",&yInput);
            cInput.x = xInput;
            cInput.y = yInput;
            // printf("%d %d\n",xInput,yInput);
            // printf("%d %d %d %d\n",xInput>0 , xInput<size+1 , yInput>0 , yInput<size+1);
            // printf("%d %d %d\n",R.num_coord,B.num_coord,S.num_coord);
            if(xInput>0 && xInput<size+1 && yInput>0 && yInput<size+1){
                NextPlayerMove(cInput,&over,&start,&go,&good,&val,&R,&B,&S,&T,&M);
            }
        }
    }

    return 0;
}