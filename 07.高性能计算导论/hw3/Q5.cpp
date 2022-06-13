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
    //vector = unique_ptr<double[]>(new double[n]);
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
    MPI_Comm comm_world = MPI_COMM_WORLD;
    MPI_Comm_size(comm_world, &psize);
    MPI_Comm_rank(comm_world, &pid);
    

    int psize_sr = sqrt(psize);
    if (psize_sr * psize_sr != psize) {
        printf("the number of threads must be the square of a number.\n");
        exit(-1);
    }
    if (n  % psize_sr != 0) {
        printf("n must be divisible by the sqrare root of the number of thread.\n");
        exit(-1);
    }
    int loc_n = n / psize_sr;
    unique_ptr<double[]> matrix, ser_res,
        vector(new double[n]); //vector is used for each thread
    
    if (pid == 0) {
        gen(n, matrix, vector);
        double ser_beg = MPI_Wtime();
        ser_res = serial(n, matrix, vector);
        ser_duration = MPI_Wtime() - ser_beg;
        printf("The serial program finished in %.4f s\n", ser_duration);
    }
    MPI_Datatype vec_t, block_t;
    MPI_Type_vector(loc_n, loc_n, n, MPI_DOUBLE, &vec_t);
    MPI_Type_create_resized(vec_t, 0, loc_n * sizeof(double), &block_t);
    MPI_Type_commit(&block_t);

    unique_ptr<int[]> blocksize(new int[psize]), stride(new int[psize]);
    for (int i = 0; i < psize_sr; i++) {
        for (int j = 0; j < psize_sr; j++) {
            blocksize[i * psize_sr + j] = 1;
            // (i * n + j) * loc_n * sizeof(double)
            stride[i * psize_sr + j] = i * n + j;
        }
    }
    
    unique_ptr<double[]> loc_mat(new double[loc_n * loc_n]);
    MPI_Barrier(comm_world);
    beg[0] = MPI_Wtime();

    MPI_Scatterv(matrix.get(), blocksize.get(), stride.get(), block_t,
        loc_mat.get(), loc_n * loc_n, MPI_DOUBLE, 0, comm_world);
    MPI_Bcast(vector.get(), n, MPI_DOUBLE, 0, comm_world);

    MPI_Barrier(comm_world);
    beg[1] = MPI_Wtime();

    unique_ptr<double[]> loc_res(new double[n] {});
    int vec_off = (pid % psize_sr) * loc_n, res_off = (pid / psize_sr) * loc_n;
    for (int i = 0; i < loc_n; i++) {
        double tmp = 0;
        for (int j = 0; j < loc_n; j++) {
            tmp += loc_mat[i * loc_n + j] * vector[vec_off + j];
        }
        loc_res[res_off + i] = tmp;
    }

    if (pid == 0) {
        unique_ptr<double[]> res(new double[n]);
        MPI_Reduce(loc_res.get(), res.get(), n, MPI_DOUBLE, MPI_SUM, 0, comm_world);
        end = MPI_Wtime();
        beg[0] = end - beg[0], beg[1] = end - beg[1];
        double duration[2], norm_diff=0;
        MPI_Reduce(beg, duration, 2, MPI_DOUBLE, MPI_MAX, 0, comm_world);

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
        MPI_Reduce(loc_res.get(), nullptr, n, MPI_DOUBLE, MPI_SUM, 0, comm_world);
        end = MPI_Wtime();
        beg[0] = end - beg[0], beg[1] = end - beg[1];
        MPI_Reduce(beg, nullptr, 2, MPI_DOUBLE, MPI_MAX, 0, comm_world);
    }
    MPI_Type_free(&block_t);
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