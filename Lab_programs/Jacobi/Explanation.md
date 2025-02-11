# Parallel Jacobi Iteration using OpenACC

## Introduction

The given program implements the Jacobi iterative method for solving the Poisson equation using a 2D grid. It utilizes OpenACC for parallelization, making use of GPU acceleration. The Jacobi method is an iterative technique to solve linear systems of equations and is particularly useful for large-scale problems where direct solvers are computationally expensive.

## Program Overview

The program follows these main steps:

1. **Memory Allocation:** Dynamically allocates memory for two 2D arrays `u` (solution grid) and `f` (right-hand side function).
2. **Initialization:** The solution grid `u` is initialized to zero, and `f` is set to a constant value of `1.0`.
3. **Jacobi Iteration:** The Jacobi method is applied iteratively until convergence is reached or the maximum number of iterations is met.
4. **Time Measurement:** The execution time of the Jacobi method is recorded using `gettimeofday()`.
5. **Result Output:** The final difference between iterations and execution time is printed.
6. **Memory Deallocation:** The allocated memory is freed before program termination.

## Jacobi Iteration Logic

The Jacobi method is an iterative algorithm to solve the equation:

$$
Ax = b
$$

For a 2D Poisson equation, the update formula is given by:

$$
u_{i,j}^{(k+1)} = \frac{1}{4} \left( u_{i-1,j}^{(k)} + u_{i+1,j}^{(k)} + u_{i,j-1}^{(k)} + u_{i,j+1}^{(k)} - f_{i,j} \right)
$$

where:

- \( u_{i,j}^{(k)} \) is the value at grid point \((i,j)\) at iteration \(k\)
- \( f_{i,j} \) is the right-hand side value at \((i,j)\)
- The process is repeated until the maximum change between iterations is below a specified tolerance (`TOL`).

### Parallelization with OpenACC

The program uses OpenACC to parallelize the computation. The key OpenACC directives used are:

- `#pragma acc data`: Allocates memory on the GPU and defines data movement.
- `#pragma acc parallel loop`: Specifies parallel execution of loops.
- `#pragma acc kernels`: Specifies a region of code for parallel execution.
- `#pragma acc update`: Transfers data between host and device memory.
- `#pragma acc enter/exit data`: Controls explicit data allocation and deallocation on the GPU.
- `#pragma acc copyin`: Transfers data from host to device at the beginning.
- `#pragma acc copyout`: Transfers data from device to host at the end.
- `#pragma acc copy`: Transfers data between host and device (both directions).
- `#pragma acc create`: Allocates memory on the device but does not initialize it.
- `reduction(max : max_diff)`: Computes the maximum difference in parallel.

The parallelization occurs within the main Jacobi iteration loop:

```c
#pragma acc parallel loop reduction(max : max_diff) collapse(2)
for (int i = 1; i < N - 1; i++) {
    for (int j = 1; j < M - 1; j++) {
        u_new[i][j] = 0.25 * (u[i - 1][j] + u[i + 1][j] + u[i][j - 1] + u[i][j + 1] - f[i][j]);
        double diff = fabs(u_new[i][j] - u[i][j]);
        if (diff > max_diff) {
            max_diff = diff;
        }
    }
}
```

This ensures that the computation is distributed across available GPU cores, improving performance.

## Example Calculation

Consider a 3x3 grid with initial values:

$$
\begin{bmatrix}
0 & 0 & 0 \\
0 & 0 & 0 \\
0 & 0 & 0
\end{bmatrix}
$$

With \( f_{i,j} = 1 \), applying the Jacobi update formula for the central element (1,1):

$$
u_{1,1}^{(1)} = \frac{1}{4} (0 + 0 + 0 + 0 - 1) = -0.25
$$

After a few iterations, the grid values will gradually change, converging to a steady-state solution.

## Performance Considerations

- **Grid Size (N x M):** A larger grid provides a more accurate solution but increases computation time.
- **Convergence Tolerance (TOL):** A smaller tolerance leads to a more accurate result but requires more iterations.
- **GPU Acceleration:** OpenACC significantly speeds up the iteration process compared to a purely CPU-based implementation.


