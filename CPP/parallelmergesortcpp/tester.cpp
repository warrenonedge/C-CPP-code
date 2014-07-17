
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <mpi.h>

int *merge(int *A, int asize, int *B, int bsize);
void m_sort(int *A, int min, int max);

int *merge(int *A, int asize, int *B, int bsize) {
	int ai, bi, ci, i;
	int *C;
	int csize = asize+bsize;

	ai = 0;
	bi = 0;
	ci = 0;

	C = (int *)malloc(csize*sizeof(int));
	while ((ai < asize) && (bi < bsize)) {
		if (A[ai] <= B[bi]) {
			C[ci] = A[ai];
			ci++; ai++;
		} else {
			C[ci] = B[bi];
			ci++; bi++;
		}
	}

	if (ai >= asize)
		for (i = ci; i < csize; i++, bi++)
			C[i] = B[bi];
	else if (bi >= bsize)
		for (i = ci; i < csize; i++, ai++)
			C[i] = A[ai];

	for (i = 0; i < asize; i++)
		A[i] = C[i];
	for (i = 0; i < bsize; i++)
		B[i] = C[asize+i];
	return C;
}

void m_sort(int *A, int min, int max)
{
	int *C;
	int mid = (min+max)/2;
	int lowerCount = mid - min + 1;
	int upperCount = max - mid;

	if (max == min) {
		return;
	} else {
		m_sort(A, min, mid);
		m_sort(A, mid+1, max);
		C = merge(A + min, lowerCount, A + mid + 1, upperCount);
	}
}

main(int argc, char **argv)
{
	int * data;
	int * chunk;
	int * other;
	int m,n,scanner;
	int id,p,r;
	int s = 0;
	int i;
	int count;
	MPI_Status status;
	FILE *infile,*outfile,*outfile2;
	double start,end;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	MPI_Comm_size(MPI_COMM_WORLD,&p);

	if(id==0)
	{
		infile = fopen(argv[1],"r");
        fscanf(infile,"%d",&n);
		srandom(clock());
		s = n/p;
		r = n%p;
        data = (int*) calloc (n+s-r,sizeof(int));
        for (i=0;i<n+s-r;i++)
        {
            fscanf(infile,"%d",&scanner);
            data[i]=scanner;
        }
        fclose(infile);
		if(r!=0)
		{
			for(i=n;i<n+s-r;i++)
				data[i]=0;
			s=s+1;
		}
	}
	start = MPI_Wtime();
	MPI_Bcast(&s,1,MPI_INT,0,MPI_COMM_WORLD);
	chunk = (int *)malloc(s*sizeof(int));
	MPI_Scatter(data,s,MPI_INT,chunk,s,MPI_INT,0,MPI_COMM_WORLD);
	m_sort(chunk, 0, s-1);
	

	count = 1;
	while(count<p)
	{
		if(id%(2*count)==0)
		{
			if(id+count<p)
			{
				MPI_Recv(&m,1,MPI_INT,id+count,0,MPI_COMM_WORLD,&status);
				other = (int *)malloc(m*sizeof(int));
				MPI_Recv(other,m,MPI_INT,id+count,0,MPI_COMM_WORLD,&status);
				chunk = merge(chunk,s,other,m);
				s = s+m;
			} 
		}
		else
		{
			int near = id-count;
			MPI_Send(&s,1,MPI_INT,near,0,MPI_COMM_WORLD);
			MPI_Send(chunk,s,MPI_INT,near,0,MPI_COMM_WORLD);
			break;
		}
		count *= 2;
	}

	end = MPI_Wtime();
	if (id == 0)
	{   
		char *dummy;
	    float seqTime;
	    outfile = fopen("time.out","a+");
	    outfile2 = fopen("output.out","w");
	    fscanf(outfile,"%s",&dummy);
	    fscanf(outfile,"%s",&dummy);
	    fscanf(outfile,"%f",&seqTime);
	    fprintf(outfile, "\nNumber of Processes: %d \n",p);
	    fprintf(outfile, "MPI Time: %f \n", (end-start));
	    fprintf(outfile, "Speedup Factor: %f \n", (seqTime/(end-start)));
        for(int i=0;i<s;i++)
            fprintf(outfile2, "%d\n", chunk[i]);
        fclose(outfile);
        fclose(outfile2);
	}
	MPI_Finalize();
	return 0;
}
