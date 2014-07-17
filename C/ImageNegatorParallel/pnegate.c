/*Damon Edge
 * CSC 473 Assignment 2 
*/
#include "mpi.h"
#include "image.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void process_data (unsigned char *photo, int maxRGB, int localsize)
{
    int i;
    
    for (i = 0; i < localsize; i++)
    {
        photo[i] = negate (photo[i], maxRGB);
    }
}

image *setup (int argc, char **argv)
{
    image *photo;

    if (argc < 3)
    {
        fprintf (stderr, "Usage: %s <infile> <outfile>\n\n", argv [0]);
        return NULL;
    }

    photo = read_image (argv [1]);
    if (photo == NULL)
    {
        fprintf (stderr, "Unable to read input file %s\n\n", argv [1]);
        return NULL;
    }

    return photo;
}

void cleanup (image *photo, char **argv)
{
    int rc = write_image (argv [2], photo);
    if (!rc)
    {
        fprintf (stderr, "Unable to write output file %s\n\n", argv [2]);
    }

    clear_image (photo);
}

int main (int argc, char **argv)
{
	int size;
	int id;
	double t_start;
	double t_end;
	image *photo;
	int localsize, totalsize, maxRGB;
	unsigned char * localdata;
	
	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	MPI_Comm_rank (MPI_COMM_WORLD, &id);

	if (id == 0) {
		photo = setup (argc, argv);
		totalsize = photo->width * photo->height * 3;
		localsize = totalsize / size;
		maxRGB = photo->max_value;
	}
	t_start = MPI_Wtime();
	
	/* broadcast max rgb value */
	MPI_Bcast(&maxRGB, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	/* broadcast local data size */
	MPI_Bcast(&localsize, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	localdata = (unsigned char *) malloc(localsize);
	
	MPI_Scatter(photo->data, localsize, MPI_BYTE, localdata, localsize, MPI_BYTE, 0, MPI_COMM_WORLD);
	
	process_data(localdata, maxRGB, localsize);
	
	MPI_Gather(localdata, localsize, MPI_BYTE, photo->data, localsize, MPI_BYTE, 0, MPI_COMM_WORLD);
	
	t_end = MPI_Wtime();
	
	if (id == 0) {
		cleanup (photo, argv);
		printf("p = %d, elapsed time = %.2lf s.", size, t_end - t_start);
	}	
	
	MPI_Finalize ();
	return 0;
}
