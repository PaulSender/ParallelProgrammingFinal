/* C program for Merge Sort */
#include <stdio.h> 
#include <stdlib.h> 
#include "mpi.h"  
// Merges two subarrays of arr[]. 
// First subarray is arr[l..m] 
// Second subarray is arr[m+1..r] 
void merge(int arr[], int l, int m, int r) 
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 = r - m; 
  
    /* create temp arrays */
    int *L = (int *)malloc(n1*sizeof(int));
    int *R = (int *)malloc(n2*sizeof(int)); 
  
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1 + j]; 
  
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = l; // Initial index of merged subarray 
    while (i < n1 && j < n2) { 
        if (L[i] <= R[j]) { 
            arr[k] = L[i]; 
            i++; 
        } 
        else { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    /* Copy the remaining elements of L[], if there 
       are any */
    while (i < n1) { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    /* Copy the remaining elements of R[], if there 
       are any */
    while (j < n2) { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
} 
  
/* l is for left index and r is right index of the 
   sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r) 
{ 
    if (l < r) { 
        // Same as (l+r)/2, but avoids overflow for 
        // large l and h 
        int m = l + (r - l) / 2; 
  
        // Sort first and second halves 
        mergeSort(arr, l, m); 
        mergeSort(arr, m + 1, r); 
  
        merge(arr, l, m, r); 
    } 
} 
  
/* UTILITY FUNCTIONS */
/* Function to print an array */
void printArray(int A[], int size) 
{ 
    int i; 
    for (i = 0; i < size; i++) 
        printf("%d ", A[i]); 
    printf("\n"); 
} 
  
/* Driver program to test above functions */
int main(int argc, char **argv) 
{
   int numranks, rank;
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
   if(rank == 0){
   	for(int i = 0; i < n; i++){
   		int random_number = rand() % 100 + 1;
        	arr[i] = random_number;
   	}
   }
   //Giving every rank the array
   MPI_Bcast(arr, n, MPI_INT, 0, MPI_COMM_WORLD);

   //Splitting up the work.
   int split = n / numranks;
   int myStart = rank * split;
   int myEnd = (rank+1) * split -1;
   if(rank == numranks-1){
	myEnd = n;
   }
   printf("Rank: %d, split: %d, myStart: %d, myEnd: %d\n",rank, split, myStart, myEnd);
   //Creating and populating temp array
   int *t =(int *)malloc(split*sizeof(int));
   int c = 0;
   for(int i = myStart; c<split; i++){
	t[c] = arr[i];
	c++;
   }
   printf("Rank: %d Array: \n",rank);
   printArray(t, split);
   printf("\n");
   if(rank == 0){ 
   	printf("Given array is \n"); 
   	printArray(arr, n); 
  }
   if(rank == 0){
   	mergeSort(t, myStart, myEnd -1);
   }
   printf("Rank: %d, After mergesort \n",rank); 
   MPI_Gather(t, split, MPI_INT, arr, split, MPI_INT, 0, MPI_COMM_WORLD);
 
   if(rank == 0){
	printf("Array after gather\n");
	printArray(arr, n);
	int m = n / 2;
	printf("Before final merge\n");
	merge(arr, 0, m, n);
   	printf("\nSorted array is \n"); 
   	printArray(arr, n);
   }
   double endTime = MPI_Wtime();
   MPI_Finalize();

   printf("Time: .5%f\n", endTime -startTime);
   return 0; 
} 
