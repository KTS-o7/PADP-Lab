# Matrix Multiplication using OpenMP

## Logic Explanation

The program implements parallel matrix multiplication using OpenMP. Here's a detailed breakdown:

1. **Matrix Structure**
   - The program performs matrix multiplication on dynamically allocated square matrices of different sizes.
   - The matrices are initialized with random values.
   - The resulting matrix stores the computed product.

2. **Multiplication Process**
   - For each element `result[i][j]`, compute the sum:
     ```c
     sum += matrix1[i][k] * matrix2[k][j];
     ```
   - This operation is performed for all elements in the result matrix.
   - OpenMP is used to parallelize the computation and speed up execution.

## Code Explanation

The program leverages OpenMP for parallel processing. Here's a detailed breakdown:

### Key Components

1. **Memory Allocation**

   ```c
   int **matrix1 = (int **)malloc(rows * sizeof(int *));
   ```
   - Dynamically allocates memory for matrices.
   - Uses double pointers for 2D array representation.

2. **Matrix Initialization**

   ```c
   #pragma omp parallel for collapse(2)
   for (i = 0; i < rows; i++) {
       for (j = 0; j < cols; j++) {
           matrix1[i][j] = rand() % 100;
           matrix2[i][j] = rand() % 100;
           result[i][j] = 0;
       }
   }
   ```
   - Initializes matrices with random values.
   - Uses OpenMP's `collapse(2)` to parallelize both loops.

3. **Parallel Implementation**

   ```c
   #pragma omp parallel for collapse(2) schedule(static)
   for (i = 0; i < rows; i++) {
       for (j = 0; j < cols; j++) {
           int sum = 0;

           #pragma omp simd reduction(+:sum)
           for (k = 0; k < cols; k++) {
               sum += matrix1[i][k] * matrix2[k][j];
           }

           result[i][j] = sum;
       }
   }
   ```
   - `#pragma omp parallel for collapse(2)`: Parallelizes both row and column iterations.
   - `schedule(static)`: Distributes iterations evenly among threads.
   - `#pragma omp simd reduction(+:sum)`: Uses SIMD vectorization to accelerate computation.

### OpenMP Directives and Performance Impact

1. **Thread Management**

   ```c
   omp_set_num_threads(num_threads[t]);
   ```
   - Dynamically sets the number of threads before execution.
   - Higher thread count generally improves performance, but overhead may occur for very small matrices.

2. **Loop Parallelization**

   ```c
   #pragma omp parallel for collapse(2) schedule(static)
   ```
   - `collapse(2)`: Flattens two nested loops for better parallel efficiency.
   - `schedule(static)`: Ensures balanced workload distribution among threads.

3. **Vectorization Optimization**

   ```c
   #pragma omp simd reduction(+:sum)
   ```
   - Enables SIMD (Single Instruction, Multiple Data) execution.
   - Each thread processes multiple elements simultaneously, improving cache efficiency.

### Performance Analysis

- Measures execution time using `omp_get_wtime()`.
- Tests different thread configurations.
- Demonstrates parallel speedup potential.
- Compares time taken by different thread counts.

## Compilation Instructions

Compile the program with OpenMP support:

```bash
gcc -fopenmp Matrix_mul.c -o mat_mul
```

## Example Output

```bash
./mat_mul
+------------+------------+------------+------------+------------+
| MatrixSize | 1 Thread  | 2 Thread  | 4 Thread  | 8 Thread  |
+------------+------------+------------+------------+------------+
|        100 |   0.000123 |   0.000089 |   0.000054 |   0.000032 |
|        400 |   0.001512 |   0.000876 |   0.000523 |   0.000398 |
|       1600 |   0.065712 |   0.032134 |   0.017654 |   0.010432 |
|       3200 |   0.512342 |   0.275132 |   0.142567 |   0.082314 |
+------------+------------+------------+------------+------------+
```

## Observations
- Execution time decreases as the number of threads increases.
- `collapse(2)` improves load balancing for larger matrices.
- `schedule(static)` ensures uniform work distribution across threads.
- `#pragma omp simd` enhances performance by leveraging CPU vector units.

## Effect of Excess Threads

When more threads are deployed than needed (e.g., more threads than available CPU cores), performance can degrade due to:

- Thread Overhead: Managing excessive threads incurs scheduling overhead.

- Context Switching: More threads than available cores lead to frequent context switching, reducing efficiency.

- Load Imbalance: Some threads may remain idle or underutilized.

- Memory Bandwidth Limitations: More threads increase contention for shared memory, limiting performance gains.

In practice, using more threads than available physical cores rarely improves performance and may even slow down execution.


