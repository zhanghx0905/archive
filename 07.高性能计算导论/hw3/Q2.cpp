#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
const int N = 10;

int main() {
    int comm_sz, my_rank;
    MPI_Comm comm;
    int localData[N];

    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);
    // initialize local array
    srand(my_rank);
    for (int i = 0; i < N; i++) {
        localData[i] = rand() % 10;
    }
    // calculate the prefix sum of localData in each thread
    for (int i = 1; i < N; i++)
        localData[i] += localData[i - 1];
    // prefix = a_i0 + ... + a_(p-1)N
    int prefix;
    MPI_Scan(&localData[N - 1], &prefix, 1, MPI_INT, MPI_SUM, comm);
    prefix -= localData[N - 1];
    for (int i = 0; i < N; i++)
        localData[i] += prefix;

;    if (!my_rank) { //my_rank == 0
        int* globalData=new int[N * comm_sz];
        MPI_Gather(localData, N, MPI_INT, globalData, N, MPI_INT, 0, comm);
        for (int i = 0; i < N * comm_sz; i++)
            printf("%d ", globalData[i]);
        printf("\n");
        delete[] globalData;
    }
    else {
        MPI_Gather(localData, N, MPI_INT, NULL, N, MPI_INT, 0, comm);
    }
    MPI_Finalize();
    return 0;
}  /* main */
