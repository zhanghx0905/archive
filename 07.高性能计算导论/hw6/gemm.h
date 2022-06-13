// studentid: 2018011365
#include "util.h"

const size_t TS = 16;
const size_t VECTOR_SIZE = 4;
typedef unsigned long long ull;

template <class T> 
__global__ void gemm_tilling(
    const T* __restrict__ A,
    const T* __restrict__ B,
    T* C,
    T alpha, T beta,
    int M, int N, int K) {

    const size_t row = TS * blockIdx.y + threadIdx.y,
        col = TS * blockIdx.x + threadIdx.x;
    __shared__ T Asub[TS][TS], Bsub[TS][TS];

    T c = 0.;
    int k0 = 0, tiles = (K + TS - 1) / TS;

    for (int i = 0; i < tiles; i++, k0 += TS) {
        if (row < M && (k0 + threadIdx.x < K))
            Asub[threadIdx.y][threadIdx.x] = A[row * K + (k0 + threadIdx.x)];
        else
            Asub[threadIdx.y][threadIdx.x] = 0.;

        if (col < N && (k0 + threadIdx.y < K))
            Bsub[threadIdx.y][threadIdx.x] = B[(k0 + threadIdx.y) * N + col];
        else
            Bsub[threadIdx.y][threadIdx.x] = 0.;

        __syncthreads();
        for (int k = 0; k < TS; k++)
            c += Asub[threadIdx.y][k] * Bsub[k][threadIdx.x];
        __syncthreads();
    }

    if (row < M && col < N) {
        C[row * N + col] = alpha * c + beta * C[row * N + col];
    }
}


template<class T>
__global__ void gemm_opt(
    T* A,T* B, T* C,
    T alpha, T beta,
    int M, int N, int K)
{
    const size_t bx = blockIdx.x,
        by = blockIdx.y,
        tx = threadIdx.x,
        ty = threadIdx.y;

    __shared__ T Asub[TS * TS];
    T Csub[TS] = { 0. };

    const size_t aBegin = K * TS * by,
        aEnd = aBegin + K - 1,
        aStep = TS,
        bBegin = TS * VECTOR_SIZE * bx,
        bStep = TS * N;

    const size_t vec_col = TS * ty + tx;

    for (int a = aBegin, b = bBegin, phase = 0; a <= aEnd; a += aStep, b += bStep, phase += TS) {
#pragma unroll
        for (int i = 0; i < TS; i += VECTOR_SIZE) {  // read in Asub in column major
            if (phase + tx < K && TS * by + (i + ty) < M)
                Asub[(i + ty) + TS * tx] = A[a + K * (i + ty) + tx];
            else
                Asub[(i + ty) + TS * tx] = 0.;
        }
        __syncthreads();

        if (bBegin + vec_col < N) {
            T* ap = Asub, * bp = &B[b + vec_col];
#pragma unroll
            for (int i = 0; i < TS && phase + i < K; ++i) {
                T bv = *bp;  // bv = Bsub[i][TS*ty+tx]
                Csub[0] += ap[0] * bv;
                Csub[1] += ap[1] * bv;  // ap[i] = Asub[0][i]
                Csub[2] += ap[2] * bv;
                Csub[3] += ap[3] * bv;
                Csub[4] += ap[4] * bv;
                Csub[5] += ap[5] * bv;
                Csub[6] += ap[6] * bv;
                Csub[7] += ap[7] * bv;
                Csub[8] += ap[8] * bv;
                Csub[9] += ap[9] * bv;
                Csub[10] += ap[10] * bv;
                Csub[11] += ap[11] * bv;
                Csub[12] += ap[12] * bv;
                Csub[13] += ap[13] * bv;
                Csub[14] += ap[14] * bv;
                Csub[15] += ap[15] * bv;
                ap += TS;
                bp += N;
            }
        }
        __syncthreads();
    }

    if (bBegin + vec_col < N) {
        int row = TS * by, col = bBegin + vec_col;
        int idx = N * row + col;
        for (int i = 0; i < TS && row < M; ++i) {
            C[idx] = alpha * Csub[i] + beta * C[idx];
            row++;
            idx += N;
        }
    }
}


template<class T>
double myGEMM(T* A, T* B, T* C, T alpha, T beta)
{
	//printf("perform your gemm here on m=%d n=%d k=%d\n", M, N, K);
	bool preprocess = false;
	if(preprocess)
	{
		// your preprocess
		timestamp(t0);
		// your gemm
;
		checkCudaErrors(cudaDeviceSynchronize());
		timestamp(t1);

		return getDuration(t0, t1);
	}
    else
    {
        if ((ull)M * (ull)N * (ull)K < pow(300, 3)) {
            dim3 dimBlock(TS, TS);
            dim3 dimGrid((N + TS - 1) / TS, (M + TS - 1) / TS);
            gemm_tilling << <dimGrid, dimBlock >> > (A, B, C, alpha, beta, M, N, K);
        }
        else {
            dim3 dimBlock(TS, VECTOR_SIZE);
            dim3 dimGrid((N + TS * VECTOR_SIZE - 1) / (TS * VECTOR_SIZE), (M + TS - 1) / TS);
            gemm_opt << <dimGrid, dimBlock >> > (A, B, C, alpha, beta, M, N, K);
        }
        checkCudaErrors(cudaDeviceSynchronize());
        return 0.f;
    }
	
}
