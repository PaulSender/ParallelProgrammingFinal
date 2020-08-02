// C program for implementation of Bubble sort 
#include <stdio.h> 
#include <stdlib.h>
  
void swap(int *xp, int *yp) 
{ 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 
  
// A function to implement bubble sort 
void bubbleSort(int arr[], int n) 
{ 
   int i, j; 
   for (i = 0; i < n-1; i++)       
  
       // Last i elements are already in place    
       for (j = 0; j < n-i-1; j++)  
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


  
// Driver program to test above functions 
int main(int argc, char **argv) 
{ 
    if(argc < 2){
        printf("Please give the number of elements you would like to sort and try again.\n");
        return 0;
    }
    else{
        int n = atoi(argv[1]);
        int *arr =(int *)malloc(n*sizeof(int));
        for(int i = 0; i < n; i++){
            int random_number = rand() % 100 + 1;
            arr[i] = random_number;
        }
        printf("Starting array: \n");
        printArray(arr, n); 
        bubbleSort(arr, n); 
        printf("Sorted array: \n"); 
        printArray(arr, n); 
        return 0; 
    }
} 