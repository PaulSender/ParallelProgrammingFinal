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
void bubbleSort(int arr[], int split) 
{ 
   int i, j; 
   for (i = 0; i < split-1; i++)       
       // Last i elements are already in place    
       for (j = 0; j < split-i-1; j++)  
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

void main (int argc, char **argv){

	int numranks, rank, rc;
	// initialize MPI
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numranks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if(rank == 0){
		if(argc < 2){
     			printf("Please give the number of elements you would like to sort and try again.\n");
			MPI_Finalize();
        	
    	}
	}
	double startTime = MPI_Wtime();    	
   	int n = atoi(argv[1]);
   	int *arr = (int *)malloc(n*sizeof(int));
	//Creating Values
	//Decided it would be faster for every rank to do this work
	//vs. send it over the communicator. Maybe not tho we can see
    	for(int i = 0; i < n; i++){
        	int random_number = rand() % 100 + 1;
        	arr[i] = random_number;
        	}
	
	//Splitting up the work so rank 0 does nothing.
	int split = n / (numranks-1);
	int myStart;
	int myEnd;
	if(rank == 0){
		myStart = 0;
		myEnd = 0;
	}
	else{
		myStart = (rank-1) * split;
		myEnd = rank *split -1;
	}
	//Creating temp array
	int* t =(int*)malloc(split*sizeof(int));

	if(rank == numranks -1){
		myEnd = n;
	}
	//if(rank == 0){
	//	printf("Starting array: \n");
	//	printArray(arr, n); 
	//}
	//Assigning correct values to temp array
	int c = 0;
	for(int i = myStart; i <= myEnd; i++){
		t[c] = arr[i];
		c++;
	}
	//checks

	int *revArr = (int *)malloc(numranks*sizeof(int));
	int *disp = (int *)malloc(numranks*sizeof(int));
	revArr[0] = 0;
	disp[0] = 0;
	disp[1] = 0;
	disp[2] = 5;
	for(int i = 1; i < numranks; i++){
		revArr[i] = split;
	}
	//for(int j = 1; j< numranks; j++){
	//	disp[j] = rank-1 * split;
	//}
	bubbleSort(t, split);
 
	printf("After Sort from Rank: %d\n", rank);
        printArray(t, split);

	free(arr);
	MPI_Gatherv(t, split, MPI_INT, arr, revArr, disp, MPI_INT, 0, MPI_COMM_WORLD);
	
	if(rank == 0){
		printf("After gather: \n");
		printArray(arr,n);

		//bubbleSort(arr,n);

		//printf("Sorted array: \n"); 
    		//printArray(arr, n);
		double endtime = MPI_Wtime();
		printf("NumRanks: %d, Time: %.5f\n",numranks, endtime-startTime); 
	}

    free(arr);
    MPI_Finalize();  
   }
