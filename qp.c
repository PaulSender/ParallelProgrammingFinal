#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
void swap(int* a, int* b) 
{ 
    int t = *a; 
    *a = *b; 
    *b = t; 
} 

int partition (int arr[], int low, int high) 
{ 
    int pivot = arr[high];    // pivot 
    int i = (low - 1);  // Index of smaller element 
  
    for (int j = low; j <= high- 1; j++) 
    { 
        // If current element is smaller than the pivot 
        if (arr[j] < pivot) 
        { 
            i++;    // increment index of smaller element 
            swap(&arr[i], &arr[j]); 
        } 
    } 
    swap(&arr[i + 1], &arr[high]); 
    return (i + 1); 
} 

void quickSort(int arr[], int low, int high) {
	if (low < high) {
		int pi = partition(arr, low, high); 
  
        // Separately sort elements before 
        // partition and after partition 
        quickSort(arr, low, pi - 1); 
        quickSort(arr, pi + 1, high);
	}
}



int main(int argc, char **argv) {
    int numranks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status stat;
    int n = 1000;
    int myN = n / numranks;
    int *globaldata;
    int *localdata = (int *)malloc(myN * sizeof(int));
    int *finaldata;
    int pivot;
    int i;

    if (rank == 0) {
		globaldata = (int *)malloc(n * sizeof(int));
		finaldata = (int *)malloc(n *sizeof(int));
		for (int i = 0; i < n; i++) {
			globaldata[i] = rand() % n;
		}
		pivot = globaldata[rand() % n];
    }
	MPI_Bcast(&pivot, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	
    MPI_Scatter(globaldata, myN, MPI_INT, localdata, myN, MPI_INT, 0, MPI_COMM_WORLD);
	printf("rank %d unsorted array is: ", rank);
    int *lower = (int *)malloc((myN/2) * sizeof(int));
    int *higher = (int *)malloc((myN/2) * sizeof(int));
	for (int i = 0; i < myN; i++) {
		if(localdata[i] < pivot) {
            lower[i] = localdata[i];
        }
        else {
            higher[i] = localdata[i];
        }
    //Higher rank is receving the higher data from the lower rank and sending it's low list to lower rank
    if(rank == 1) {
        MPI_Recv(higher, myN/2, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
        MPI_Send(lower, myN/2, MPI_INT, 0, 0, MPI_COMM_WORLD);
        printf("Rank %d: ", rank);
        for(int i = 0; i < myN/2; i++) {
            printf("%d ", higher[i]);
        }
        printf("\n");
    }
    //Lower rank is receiving the lower data from the higher rank and sending the higher list to the higher rank
    else {
        MPI_Send(higher, myN/2, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(lower, myN/2, MPI_INT, 1, 0, MPI_COMM_WORLD, &stat);
          printf("Rank %d: ", rank);
        for(int i = 0; i < myN/2; i++) {
            printf("%d ", lower[i]);
        }
        printf("\n");
    }
	printf("\n");

	//quickSort(localdata, 0, myN - 1);
	//printf("rank %d sorted array is: ", rank);
	// for (int i = 0; i < myN; i++) {
	// 	printf("%d ", localdata[i]);
	// }
    
	// printf("\n");

    MPI_Gather(localdata, myN, MPI_INT, finaldata, myN, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
		//quickSort(finaldata, 0, n-1);
		printf("Final Result: ");
     for(int i = 0; i < 4; i++) {
		 printf("%d ", finaldata[i]);
	 }
    }


    MPI_Finalize();
    return 0;
}

