#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* calloc, exit, free */
#include <string.h>
#include <fstream>
#include <time.h>  

using namespace std;


void merger(int *data, int *temp, int firstIdx, int middleIdx, int lastIdx)
{
    int h=firstIdx;
    int i=firstIdx;
    int j=middleIdx+1;

    while((h<=middleIdx)&&(j<=lastIdx))
    {
        if(data[h]<=data[j])
        {
            temp[i]=data[h];
            h++;
        }
        else
        {
            temp[i]=data[j];
            j++;
        }
        i++;
    }
    int k;
    if(h<=middleIdx)
    {
        for(k=h; k<=middleIdx; k++)
        {
            temp[i]=data[k];
            i++;
        }
    }
    else
    {
       for(k=j; k<=lastIdx; k++)
        {
            temp[i]=data[k];
            i++;
        }
    }
    for(k=firstIdx; k<=lastIdx; k++) 
        data[k]=temp[k];
}

void mergesort(int *data, int*temp, int firstIdx, int lastIdx)
{
    int middleIdx;
    if(firstIdx<lastIdx)
    {
        middleIdx=(firstIdx+lastIdx)/2;
        mergesort(data,temp,firstIdx,middleIdx);
        mergesort(data,temp,middleIdx+1,lastIdx);
        merger(data,temp,firstIdx,middleIdx,lastIdx);
    }
}




int main(int argc, char **argv)
{   
    int dataSize, scanner, i;
    FILE *infile, *outfile, *outfile2;
    infile = fopen(argv[1],"r");
    
    fscanf(infile,"%d",&dataSize);
    int *data = (int*) calloc (dataSize,sizeof(int));
    int *temp = (int*) calloc (dataSize,sizeof(int));
    
    for (i=0;i<dataSize;i++)
    {
        fscanf(infile,"%d",&scanner);
        data[i]=scanner;
    }
    
    fclose(infile);
    
    clock_t time = clock();
    mergesort(data, temp, 0, dataSize-1);
    time = clock()-time;
    
    outfile = fopen("time.out","a+");
    outfile2 = fopen("output.out","w");
    float timeElapsed = ((float)time)/CLOCKS_PER_SEC;
    fprintf(outfile, "Sequential Time: %f\n", timeElapsed);
    //printf("%f ms\n",timeElapsed);
    for(int i=0;i<dataSize;i++)
        fprintf(outfile2, "%d ", data[i]);
    fclose(outfile);
    fclose(outfile2);
    return 1;
}
