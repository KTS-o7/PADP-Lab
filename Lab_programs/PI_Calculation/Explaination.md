# Monte Carlo Method for PI Calculation (OpenMP)

## Logic Explanation

The Monte Carlo method calculates PI using statistical random sampling. Here's a detailed breakdown:

1. Consider a 1x1 square containing a quarter circle of radius 1

   - Square area = 1 unit²
   - Quarter circle area = π/4 unit²

2. Random Point Generation:
   - Generate (x,y) coordinates between 0 and 1
   - Check if point lies inside circle using: x² + y² ≤ 1
   - Points inside circle/total points ≈ area ratio = π/4
   - Therefore, π ≈ 4 \* (points inside circle)/(total points)

## Code Explanation

The program leverages OpenMP for parallel processing. Here's a detailed breakdown:

### Key OpenMP Directives

1. **Thread Management**

```c
omp_set_num_threads(n)  // Sets number of threads
#pragma omp parallel    // Creates parallel region
```

2. **Work Distribution**

```c
#pragma omp for        // Distributes loop iterations
```

3. **Synchronization**

```c
#pragma omp atomic     // Ensures atomic updates
```

# Pragma Directives

```c
#pragma omp atomic
```

This directive ensures that the increment operation on the `count` variable is performed atomically, preventing
For example, if two threads try to increment the `count` variable simultaneously, the `atomic` directive ensures that the operation is performed in a thread-safe manner.
It works by locking the memory location of the variable during the increment operation, ensuring that only one thread can access it at a time.

```c
#pragma omp parallel
```

This directive creates a parallel region where multiple threads are spawned to execute the code block enclosed by the directive. Each thread executes the code independently, and the work is distributed among the threads by the OpenMP runtime.

For example, in the Monte Carlo method for calculating PI, the parallel region is used to distribute the work of generating random points and checking if they lie inside the circle among multiple threads.

So the code from the Monte Carlo method for calculating PI would look like this:

```c
#pragma omp parallel
{
    // Code block executed by each thread
    // Generate random points and check if they lie inside the circle
}
```

```c
#pragma omp for
```

This directive is used to distribute loop iterations among multiple threads in a parallel region. It divides the loop iterations into chunks and assigns each chunk to a different thread for parallel execution.

### Implementation Details

1. **Thread Configuration**

   - Tests with varying thread counts: 1, 2, 4, and 8
   - Each configuration:
     - Sets thread count using `omp_set_num_threads()`
     - Measures execution time using `omp_get_wtime()`

2. **Parallel Processing Strategy**

   - Each thread independently:
     - Receives unique random seed
     - Processes its assigned portion of points
     - Maintains local count for thread safety
     - Updates global count atomically

3. **Random Number Generation**

   - Uses thread-safe `rand_r()` function
   - Each thread has unique seed to avoid correlation
   - Numbers scaled to [0,1] range using:

   ```c
   double x = (double)rand_r(&seed) / RAND_MAX;
   ```

4. **Performance Optimization**

   - Local counting reduces atomic operations
   - Thread-safe random number generation
   - Minimal synchronization requirements

5. **Results Collection**
   - Times each configuration
   - Calculates PI approximation
   - Displays formatted results table

### Performance Analysis

The implementation demonstrates:

- Parallel speedup with increasing threads
- Accuracy maintenance across thread counts
- Scalability with input size
- Efficient resource utilization

This parallel implementation significantly reduces computation time while maintaining statistical accuracy in PI calculation.

## Compilation Instructions

```bash
gcc -fopenmp Pi_calc_monto_carlo.c -o Pi_calc_monto_carlo
```

## Example Output

```bash
./Pi_calc_monto_carlo
Enter the number of iterations (recommend 10K+): 100000000000

+---------------+----------------+---------------+------------------+
| Num Threads   | Num Points     | Estimated PI  | Time Taken (s)  |
+---------------+----------------+---------------+------------------+
| 1             | 1215752192     | 3.141591      | 16.287262        |
| 2             | 1215752192     | 3.141589      | 8.135068         |
| 4             | 1215752192     | 3.141567      | 4.134700         |
| 8             | 1215752192     | 3.141619      | 2.280567         |
+---------------+----------------+---------------+------------------+
```

---

## MPI Implementation

```bash
mpicc -o mpi_pi_calc mpi_pi_calc.c
mpirun -np 4 ./mpi_pi_calc
```
