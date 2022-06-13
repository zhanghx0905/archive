# Homework 3

张鹤潇 2018011365

```shell
make
# 以5个进程启动Q1
srun -n 5 Q1
# 以5个进程启动Q5，设定矩阵规模为3600
srun -n 9 Q5 3600
```

## Q1.Ex 3.13

### 解题思路

用`Scatterv`向子进程分发子向量数据，在每个子进程内进行向量加法计算，最后用`Gatherv`把计算得到的结果取回主进程以统一输出。

### 代码实现

为了使子进程分到的向量长度尽可能均匀，这里使用了第一次作业`Q2`提到的算法。

```c++
int remainder = global_n % comm_sz, quotient = global_n / comm_sz;
offsets[0] = 0;
for (int rank = 0; rank < comm_sz; rank++) {
    if (rank < remainder)
        local_n_arr[rank] = quotient + 1;
    else 
        local_n_arr[rank] = quotient;
    if (rank > 0)
        offsets[rank] = offsets[rank - 1] + local_n_arr[rank - 1];
}
```

数据分发部分：

```c++
// 将子向量长度分发给各个子进程
MPI_Scatter(local_n_arr, 1, MPI_INT, &local_n, 1, MPI_INT, 0, comm);

local_x = new int[local_n], local_y = new int[local_n];
MPI_Scatterv(global_x, local_n_arr, offsets, MPI_INT, local_x, local_n, MPI_INT, 0, comm);
MPI_Scatterv(global_y, local_n_arr, offsets, MPI_INT, local_y, local_n, MPI_INT, 0, comm);
```

计算和数据收集部分：

```c++
for (int i = 0; i < local_n; i++)
    local_x[i] += local_y[i];
MPI_Gatherv(local_x, local_n, MPI_INT, global_x, local_n_arr, offsets, MPI_INT, 0, comm);
```

### 测试结果

正确性测试如下：

- 进程数为10，子向量长度为7。

```shell
# input
What's the order of the vectors?
7
Enter the vector X:
1 2 3 4 5 6 7
Enter the vector Y:
7 6 5 4 3 2 -1
# output
8 8 8 8 8 8 6 
```

- 进程数为5，子向量长度为7.

```shell
# input
What's the order of the vectors?
7               
Enter the vector X:
1 2 3 4 5 6 7
Enter the vector Y:
7 6 5 4 3 2 7
# output
8 8 8 8 8 8 14 
```

## Q2.Ex 3.11(d)

### 解题思路

先求每个子进程向量的前缀和，再在每个子向量的最后一个位置调用`MPI_Scan`求之前所有进程子向量元素的前缀和，将之加到本进程子向量上，就完成了前缀和计算过程。最后调用`MPI_Gather`由主进程统一输出即可。形式化过程如下：
$$
\begin{bmatrix}
a_{11} & \cdots & a_{1N}\\
& \ddots &\\
a_{p1} & \cdots & a_{pN}
\end{bmatrix}
\Rightarrow
\begin{bmatrix}
a_{11} & \cdots & a_{11}+\cdots +a_{1N}\\
& \ddots &\\
a_{p1} & \cdots & a_{p1}+\cdots+a_{pN}
\end{bmatrix}(1)\\

\stackrel{MPI\_Scan((1)[,\ N])}\Rightarrow
\begin{bmatrix}
a_{11}+\cdots+a_{1N}\\
a_{11}+\cdots+a_{2N}\\
\vdots\\
a_{11}+\cdots+a_{pN}
\end{bmatrix}
\stackrel{-(1)[,\ N]}\Rightarrow
\begin{bmatrix}\\

a_{11}+\cdots+a_{1N}\\
\vdots\\
a_{11}+\cdots+a_{p-1N}
\end{bmatrix}(2)\\

(1)+(2)\Rightarrow local\_res\stackrel{MPI\_Gather}\Rightarrow...
$$

### 代码实现

```c++
// calculate the prefix sum of localData in each thread
for (int i = 1; i < N; i++)
    localData[i] += localData[i - 1];
// prefix = a_i0 + ... + a_(p-1)N
int prefix;
MPI_Scan(&localData[N - 1], &prefix, 1, MPI_INT, MPI_SUM, comm);
prefix -= localData[N - 1];
for (int i = 0; i < N; i++)
    localData[i] += prefix;
```

### 测试结果

正确性测试如下, 进程数为5。

```shell
3 9 16 21 24 29 35 37 46 47 50 56 63 68 71 76 82 84 93 94 94 103 111 116 117 125 129 136 141 148 154 159 167 167 172 172 174 180 181 185 186 189 193 199 202 209 214 217 225 230
```

## Q3.PA 3.1

### 解题思路

本题流程比较简单，在每个进程内计算出桶中元素的个数后，调用`MPI_Reduce`取回主进程打印输出即可。考虑到输入数据的确定性，简单起见，`Which_bin`函数以线性查找的方式实现。

### 代码实现

```c
// FInd_bins  
for (int i = 0; i < local_data_count; i++) {
    int bin = Which_bin(local_data[i], bin_maxes, bin_count, min_meas);
    loc_bin_cts[bin]++;
}
MPI_Reduce(loc_bin_cts, bin_counts, bin_count, MPI_FLOAT, MPI_SUM, 0, comm);

// Which_bin, linear search
for (int i = 0; i < bin_count; i++) {
    if (bin_maxes[i] >= data)
        return i;
}
return bin_count - 1; // for cases data > max_areas 
```

### 测试结果

正确性测试如下，输入`10 0 100 200`.

```shell
0.000-10.000:	XXXXXXXXXXXXXXXXXX
10.000-20.000:	XXXXXXXXXXXXXXXXXXXXXX
20.000-30.000:	XXXXXXXXXXXXXXXXXXXX
30.000-40.000:	XXXXXXXXXXXXXXXX
40.000-50.000:	XXXXXXXXXXXXXXX
50.000-60.000:	XXXXXXXXXXXXXXXXXXXX
60.000-70.000:	XXXXXXXXXXXXXXXXXXX
70.000-80.000:	XXXXXXXXXXXXXXXXXXXXXX
80.000-90.000:	XXXXXXXXXXXXXXXXXXXXX
90.000-100.000:	XXXXXXXXXXXXXXXXXXXXXXXXXXX
```

多次测试，所得结果与进程数无关。

## Q4.PA3.5

### 解题思路

本题的关键在于如何分发子矩阵。为了减少信息发送的次数，提高效率，我参考了`prog3.5_mpi_mat_vect_col.c`，用`MPI_Type_vector`和`MPI_Type_create_resized`定义派生数据类型`col_t`，它由$n$个长度为$\frac{n}{comm\_sz}$，偏移为$n$的double块组成，用这样的数据块按列分割矩阵(row-major)，用将之发送到子进程中。

接下来再将向量均分到每个进程中，在每个进程上运行矩阵乘法，求得部分和。最后用`MPI_Reduce`汇总到主进程中。

### 代码实现

派生类型定义：

```c++
 MPI_Datatype vec_t, col_t;
 MPI_Type_vector(n, loc_col, n, MPI_DOUBLE, &vec_t);
 MPI_Type_create_resized(vec_t, 0, loc_col * sizeof(double), &col_t);
 MPI_Type_commit(&col_t);
```

数据分发：

```c++
 for (int i = 0; i < psize; i++) {
    blocksize[i] = 1;
  	// i号块相对于开头地址的偏移量是i * loc_col * sizeof(double)
    stride[i] = i;
 }
//用unique_ptr模板类简化内存管理，调用get方法返回指针
 MPI_Scatterv(matrix.get(), blocksize.get(), stride.get(), col_t,  loc_mat.get(),  n * loc_col, MPI_DOUBLE, 0, comm);
 MPI_Scatter(vector.get(), loc_col, MPI_DOUBLE, loc_vec.get(), loc_col,  MPI_DOUBLE, 0, comm);
```

数据收集：

```c++
  if (pid == 0) {
      unique_ptr<double[]> res(new double[n]);
      MPI_Reduce(loc_res.get(), res.get(), n, MPI_DOUBLE, MPI_SUM, 0, comm);
      // ...
      // 统计时间
      MPI_Reduce(beg, duration, 2, MPI_DOUBLE, MPI_MAX, 0, comm);
  }
  else {
      MPI_Reduce(loc_res.get(), res.get(), n, MPI_DOUBLE, MPI_SUM, 0, comm);
      // ...
      MPI_Reduce(beg, duration, 2, MPI_DOUBLE, MPI_MAX, 0, comm);
  }
```

### 测试结果

串行结果与并行结果之差的范数基本为零，可见结果的正确性。

考虑数据分发, 运行时间/加速比：

| p    | $n=3600$      | $n=7200$      | $n=14400$     | $n=28800$     |
| ---- | ------------- | ------------- | ------------- | ------------- |
| 1    | 0.0405 s/0.21 | 0.1576 s/0.21 | 0.6416 s/0.22 | 2.6114 s/0.25 |
| 2    | 0.0506 s/0.17 | 0.1983 s/0.17 | 0.7871 s/0.18 | 3.1441 s/0.21 |
| 3    | 0.0534 s/0.16 | 0.2099 s/0.16 | 0.8266 s/0.18 | 3.2857 s/0.20 |
| 4    | 0.0503 s/0.17 | 0.2164 s/0.16 | 0.8477 s/0.16 | 3.4007 s/0.20 |
| 8    | 0.0551 s/0.16 | 0.2256 s/0.15 | 0.8779 s/0.16 | -             |
| 12   | 0.0602 s/0.15 | 0.2352 s/0.15 | 0.9135 s/0.15 | -             |
| 16   | 0.0626 s/0.14 | 0.2416 s/0.14 | 0.9463 s/0.14 | -             |
| 20   | 0.0628 s/0.14 | 0.2410 s/0.14 | 0.9539 s/0.14 | -             |
| 30   | 0.5239 s/0.02 | 2.0478 s/0.02 | 8.1883 s/0.02 | -             |

不考虑数据分发：

| p    | $n=3600$      | $n=7200$      | $n=14400$     | $n=28800$     |
| ---- | ------------- | ------------- | ------------- | ------------- |
| 1    | 0.0086 s/1.00 | 0.0335 s/1.00 | 0.1358 s/1.02 | 0.6401 s/1.00 |
| 2    | 0.0046 s/1.83 | 0.0175 s/1.91 | 0.0683 s/2.07 | 0.2924 s/2.25 |
| 3    | 0.0032 s/2.73 | 0.0128 s/2.65 | 0.0480 s/3.12 | 0.1914 s/3.50 |
| 4    | 0.0025 s/3.42 | 0.0094 s/3.55 | 0.0381 s/3.58 | 0.1501 s/4.57 |
| 8    | 0.0025 s/3.53 | 0.0076 s/4.50 | 0.0281 s/4.88 | -             |
| 12   | 0.0019 s/4.49 | 0.0079 s/4.36 | 0.0251 s/5.39 | -             |
| 16   | 0.0015 s/5.60 | 0.0053 s/6.47 | 0.0223 s/6.02 | -             |
| 20   | 0.0013 s/6.46 | 0.0041 s/8.42 | 0.0189 s/7.30 | -             |
| 30   | 0.0033 s/2.62 | 0.0087 s/3.95 | 0.0154 s/9.40 | -             |

数据分发时间 (s)：

| p    | $n=3600$ | $n=7200$ | $n=14400$ | $n=28800$ |
| ---- | -------- | -------- | --------- | --------- |
| 1    | 0.0319   | 0.1241   | 0.5058    | 1.9713    |
| 2    | 0.046    | 0.1808   | 0.7188    | 2.8517    |
| 3    | 0.0502   | 0.1971   | 0.7786    | 3.0673    |
| 4    | 0.0478   | 0.207    | 0.8096    | 3.2506    |
| 8    | 0.0526   | 0.218    | 0.8498    | -         |
| 12   | 0.0583   | 0.2273   | 0.8884    | -         |
| 16   | 0.0611   | 0.2363   | 0.924     | -         |
| 20   | 0.0615   | 0.2369   | 0.935     | -         |
| 30   | 0.5206   | 2.0391   | 8.1729    | -         |

过大的$n$和$p$组合导致了未知的`Segmentation fault`错误，暂时无法在集群上测试。

当矩阵规模n一定时，进程数p增大时，加速比也有增大的趋势，p越大，这种增长越缓慢；当p一定，n增大时，加速比也变大。尤其是当n较大，p较小时，不含数据分发的并行效率可以大于1。但包含了数据分发的并行效率如此之低，使我感到难以理解。

<img src=".\pic\1.png" alt="1" style="zoom:67%;" />

<img src=".\pic\2.png" alt="2" style="zoom:63%;" />

## Q5.PA3.6

### 解题思路

在上题的基础上，对派生数据类型的定义稍作修改，就可以将矩阵按块划分给子进程。

关于如何分发向量，我尝试了两种方法。

- 将整个向量广播给所有进程，让进程自己决定需要哪一部分向量进行运算，并将结果保存到`local_result`的相应位置，最后直接调用`MPI_Reduce`即可得到结果；
- 用`MPI_Comm_cart`和`MPI_Comm_sub`将进程按行和列分成多个通信组，调用`MPI_Scatter`将原向量分发给第一列的进程，再让后者在其所在的行通信组广播。收集结果时的顺序与上述过程相反。

实践表明，第一种方法在代码上要简洁的多，且总运行时间也更短，所以我最终提交的代码是基于第一种方法的。

### 代码实现

派生类型定义：

```c++
MPI_Datatype vec_t, block_t;
MPI_Type_vector(loc_n, loc_n, n, MPI_DOUBLE, &vec_t);
MPI_Type_create_resized(vec_t, 0, loc_n * sizeof(double), &block_t);
MPI_Type_commit(&block_t);
```

数据分发：

```c++
for (int i = 0; i < psize_sr; i++) {
    for (int j = 0; j < psize_sr; j++) {
        blocksize[i * psize_sr + j] = 1;
        // 偏移量 (i * n + j) * loc_n * sizeof(double)
        stride[i * psize_sr + j] = i * n + j;
    }
}

MPI_Scatterv(matrix.get(), blocksize.get(), stride.get(), block_t, loc_mat.get(), loc_n * loc_n, MPI_DOUBLE, 0, comm_world);
MPI_Bcast(vector.get(), n, MPI_DOUBLE, 0, comm_world);
```

局部结果计算：

```c++
int vec_off = (pid % psize_sr) * loc_n, res_off = (pid / psize_sr) * loc_n;
for (int i = 0; i < loc_n; i++) {
    double tmp = 0;
    for (int j = 0; j < loc_n; j++) {
        tmp += loc_mat[i * loc_n + j] * vector[vec_off + j];
      }
    loc_res[res_off + i] = tmp;
}
```

### 测试结果

串行结果与并行结果之差的范数基本为零，可见结果的正确性。

| p    | n     | 数据分发时间 (s) | 计算时间 (s) | 总时间 (s) | 考虑数据分发 加速比 | 不考虑数据分发 加速比 |
| ---- | ----- | ---------------- | ------------ | ---------- | ------------------- | --------------------- |
| 1    | 3600  | 0.0320           | 0.0085       | 0.0405     | 0.21                | 1.00                  |
| 1    | 7200  | 0.1239           | 0.0329       | 0.1568     | 0.21                | 1.00                  |
| 1    | 14400 | 0.4964           | 0.1351       | 0.6315     | 0.21                | 1.00                  |
| 1    | 28800 | 2.0367           | 0.6580       | 2.6947     | 0.25                | 1.01                  |
| 4    | 3600  | 0.0464           | 0.0024       | 0.0488     | 0.17                | 3.51                  |
| 4    | 7200  | 0.2101           | 0.0090       | 0.2191     | 0.15                | 3.65                  |
| 4    | 14400 | 0.7965           | 0.0387       | 0.8352     | 0.17                | 3.69                  |
| 4    | 28800 | 3.2985           | 0.1572       | 3.4557     | 0.19                | 4.23                  |
| 9    | 3600  | 0.0494           | 0.0022       | 0.0516     | 0.17                | 4.04                  |
| 9    | 7200  | 0.2375           | 0.0075       | 0.2450     | 0.15                | 4.75                  |
| 9    | 14400 | 0.8408           | 0.0274       | 0.8682     | 0.16                | 4.93                  |
| 16   | 3600  | 0.0589           | 0.0016       | 0.0605     | 0.14                | 5.32                  |
| 16   | 7200  | 0.2311           | 0.0054       | 0.2365     | 0.14                | 6.29                  |
| 16   | 14400 | 0.9290           | 0.0231       | 0.9521     | 0.14                | 6.04                  |

和PA3-5的结果基本一致。

<img src=".\pic\3.png" alt="3" style="zoom:67%;" />

<img src=".\pic\4.png" alt="4" style="zoom:67%;" />