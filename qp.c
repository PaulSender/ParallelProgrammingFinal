#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

double startTime;


//static inline /* this improves performance; Exercise: by how much? */
// swap helper function 
void swap(int * sub, int a, int b)
{
    int t = sub[a];
    sub[a] = sub[b];
    sub[b] = t;
}

// Main quicksort method adapted to use each rank's sub array and respective high and low points
/*
    1. Recursive function that picks a pivot somewhere in the middle of the sub array
    2. Swap the pivot and first element (some algorithms don't do this but it worked with this one)
    3. Partition the sub array starting at it's respective "low" element
        A. If the current element is less than the pivot element
        a. increment partitioning index to keep track of where to swap 
        b. swap the current index and the partitioning index
        c. when loop breaks, swap the low and partitioning index to insure the pivot is back in the correct place
    4. Recurse quicksort on lower half of the array
    5. Recurse on array after step 4 to sort the higher half of the sub array


*/


void quicksort(int * sub, int low, int high)
{
    
    // base case?
    if (high <= 1)
        return;
    // find pivot and swap with first element (pivot has to be in the center of the sub array)
    int x = low+high/2;
    int pivot = sub[x];
    swap(sub, low, x);
    // partition sub array starting at it's respective low element
    // pi = partitioning index
    int pi = low;
    for (int i = low+1; i < low+high; i++)
        if (sub[i] < pivot) {
            pi++;
            swap(sub, i, pi);
            
        }
    // swap pivot correctly
    swap(sub, low, pi);

    // recurse below partition
    quicksort(sub, low, pi-low);
    // recure above partition
    quicksort(sub, pi+1, low+high-pi-1);
}


/* merge two sorted arrays v1, v2 of lengths n1, n2, respectively */
int * merge(int * v1, int n1, int * v2, int n2)
{
    int * result = (int *)malloc((n1 + n2) * sizeof(int));
    int i = 0;
    int j = 0;
    int k;
    for (k = 0; k < n1 + n2; k++) {
        if (i >= n1) {
            result[k] = v2[j];
            j++;
        }
        else if (j >= n2) {
            result[k] = v1[i];
            i++;
        }
        else if (v1[i] < v2[j]) { // indices in bounds as i < n1 && j < n2
            result[k] = v1[i];
            i++;
        }
        else { // v2[j] <= v1[i]
            result[k] = v2[j];
            j++;
        }
    }
    return result;
}

int main(int argc, char ** argv)
{
    int n;
    int * data = NULL;
    int c, s;
    int * chunk;
    int o;
    int * other;
    int step;
    int p, rank;
    MPI_Status status;
    double elapsed_time;
    int i;

    if (argc < 2) {
        printf("Please give the number of elements you would like to sort and try again.\n");
        MPI_Finalize();
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        // read size of data
        // file = fopen(argv[1], "r");
        // fscanf(file, "%d", &n);
        // compute chunk size
        n = atoi(argv[1]);
        c = (n%p!=0) ? n/p+1 : n/p;
        // read data from file
        data = (int *)malloc(p*c * sizeof(int));
        for(int i = 0; i < n; i++) {
            data[i] = rand() % n;
            //printf("%d ", data[i]);
        }
        // pad data with 0 -- doesn't matter
        // for (i = n; i < p*c; i++)
        //   data[i] = 0;
    }

    // start the timer
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();

    // broadcast size
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // compute chunk size
    c = (n%p!=0) ? n/p+1 : n/p;

    // scatter data
    chunk = (int *)malloc(c * sizeof(int));
    MPI_Scatter(data, c, MPI_INT, chunk, c, MPI_INT, 0, MPI_COMM_WORLD);
    free(data);
    data = NULL;

    // compute size of own chunk and sort it
    s = (n >= c * (rank+1)) ? c : n - c * rank;
    quicksort(chunk, 0, s);

    // up to log_2 p merge steps
    for (step = 1; step < p; step = 2*step) {
        if (rank % (2*step) != 0) {
            // rank is no multiple of 2*step: send chunk to rank-step and exit loop
            MPI_Send(chunk, s, MPI_INT, rank-step, 0, MPI_COMM_WORLD);
            break;
        }
        // rank is multiple of 2*step: merge in chunk from rank+step (if it exists)
        if (rank+step < p) {
            // compute size of chunk to be received
            o = (n >= c * (rank+2*step)) ? c * step : n - c * (rank+step);
            // receive other chunk
            other = (int *)malloc(o * sizeof(int));
            MPI_Recv(other, o, MPI_INT, rank+step, 0, MPI_COMM_WORLD, &status);
            // merge and free memory
            data = merge(chunk, s, other, o);
            free(chunk);
            free(other);
            chunk = data;
            s = s + o;
        }
    }

    // stop the timer
    elapsed_time += MPI_Wtime();

    // write sorted data to out file and print out timer
    if (rank == 0) {
        for (i = 0; i < s; i++)
            printf("%d ", chunk[i]);
        printf("\n");
        printf("\n");
        printf("\n");
        //   fprintf(file, "%d\n", chunk[i]);
        // fclose(file);
        printf("Quicksort %d ints on %d procs: %f secs\n", n, p, elapsed_time);
    }

    MPI_Finalize();
    return 0;
}