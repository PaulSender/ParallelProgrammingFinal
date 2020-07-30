#include <stdio.h>
#include "mpi.h"

int main (int argc, char **argv){

	int numranks, rank, rc;

	// initialize MPI
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numranks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);


	MPI_Finalize();

}
