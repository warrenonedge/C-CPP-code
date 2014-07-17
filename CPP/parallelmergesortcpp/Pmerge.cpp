#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <mpi.h>

int *merge(int *x, int sizeOfx, int *y, int sizeOfy);
void main_sort(int *x, int low, int high);

void main_sort(int *x, int low, int high)
{
    int *temp_array;
    int mid = (low+high)/2;
    int lowCount = mid - low + 1;
    int highCount = high - mid;
    
    if (high == low)
        return;
    else
    {
        main_sort(x, low, mid);
        main_sort(x, mid+1, high);
        temp_array = merge(x + low, lowCount, x + mid + 1, highCount);
    }
}

int *merge(int *x, int sizeOfx, int *y, int sizeOfy)
{
    int xiter = 0;
    int yiter = 0;
    int ziter = 0;
    int i;
    int *z;
    int sizeOfz = sizeOfx + sizeOfy;
    
    z = (int*) calloc (sizeOfz,sizeof(int));
    while ((xiter < sizeOfx) && (yiter < sizeOfy))
    {
        if (x[xiter] > y[yiter])
        {
            z[ziter] = y[yiter];
            ziter+=1;
            yiter+=1;
        }
        else
        {
            z[ziter] = x[xiter];
            ziter+=1;
            xiter+=1;
        }
    }
    
    if (xiter >= sizeOfx)
    {
        for (i = ziter; i < sizeOfz; i++)
        {
            z[i] = y[yiter];
            yiter+=1;
        }
    }
    else if (yiter >= sizeOfy)
    {
        for (i = ziter; i < sizeOfz; i++)
        {
            z[i] = x[xiter];
            xiter+=1;
        }
    }
    for (i = 0; i < sizeOfx; i++)
        x[i] = z[i];
    for (i = 0; i < sizeOfy; i++)
        y[i] = z[sizeOfx+i];
    return z;
}

int main(int argc, char **argv)
{
    int num_proc;
    int id;
    int dataSize, scanner, i, j, count;
    int num_per_proc = 0;
    int *data, *section, *temp;
    double start;
    double end;
    FILE *infile, *outfile, *outfile2;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_size (MPI_COMM_WORLD, &num_proc);
	MPI_Comm_rank (MPI_COMM_WORLD, &id);
	if (id == 0)
	{
	    infile = fopen(argv[1],"r");
        fscanf(infile,"%d",&dataSize);
        data = (int*) calloc (dataSize,sizeof(int));
        for (i=0;i<dataSize;i++)
        {
            fscanf(infile,"%d",&scanner);
            data[i]=scanner;
        }
        fclose(infile);
        start = MPI_Wtime();
        num_per_proc = dataSize/num_proc;
	}
	MPI_Bcast(&num_per_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);
    section = (int*) calloc (num_per_proc,sizeof(int));
    MPI_Scatter(data, num_per_proc, MPI_INT, section, num_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
    main_sort(section, 0, num_per_proc-1);
	
	count = 1;
	while(count<num_proc)
	{
	    int i = 2*count;
	    if (id % i != 0)
	    {
	        int close = id-count;
	        MPI_Send(&num_per_proc, 1, MPI_INT, close, 0, MPI_COMM_WORLD);
	        MPI_Send(section, num_per_proc, MPI_INT, close, 1, MPI_COMM_WORLD);
	        break;
	    }
	    else
	    {
	        if (id+count<num_proc)
	        {
	            MPI_Recv(&j, 1, MPI_INT, id+count, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	            temp = (int*) calloc (j,sizeof(int));
	            MPI_Recv(temp, j, MPI_INT, id+count, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	            section = merge(section,num_per_proc,temp,j);
	            num_per_proc += j;
	        }
	    }
	    count *= 2;
	}
	end = MPI_Wtime();
	
	if (id == 0)
	{   
	    float seqTime;
	    outfile = fopen("time.out","a+");
	    outfile2 = fopen("output.out","w");
	    fscanf(outfile,"%f",&seqTime);
	    fprintf(outfile, "MPI Time: %f ", (end-start));
	    fprintf(outfile, "Speedup Factor: %f ", (seqTime/(end-start)*1000));
        for(int i=0;i<dataSize;i++)
            fprintf(outfile2, "%d ", data[i]);
        fclose(outfile);
        fclose(outfile2);
	}
	MPI_Finalize();
	return 0;
}
