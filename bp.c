#include <stdio.h> 
#include <stdlib.h>
#include "mpi.h"

void swap(int *xp, int *yp) 
{ 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 
  
// A function to implement bubble sort 
void bubbleSort(int arr[], int myStart, int myEnd) 
{ 
   int i, j; 
   for (i = myStart; i < myEnd-1; i++)       
       // Last i elements are already in place    
       for (j = 0; j < myStart-i-1; j++)  
           if (arr[j] > arr[j+1]) 
              swap(&arr[j], &arr[j+1]); 
} 
  
/* Function to print an array */
void printArray(int arr[], int size) 
{ 
    int i; 
    for (i=0; i < size; i++) 
        printf("%d ", arr[i]); 
    printf("\n"); 
}

int main (int argc, char **argv){

	int numranks, rank, rc;
	// initialize MPI
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numranks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if(rank == 0){
		if(argc < 2){
     		printf("Please give the number of elements you would like to sort and try again.\n");
			MPI_Finalize();
        	return 0;
    	}
	}
    	
    int n = atoi(argv[1]);
    int *arr = (int *)malloc(n*sizeof(int));
	//Creating Values
	//Decided it would be faster for every rank to do this work
	//vs. send it over the communicator. Maybe not tho we can see
    for(int i = 0; i < n; i++){
        int random_number = rand() % 100 + 1;
        arr[i] = random_number;
        }
	}
	//Splitting up the work.
	int split = n / numranks;
	int myStart = rank * split;
	int myEnd = (rank+1) * split -1 ;
	if(rank == numranks -1){
		myEnd = n;
	}
	if(rank == 0){
		printf("Starting array: \n");
		printArray(arr, n); 
	}
    
    bubbleSort(arr, myStart, myEnd); 

	MPI_Gather(arr, split, MPI_INT, arr, split, MPI_INT, 0, MPI_COMM_WORLD);

	if(rank == 0){
		printf("Sorted array: \n"); 
    	printArray(arr, n); 
	}
    
    return 0; 
    }
	
	MPI_Finalize();

}
