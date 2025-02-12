# Parallel and Distributed Processing (PADP) Lab (21CS72)

---

### Overview

This repository contains lab programs for the Parallel and Distributed Processing (PADP) course. Each program demonstrates core parallel programming concepts using OpenMP or MPI.

### Table of Contents

| Program                               | Description                                         | Link to Explanation                                                            |
| ------------------------------------- | --------------------------------------------------- | ------------------------------------------------------------------------------ |
| Monte Carlo Method for PI Calculation | Parallel random sampling for PI approximation       | [Explanation](./Lab_programs/PI_Calculation/Explaination.md) |
| Matrix Multiplication (OpenMP)        | Parallel matrix multiplication using OpenMP         | [Explanation](./Lab_programs/Matrix_Multiply/Explaination.md)                       |
| Sieve of Eratosthenes (OpenMP)        | Finding prime numbers using different approaches    | [Explanation](./Lab_programs/Sieve_Of_Erastothenes/Explaination.md)                       |
| MPI Message Passing                   | Demonstrates MPI-based inter-process communication  | [Explanation](./Lab_programs/MPI_message/Explaination.md)                      |
| Image Processing (OpenMP)             | RGB to modified grayscale conversion in parallel    | [Explanation](./Lab_programs/Image_OMP/Explaination.md)                        |
| Word Search (OpenMP)                  | Parallel word search in a mixture of sentences      | [Explanation](./Lab_programs/Wordsearch/Explaination.md)                       |
| Jacobi Iteration (OpenACC)    | Parallel Jacobi Iteration using OpenACC | [Explanation](./Lab_programs/Jacobi/Explanation.md)           |
| Dynamic Memory Allocation (OpenMP)    | Parallel matrix multiplication using dynamic memory | [Explanation](./Lab_programs/DynamicMemAlloc_MatMul/Explaination.md)           |

### Prerequisites

- GCC or Clang compiler with OpenMP support
- MPI compiler (e.g., mpicc) for MPI programs
- GD library (libgd) if running the Image Processing example

### Compilation & Execution

Below is a quick reference for running each program:

1. **Monte Carlo Method for PI Calculation**

```bash
mpicc Monto_Carlo_MPI.c -o monto_mpi
mpirun monto_mpi
```
<p style="text-align:center;">or</p>

```bash
gcc -fopenmp Monto_Carlo_OMP.c -o monto_omp
./monto_omp
```

2. **Matrix Multiplication (OpenMP)**

```bash
gcc -fopenmp matrix_mul.c -o matrix_mul
./matrix_mul
```

3. **Sieve of Eratosthenes (OpenMP)**

```bash
gcc -fopenmp sieve_euro.c -o sieve -lm
./sieve
```

4. **MPI Message Passing**

```bash
mpicc mpi_message.c -o mpi_message
mpirun -np 4 ./mpi_message
```

5. **Image Processing (OpenMP)**

```bash
gcc -fopenmp image_omp.c -o image_omp -lgd
./image_omp input.png output.png
```

6. **Word Search (OpenMP)**

```bash
gcc -fopenmp wordsearch.c -o wordsearch
./wordsearch
```

7. **Jacobi Iteration (OpenACC)**
```bash
gcc -fopenacc -o jacobi jacobi.c -lm
./jacobi
```

8. **Dynamic Memory Allocation (OpenMP)**

```bash
gcc -fopenmp dynamic_mem_alloc_matmul.c -o dynamic_mem_alloc_matmul
./dynamic_mem_alloc_matmul
```

### Usage Instructions

- For matrix multiplication, adjust row/column values directly in the code if needed.
- For sieve, specify input size (1M, 10M, 100M, etc.) within the code/CLI prompt.
- For Monte Carlo PI, pass the number of random points as a command line argument or enter interactively.
- For MPI message passing, ensure mpirun -np <num_procs> is used, with <num_procs> matching your setup.
- For image processing, provide input/output filenames and optionally tweak scheduling parameters.
- For word search, you need to keep an input text file named `words.txt` in the same directory. It should have a list of sentences separated by newlines.
- For dynamic memory allocation, adjust matrix size as needed. We actually need `nvc` compiler to run this program. So with GCC it's kind of redundant.

### References

- [OpenMP Documentation](https://www.openmp.org/resources/)
- [MPI Official Site](https://www.mpi-forum.org/)

### Contributors

- [Krishnatejaswi S](https://github.com/KTS-o7)
- [Joshua Elias Alva](https://github.com/joshalva23)
