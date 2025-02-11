# Sieve of Eratosthenes Implementation using OpenMP

## Logic Explanation

The program implements three versions of the Sieve of Eratosthenes algorithm:

1. Cache Unfriendly Version

   - Uses a single boolean array
   - Marks composites directly in the full array
   - Simple but cache inefficient

2. Cache Friendly Version

   - Uses segmented approach
   - Processes numbers in smaller chunks
   - Better cache utilization
   - Reduces memory access patterns

3. Parallel Version
   - Combines parallelism with cache optimization
   - Uses OpenMP for parallel processing
   - Maintains thread-local arrays

## Code Explanation

## Code Explanation

1. **Mark Function** (`mark`)

   - Input: boolean array, starting position, step size, and limit
   - Purpose: Marks all multiples of a number as composite (not prime)
   - Works by: Starting from position `i`, marks every `step`th number as true (composite)
   - Used by all three implementations as a helper function

2. **Cache Unfriendly Implementation** (`cache_unfriendly_sieve`)

   - Uses a single large boolean array for the entire range
   - Steps:
     1. Creates array of size n+1 initialized to false
     2. For each number from 2 to √n:
        - If number is not marked composite, mark all its multiples
     3. Counts unmarked numbers (these are primes)
   - Simple but inefficient due to scattered memory access

3. **Cache Friendly Implementation** (`cache_friendly_sieve`)

   - Uses segmented approach for better cache usage
   - Steps:
     1. First finds all primes up to √n using small array
     2. Stores these small primes in separate array
     3. Divides remaining range into windows
     4. For each window:
        - Creates small segment array
        - Marks composites using stored primes
        - Counts primes in segment
   - More efficient due to better memory access patterns

4. **Parallel Implementation** (`parallel_sieve`)

   - Combines cache-friendly approach with OpenMP parallelization
   - Steps:
     1. Sequential part:
        - Finds initial prime factors up to √n
        - Stores these factors for later use
     2. Parallel part:
        - Divides work into windows
        - Each thread:
          - Gets own composite array
          - Processes assigned window
          - Marks composites using stored factors
          - Counts primes in its window
     3. Combines results using reduction
   - Most efficient for large numbers due to parallel processing

5. **Main Function**

   - Tests three implementations with different input sizes
   - Measures execution time using OpenMP timer
   - Displays results in formatted table
   - Shows speed comparison between implementations

6. **Base Marking Function**

   ```c
   static inline long mark(bool composite[], long i, long step, long limit)
   ```

   - Marks composite numbers in the array
   - Used by all three implementations

7. **Cache Unfriendly Implementation**

   ```c
   long cache_unfriendly_sieve(long n)
   ```

   - Allocates single large array
   - Sequential marking of composites
   - Simple but less efficient

8. **Cache Friendly Implementation**

   ```c
   long cache_friendly_sieve(long n)
   ```

   - Uses segmented approach
   - Processes numbers in windows
   - Better cache utilization

9. **Parallel Implementation**
   ```c
   #pragma omp parallel
   ```
   - Uses OpenMP parallelization
   - Thread-local arrays
   - Reduction for counting primes

## Compilation Instructions

```bash
gcc -fopenmp sieve_euro.c -o sieve -lm
```

## Performance Analysis

- Tests with three input sizes:
  - 1 million
  - 10 million
  - 100 million
- Compares execution times:
  - Cache unfriendly version
  - Cache friendly version
  - Parallel version
- Measures using `omp_get_wtime()`

## Example Output

```bash
./sieve

Sieve of Eratosthenes - Prime Number Counting
============================================

+---------------+----------------------------+----------------------------+----------------------------+
|   Input Size  |  Cache Unfriendly (sec)   |   Cache Friendly (sec)    |     Parallel (sec)        |
+---------------+----------------------------+----------------------------+----------------------------+
|           1M |        78498 (0.005201 s)      |        78498 (0.005801 s)      |        78498 (0.015791 s)      |
|          10M |       664579 (0.105846 s)      |       664579 (0.057394 s)      |       664579 (0.021252 s)      |
|         100M |      5761455 (1.387309 s)      |      5761455 (0.610438 s)      |      5761455 (0.120343 s)      |
+---------------+----------------------------+----------------------------+----------------------------+
```
