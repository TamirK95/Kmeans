#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


int countingCoordinates(char str[]);
int isConverged(double **clusters1, double **clusters2, int k, int numOfCords);
void copyArr1ToArr2(double **arr1, double **arr2, int k, int numOfCords);
void makeAllZero(double **currClusters, int k , int numOfCords);
void printFinalRes(double **res, int k, int numOfCords);
void freeSpace (double **arr, int i);
int checkIfInteger(char n[]);
void callError();
void freeAndExit();


int main(int argc, char *argv[])
{
    const char s[2] = ",";
    char *cord;
    char* DP = (char*) malloc(1024 * sizeof(char));
    char* firstDP = (char*) malloc(1024 * sizeof(char));
    int t, i=1, a, b, k, numOfCords, size=500, j=0, total=0, iterCounter=0, maxIter=200, *countersArr;
    double **tempDPs, **currClusters, **oldClusters, **DPs;
    if(argc != 2 && argc !=3)
    {
        callError();
        free(DP);
        free(firstDP);
        return 0;
    }
    if(!checkIfInteger(argv[1]))
    {
        freeAndExit(DP, firstDP);
        return 0;
    }
    k=atoi(argv[1]);
    if(argc==3)  /*maxIter is given*/
    {
        if(!checkIfInteger(argv[2]))
        {
            freeAndExit(DP, firstDP);
            return 0;
        }
        maxIter=atoi(argv[2]);
    }
    else if (argc != 2)
    {
        freeAndExit(DP, firstDP);
        return 0;
    }
    if (k<=0 || maxIter<=0)
    {
        freeAndExit(DP, firstDP);
        return 0;
    }
    scanf("%s",firstDP);
    numOfCords = countingCoordinates(firstDP);
    DPs = (double **) malloc(500 * sizeof(double *));
    assert(DPs != NULL);
    for (t = 0; t < size; t++) 
    {
        DPs[t] = (double *) malloc(numOfCords * sizeof(double));
        assert(DPs[t] != NULL);
    }
    /*taking care of first DP:*/
    cord = strtok(firstDP, s);
    while (cord!=NULL)
        {
            double fcord = (double)atof(cord);
            DPs[0][j] = fcord;
            j++;
            cord = strtok(NULL,s);
            total++;
        }
    /*now for all the rest DPs:*/
    while((scanf("%s", DP) == 1))
    {
        if(i+1==size)  /*need bigger DPs array*/
        {
            size=size*2;
            tempDPs = (double **) malloc(size * sizeof(double *));  /*made new 2d array*/
            assert(tempDPs != NULL);
            for (t = 0; t < size; t++) 
            {
                tempDPs[t] = (double *) malloc(numOfCords * sizeof(double));
                assert(tempDPs[t] != NULL);
            }
            /*now to copy all members of DPs:*/
            for(a=0; a<size/2; a++)
            {
                for(b=0; b<numOfCords; b++)
                {
                    tempDPs[a][b] = DPs[a][b];
                }
            }
            freeSpace(DPs, (size/2));
            DPs = (double **) malloc(size * sizeof(double *));  /*made new 2d array with correct size (i)*/
            for (t = 0; t < size; t++)
            {
                DPs[t] = (double *) malloc(numOfCords * sizeof(double));
                assert (DPs[t] != NULL);
            }
            for(a=0; a<size; a++)
            {
                for(b=0; b<numOfCords; b++)
                {
                    DPs[a][b] = tempDPs[a][b];
                }
            }
            freeSpace(tempDPs,size);
        }
        j=0;
        cord = strtok(DP, s);
        while (cord!=NULL)
        {
            double fcord = (double)atof(cord);
            DPs[i][j] = fcord;
            j++;
            cord = strtok(NULL,s);
            total++;
        } 
        i++;
    }
    if(k>=i)
    {
        callError();
        return 0;
    }
    /*now to delete not-needed slots in DPs*/
    tempDPs = (double **) malloc(i * sizeof(double *));  /*made new 2d array with correct size (i)*/
    assert(tempDPs != NULL);
    for (t = 0; t < i; t++)
    {
        tempDPs[t] = (double *) malloc(numOfCords * sizeof(double));
        assert (tempDPs[t] != NULL);
    }
    /*now to copy all members of DPs:*/
    copyArr1ToArr2(DPs, tempDPs, i, numOfCords);
    freeSpace(DPs, size);
    DPs = (double **) malloc(i * sizeof(double *));  /*made new 2d array with correct size (i)*/
    for (t = 0; t < i; t++)
    {
        DPs[t] = (double *) malloc(numOfCords * sizeof(double));
        assert (DPs[t] != NULL);
    }
    copyArr1ToArr2(tempDPs, DPs, i, numOfCords);
    freeSpace(tempDPs,i);
    /*made the convertion, array is in the right size. now DPs contains all DPs, creating array for clusters*/
    currClusters = (double **) malloc(k * sizeof(double *));
    oldClusters = (double **) malloc(k * sizeof(double *));
    assert(currClusters != NULL);
    assert(oldClusters != NULL);
    for (t = 0; t < k; t++) 
    {
        currClusters[t] = (double *) malloc(numOfCords * sizeof(double));
        oldClusters[t] = (double *) malloc(numOfCords * sizeof(double));
        assert(currClusters[t] != NULL);
        assert(oldClusters[t] != NULL);
    }
    copyArr1ToArr2(DPs, currClusters, k, numOfCords);
    makeAllZero(oldClusters, k, numOfCords);
    /*now to compute the exact clusters, by the algo*/
    while((!isConverged(currClusters, oldClusters, k, numOfCords)) && (iterCounter<maxIter))
    {
        copyArr1ToArr2(currClusters, oldClusters, k, numOfCords);
        iterCounter++;
        countersArr = (int*) malloc(k * sizeof(int));
        for(t=0; t<k; t++)
        {
            countersArr[t]=0;
        }
        makeAllZero(currClusters, k , numOfCords);     
        for(t=0; t<i; t++)
        {
            double * DPoint = DPs[t];  /*the point ill search min for*/
            double minDistance=__FLT_MAX__;
            int closestClusterIndx = 0;
            for(a=0; a<k; a++)
            {
                double * cluster = oldClusters[a];
                double minCandidate = 0;
                for(b=0; b<numOfCords; b++)
                {
                    double x = DPoint[b]-cluster[b];
                    minCandidate+=x*x;
                }
                if(minCandidate<minDistance)
                {
                    closestClusterIndx = a;
                    minDistance = minCandidate;
                }
            }
            countersArr[closestClusterIndx]++;
            for(a=0; a<numOfCords;a++)
            {
                currClusters[closestClusterIndx][a]+=DPs[t][a];
            }

        }
        /*now to divide by |s_i|*/
        for(t=0;t<k;t++)
        {
            for(a=0;a<numOfCords;a++)
            {
                currClusters[t][a] = (currClusters[t][a]/countersArr[t]);
            }
        }
    }
    free(firstDP);
    free(DP);
    freeSpace(DPs, i);
    freeSpace(oldClusters,k);
    free(countersArr);
    printFinalRes(currClusters, k, numOfCords);
    freeSpace(currClusters,k);   
    return 1;
}


/*
* Function: CountingCoordinates
* -----------------------------
* Returns number of coordinates in given string.
*
* str - the given string.
*/
int countingCoordinates(char str[])
{
    int numOfCords = 1;
    char *ch=strchr(str,',');
    while (ch!=NULL) 
    {
        numOfCords++;
        ch=strchr(ch+1,',');
    }
    return numOfCords;
}


/*
* Function: isConverged
* -----------------------------
* Check if clusters1 equals to clusters2, return True if so, or False if not.
*
* clusters1 - first cluster.
* clusters1 - second cluster.
* k - number of data points in each cluster.
* numOfCords - number of coordinates in each data point.
*/
int isConverged(double **clusters1, double **clusters2, int k, int numOfCords)
{
    int i;
    int j;
    for(i=0; i<k; i++)
    {
        for(j=0; j<numOfCords; j++)
        {
            if(clusters1[i][j]!=clusters2[i][j])
            {
                return 0;
            }
        }
    }
    return 1;
}


/*
* Function: copyArr1ToArr2
* -----------------------------
* Copy the contents of arr1 into arr2.
*
* arr1 - first array (the one which we will copy from).
* arr2 - second array (the one which we will copy to).
* k - number of data points in each array (cluster).
* numOfCords - number of coordinates in each data point.
*/
void copyArr1ToArr2(double **arr1, double **arr2, int k, int numOfCords)
{
    int i;
    int j;
    for(i=0; i<k; i++)
    {
        for(j=0; j<numOfCords; j++)
        {
            arr2[i][j] = arr1[i][j];
        }
    }
}


/*
* Function: makeAllZero
* -----------------------------
* Turn all slots of currClusters to zero.
*
* currClusters - the given cluster which will turn to zero-cluster.
* k - number of data points in currClusters.
* numOfCords - number of coordinates in each data point.
*/
void makeAllZero(double **currClusters, int k , int numOfCords)
{
    int i;
    int j;
    for(i=0; i<k; i++)
    {
        for(j=0; j<numOfCords; j++)
        {
            currClusters[i][j] = 0;
        }
    }
}


/*
* Function: printFinalRes
* -----------------------------
* Print the coordinates as needed for the exercise.
*
* res - the result cluster.
* k - number of data points in res.
* numOfCords - number of coordinates in each data point.
*/
void printFinalRes(double **res, int k, int numOfCords)
{
    int t;
    int p;
    for(t=0;t<k;t++)
    {
        for(p=0;p<numOfCords;p++)
        {
            if(p!=numOfCords-1)
            {
                printf("%.4f,",res[t][p]);
            }
            else
            {
                printf("%.4f",res[t][p]);
            }
        }
        printf("\n");

    }
}


/*
* Function: freeSpace
* -----------------------------
* Free a given 2D array that was created using malloc, in which wach line was also created using malloc.
*
* arr - the array which will be free.
* i - number of rows in arr.
*/
void freeSpace(double **arr, int i)
{
    int a;
    for(a=0; a<i; a++)
    {
        free(arr[a]);
    }
    free(arr);
}


/*
* Function: checkIfInteger
* -----------------------------
* Check if n is non-negative integer.
*
* n - parameter that may be an integer
*/
int checkIfInteger(char n[])
{
    int i;
    int len = strlen(n);
    for(i=0; i<len; i++ )
    {
        if((n[i] < '0') || (n[i] > '9'))
        {
            return 0;
        }
    }
    return 1;
    
}


/*
* Function: callError
* -----------------------------
* Print error message and return 0.
*/
void callError()
{
    printf("error in input format.\n");
}


/*
* Function: freeAndExit
* -----------------------------
* Free allocated memory for arr1 and arr2, then exit program
*/
void freeAndExit(arr1, arr2)
{
    callError();
    free(arr1);
    free(arr2);
}
