#include<stdio.h>
#include<stdlib.h>
#include"mpi.h"

int get_max (int a[], int n){
   int max = a[0];
   for (int i = 1; i < n; i++)
      if (a[i] > max)
         max = a[i];
   return max;
}

void radix_sort (int a[], int n){
   int bucket[10][10], bucket_cnt[10];
   int i, j, k, r, NOP = 0, divisor = 1, lar, pass;
   lar = get_max (a, n);
   while (lar > 0){
      NOP++;
      lar /= 10;
   }
   for (pass = 0; pass < NOP; pass++){
      for (i = 0; i < 10; i++){
         bucket_cnt[i] = 0;
      }
      for (i = 0; i < n; i++){
         r = (a[i] / divisor) % 10;
         bucket[r][bucket_cnt[r]] = a[i];
         bucket_cnt[r] += 1;
      }
      i = 0;
      for (k = 0; k < 10; k++){
         for (j = 0; j < bucket_cnt[k]; j++){
            a[i] = bucket[k][j];
            i++;
         }
      }
      divisor *= 10;
      printf ("After pass %d : ", pass + 1);
      for (i = 0; i < n; i++)
         printf ("%d ", a[i]);
      printf ("\n");
   }
}

int main (int argc, char** argv){
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

  //Splitting up the work.
  int split = n / numranks;
  int myStart = rank * split;
  int myEnd = (rank+1) * split -1 ;

  //Creating temp array
  int* t =(int*)malloc(split*sizeof(int));

  if(rank == numranks -1){
  	myEnd = n;
  }
  //if(rank == 0){
  //      printf("Starting array: \n");
  //      printArray(arr, n);
  //}
  int c = 0;
  for(int i = myStart; i <= myEnd; i++){
  	t[c] = arr[i];
        c++;
  }

  radix_sort (t, split);

  MPI_Gather(t, split, MPI_INT, arr, split, MPI_INT, 0, MPI_COMM_WORLD);
  if(rank == 0){
	radix_sort(arr, n);
  	for (int i = 0; i < n; i++)
     		printf ("%d ", arr[i]);
  		printf ("\n");
  }
  MPI_Finalize();
  return 0;
}
