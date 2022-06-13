#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void Free_array(int* a) {
    if (a) delete[] a;
}

int main() {
    int comm_sz, my_rank;
    int local_n = 0;
    int global_n, * offsets = NULL, * local_n_arr = NULL;
    int* global_x = NULL, * global_y = NULL;
    int* local_x = NULL, * local_y = NULL;
    MPI_Comm comm;

    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    if (my_rank == 0) {
        printf("What's the order of the vectors?\n");
        scanf("%d", &global_n);

        global_x = new int[global_n];
        global_y = new int[global_n];
        printf("Enter the vector X:\n");
        for (int i = 0; i < global_n; i++)
            scanf("%d", &global_x[i]);
        printf("Enter the vector Y:\n");
        for (int i = 0; i < global_n; i++)
            scanf("%d", &global_y[i]);

        offsets = new int[comm_sz];
        local_n_arr = new int[comm_sz];

        int remainder = global_n % comm_sz,
            quotient = global_n / comm_sz;
        offsets[0] = 0;
        for (int rank = 0; rank < comm_sz; rank++) {
            if (rank < remainder)
                local_n_arr[rank] = quotient + 1;
            else 
                local_n_arr[rank] = quotient;
            if (rank > 0)
                offsets[rank] = offsets[rank - 1] + local_n_arr[rank - 1];
        }
    }
    MPI_Barrier(comm);

    MPI_Scatter(local_n_arr, 1, MPI_INT, &local_n, 1, MPI_INT, 0, comm);

    local_x = new int[local_n];
    local_y = new int[local_n];
    MPI_Scatterv(global_x, local_n_arr, offsets, MPI_INT, local_x, local_n, MPI_INT, 0, comm);
    MPI_Scatterv(global_y, local_n_arr, offsets, MPI_INT, local_y, local_n, MPI_INT, 0, comm);
    for (int i = 0; i < local_n; i++)
        local_x[i] += local_y[i];
    MPI_Gatherv(local_x, local_n, MPI_INT, global_x, local_n_arr, offsets, MPI_INT, 0, comm);

    if (!my_rank) {
        for (int i = 0; i < global_n; i++)
            printf("%d ", global_x[i]);
        printf("\n");
    }

    MPI_Finalize();

    Free_array(offsets);
    Free_array(global_x);
    Free_array(global_y);
    Free_array(local_n_arr);
    Free_array(local_x);
    Free_array(local_y);
    return 0;
}