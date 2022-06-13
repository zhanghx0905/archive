#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <memory>
#include <mpi.h>
using namespace std;

void gen(int n, unique_ptr<double[]>& matrix, unique_ptr<double[]>& vector) {
    matrix = unique_ptr<double[]>(new double[n * n]);
    srand((unsigned)time(0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i * n + j] = -1 + rand() * 1.0 / RAND_MAX * 2;
        }
    }
    vector = unique_ptr<double[]>(new double[n]);
    for (int i = 0; i < n; i++) {
        vector[i] = -1 + rand() * 1.0 / RAND_MAX * 2;
    }
}

unique_ptr<double[]> serial(int n, unique_ptr<double[]>& matrix, unique_ptr<double[]>& vector) {
    unique_ptr<double[]>result(new double[n]);
    for (int i = 0; i < n; i++) {
        double temp = 0.0;
        for (int j = 0; j < n; j++) {
            temp += matrix[i * n + j] * vector[j];
        }
        result[i] = temp;
    }
    return move(result);
}

void run(int n) {
    int pid, psize;
    double beg[2], end, ser_duration;
    MPI_Init(nullptr, nullptr);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &pid);
    MPI_Comm_size(comm, &psize);

    if (n % psize != 0) {
        printf("n must be divisible by the number of thread.\n");
        exit(-1);
    }

    int loc_col = n / psize;

    MPI_Datatype vec_t, col_t;
    MPI_Type_vector(n, loc_col, n, MPI_DOUBLE, &vec_t);
    MPI_Type_create_resized(vec_t, 0, loc_col * sizeof(double), &col_t);
    MPI_Type_commit(&col_t);

    unique_ptr<double[]> matrix, vector, ser_res;
    unique_ptr<double[]> loc_mat(new double[n * loc_col]),
        loc_vec(new double[loc_col]);

    if (pid == 0) {
        gen(n, matrix, vector);
        double ser_beg = MPI_Wtime();
        ser_res = serial(n, matrix, vector);
        ser_duration = MPI_Wtime() - ser_beg;
        printf("The serial program finished in %.4f s\n", ser_duration);
    }
    unique_ptr<int[]> blocksize(new int[psize]), stride(new int[psize]);
    for (int i = 0; i < psize; i++) {
        blocksize[i] = 1;
        stride[i] = i;
    }
    MPI_Barrier(comm);
    beg[0] = MPI_Wtime();

    MPI_Scatter(vector.get(), loc_col, MPI_DOUBLE,
        loc_vec.get(), loc_col, MPI_DOUBLE, 0, comm);
    MPI_Scatterv(matrix.get(), blocksize.get(), stride.get(), col_t,
        loc_mat.get(), n * loc_col, MPI_DOUBLE, 0, comm);

    MPI_Barrier(comm);
    beg[1] = MPI_Wtime();

    unique_ptr<double[]> loc_res(new double[n]);
    for (int i = 0; i < n; i++) {
        double temp = 0;
        for (int j = 0; j < loc_col; j++)
            temp += loc_mat[i * loc_col + j] * loc_vec[j];
        loc_res[i] = temp;
    }

    if (pid == 0) {
        unique_ptr<double[]> res(new double[n]);
        MPI_Reduce(loc_res.get(), res.get(), n, MPI_DOUBLE, MPI_SUM, 0, comm);
        end = MPI_Wtime();
        beg[0] = end - beg[0], beg[1] = end - beg[1];
        double duration[2], norm_diff = 0;
        MPI_Reduce(beg, duration, 2, MPI_DOUBLE, MPI_MAX, 0, comm);

        for (int i = 0; i < n; i++) {
            norm_diff += powl(ser_res[i] - res[i], 2.);
        }
        norm_diff = sqrtl(norm_diff);
        printf("the 2-norm of error is %.10lf\n", norm_diff);
        printf("parallel finished in (include data distribution) %.4lf s, speed up rate: %.2f\n",
            duration[0], ser_duration / duration[0]);
        printf("parallel finished in (enclude data distribution) %.4lf s, speed up rate: %.2f\n", duration[1], ser_duration / duration[1]);
    }
    else {
        MPI_Reduce(loc_res.get(), nullptr, n, MPI_DOUBLE, MPI_SUM, 0, comm);
        end = MPI_Wtime();
        beg[0] = end - beg[0], beg[1] = end - beg[1];
        MPI_Reduce(beg, nullptr, 2, MPI_DOUBLE, MPI_MAX, 0, comm);
    }
    MPI_Type_free(&col_t);
    MPI_Finalize();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("usage: %s n\n", argv[0]);
        return -1;
    }
    int n = atoi(argv[1]);
    run(n);
    return 0;
}