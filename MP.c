#include <stdio.h>
#include <string.h>

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
    int R[size][size];
    int B[size][size];
    int S[size][size];
    int T[size][size];
    int F[size][size];

    return 0;
}