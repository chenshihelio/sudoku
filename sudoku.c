#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const int N = 9;

const int statInit = 0;
const int statFix = 1;
const int statTry = 2;
const int statUnknown = -1;
const int solFail = 0;
const int solSuccess = 1;

char *fileName = "input.txt";

void writeArr(int arr[N][N]);
void readArr(int arr[N][N], char *fileName);

int solve(int arr[N][N], int stat[N][N], int countPossible[N][N]);
// int solve(int *arr, int *stat, int *countPossible);


int main(int argc, char **argv)
{
    int arr[N][N], // main array
        stat[N][N], // status of each grid point
        countPossible[N][N]; // total number of possible numbers to try  

    // read input file
    readArr(arr,fileName);
    printf("Input matrix is : \n");
    writeArr(arr);

    // set status array
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(arr[i][j]==0)
            {
                stat[i][j] = statUnknown;
            }
            else
            {
                stat[i][j] = statInit;
            }
            countPossible[i][j] = 0;
        }
    }


    // solve
    int resultSol = solve(arr,stat,countPossible);
    // // debug
    // printf("Exit the main solve() function...\n");

    if(resultSol==solFail)
    {
        printf("Cannot find a solution!!!\n");
        return 1;
    }
    else if(resultSol==solSuccess)
    {
        printf("\n\nThe solution to this sodoku is:\n\n");
        writeArr(arr);
    }
}



void readArr(int arr[N][N], char *fileName)
{
    // Open the file for reading
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Read the elements of the array from the file
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fscanf(file, "%d", &arr[i][j]);
        }
    }

    // Close the file
    fclose(file);
}



void writeArr(int arr[N][N])
{
    printf(" -----------------------\n");
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(j==0)
            {
                printf("| %d ",arr[i][j]);
            }
            else if(j==3 || j==6)
            {
                printf("| %d ",arr[i][j]);
            }
            else if(j==8)
            {
                printf("%d | \n",arr[i][j]);
            }
            else
            {
                printf("%d ",arr[i][j]);
            }
        }
        if(i==2 || i==5)
        {
            printf("|-----------------------|\n");
        }
    }
    printf(" -----------------------\n");
}


int sum(int *arr, int n)
{
    int count = 0;
    for(int i=0;i<n;i++)
    {
        count += arr[i];
    }
    return count;
}

void resetIndicator(int *arr, int n)
{
    for(int i=0;i<n;i++)
    {
        arr[i] = 0;
    }
}

int getAllPossibleNumbers(int *indicator, 
    int *possibleNumber, int n)
{
    int count = 0;
    for(int i=0;i<n;i++)
    {
        if(indicator[i]==0)
        {
            possibleNumber[count] = i+1;
            count++;
        }
    }
    return count;
}

int checkWhetherFinish(int stat[N][N])
{
    int numberUnknown = 0;
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(stat[i][j] == statUnknown)
            {
                numberUnknown++;
            }
        }
    }
    if(numberUnknown==0) 
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int solve(int arr[N][N], int stat[N][N], int countPossible[N][N])
{
    int totalUnknown = 0;

    int indicator[N], totalOccupied, totalUnoccupied,
        possibleNumber[N];

    int iBox, jBox;

    int countAllPossibleNumbers;

    int whetherFinish;

    int resultSol;


    // // debug
    // printf("\n\n\n");
    // writeArr(arr);
    // printf("\n\n\n");

    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {  
            if(stat[i][j] == statUnknown)
            {
                // get all possible values
                resetIndicator(indicator,N);
                // scan row
                for(int i1=0;i1<N;i1++)
                {
                    if(stat[i1][j] != statUnknown)
                    {
                        indicator[arr[i1][j] - 1] = 1;
                    }
                }

                // scan column
                for(int j1=0;j1<N;j1++)
                {
                    if(stat[i][j1] != statUnknown)
                    {
                        indicator[arr[i][j1] - 1] = 1;
                    }
                }

                // scan box
                iBox = i/3;
                jBox = j/3;

                for(int i1=0;i1<3;i1++)
                {
                    for(int j1=0;j1<3;j1++)
                    {
                        int i2 = iBox * 3 + i1;
                        int j2 = jBox * 3 + j1;

                        if(stat[i2][j2] != statUnknown)
                        {
                            indicator[arr[i2][j2] - 1] = 1;
                        }
                    }
                }


                // count total un-useful numbers
                totalOccupied = sum(indicator,N);
                totalUnoccupied = N - totalOccupied;

                // //debug
                // printf("i=%d, j=%d, totalOccupied = %d\n", i, j, totalOccupied);

                if(totalUnoccupied==0)
                {
                    // Not possible
                    return solFail;
                }
                else if(totalUnoccupied == 1) // only one possible number
                {
                    countAllPossibleNumbers = getAllPossibleNumbers(indicator,possibleNumber,N);
                    arr[i][j] = possibleNumber[0];
                    stat[i][j] = statTry;
                    countPossible[i][j] = 0;

                    // //debug
                    // printf("i=%d, j=%d, possibility=%d\n",i,j,arr[i][j]);

                    // Must check whether this is the last number to fill
                    whetherFinish = checkWhetherFinish(stat);
                    if(whetherFinish==1) return solSuccess;

                    resultSol = solve(arr,stat,countPossible);
                    if(resultSol == solFail)
                    {
                        // Because the only possibility fails
                        stat[i][j] = statUnknown;
                        arr[i][j] = 0;
                        return solFail;
                    }
                    else if(resultSol == solSuccess)
                    {
                        stat[i][j] = statFix;
                        return solSuccess;
                    }
                }
                else // multiple possible number
                {
                    countAllPossibleNumbers = getAllPossibleNumbers(indicator,possibleNumber,N);
                    countPossible[i][j] = countAllPossibleNumbers;
                }
            }
        }
    }

    // The only reason that the loops exit without returning
    // is that all unknown numbers have multiple choices
    // So, next we need to find the grid with minimal possibilities
    int imin,jmin;
    int countMin = N+1;
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {  
            if((stat[i][j] == statUnknown) && (countPossible[i][j] < countMin))
            {
                countMin = countPossible[i][j];
                imin = i;
                jmin = j;
            }
        }
    }

    // imin and jmin is (one of) the grids with least possibilities
    // get all possible values
    resetIndicator(indicator,N);
    // scan row
    for(int i1=0;i1<N;i1++)
    {
        if(stat[i1][jmin] != statUnknown)
        {
            indicator[arr[i1][jmin] - 1] = 1;
        }
    }

    // scan column
    for(int j1=0;j1<N;j1++)
    {
        if(stat[imin][j1] != statUnknown)
        {
            indicator[arr[imin][j1] - 1] = 1;
        }
    }

    // scan box
    iBox = imin/3;
    jBox = jmin/3;

    for(int i1=0;i1<3;i1++)
    {
        for(int j1=0;j1<3;j1++)
        {
            int i2 = iBox * 3 + i1;
            int j2 = jBox * 3 + j1;

            if(stat[i2][j2] != statUnknown)
            {
                indicator[arr[i2][j2] - 1] = 1;
            }
        }
    }


    // get all possible numbers
    countAllPossibleNumbers = getAllPossibleNumbers(indicator,possibleNumber,N);
    stat[imin][jmin] = statTry;
    for(int ic=0;ic<countAllPossibleNumbers;ic++)
    {
        arr[imin][jmin] = possibleNumber[ic];

        resultSol = solve(arr,stat,countPossible);

        if(resultSol == solSuccess)
        {
            stat[imin][jmin] = statFix;
            return solSuccess;
        }
        if(resultSol == solFail)
        {
            continue;
        }
    }
    
    // // debug...
    // printf("Almost fail.....\n");

    // If exit the previous loop, it means we failed
    stat[imin][jmin] = statUnknown;
    return solFail;
}



// int changeArr(int arr[N][N])
// {
//     arr[2][0] = 9;
//     return 1;
// }
 // for(int i=0;i<N;i++)
    // {
    //     for(int j=0;j<N;j++)
    //     {
    //         arr[i][j] = 0;
    //     }
    // }
    // arr[1][2] = 1;
    // writeArr(arr);

    // changeArr(arr);
    // writeArr(arr);