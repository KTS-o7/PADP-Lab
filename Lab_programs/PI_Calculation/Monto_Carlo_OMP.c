// Monte Carlo Method for Estimating the Value of PI using OpenMP
//
// This program estimates the value of PI using a statistical approach called the Monte Carlo method.
// The key idea is to randomly generate points in a unit square and count how many fall inside a unit circle.
//
// Algorithm:
// 1. Consider a square of side 1 unit (area = 1).
// 2. Consider a circle of radius 0.5 units inside this square (area = PI/4).
// 3. Randomly generate points (x, y) inside the square.
// 4. Count the points that fall inside the quarter circle using the condition: x² + y² ≤ 1.
// 5. Use the formula: PI ≈ 4 * (points inside circle) / (total points).
//
// Parallelization:
// - The program uses OpenMP to parallelize the random point generation and counting process.
// - Execution time is measured for different thread counts and input sizes.
//
// Compilation Command:
//   gcc -fopenmp -o Monto_Carlo_OMP Monto_Carlo_OMP.c
//
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define SEED 35791246  // Seed value for random number generation

// Function prototype
void calculate_pi(int n, int num_threads[], int num_threads_size, int result);

int main()
{
    // Different input sizes (number of points generated)
    int niter[] = {10000, 100000, 1000000, 10000000};

    // Different numbers of OpenMP threads to be tested
    int num_threads[] = {1, 2, 4, 8};
    
    // Determine the number of thread configurations
    int num_threads_size = sizeof(num_threads) / sizeof(num_threads[0]);

    // Flag to control output format:
    // result = 1 -> Print estimated PI values
    // result = 0 -> Print only execution times
    int result = 0;  

    // Print table header with adjusted column sizes
    printf("\n+--------------+------------+------------+------------+------------+\n");
    printf("|  Input Size  | Thread 1   | Thread 2   | Thread 4   | Thread 8   |\n");
    printf("+--------------+------------+------------+------------+------------+\n");

    // Run Monte Carlo simulation for each input size
    for (int iter = 0; iter < sizeof(niter) / sizeof(niter[0]); iter++)
        calculate_pi(niter[iter], num_threads, num_threads_size, result);

    // Print table footer
    printf("+--------------+------------+------------+------------+------------+\n");

    return 0;
}

/**
 * Function: calculate_pi
 * -----------------------
 * Runs the Monte Carlo simulation to estimate PI using OpenMP parallelization.
 * 
 * @param n: Number of random points to generate
 * @param num_threads: Array containing different thread counts to test
 * @param num_threads_size: Number of thread configurations in num_threads[]
 * @param result: Flag (1 = Print estimated PI values, 0 = Print execution times)
 */
void calculate_pi(int n, int num_threads[], int num_threads_size, int result)
{
    double pi_values[num_threads_size]; // Store estimated PI values
    double times[num_threads_size];     // Store execution times

    for (int j = 0; j < num_threads_size; j++)
    {
        int count = 0;  // Count of points inside the circle

        // Set the number of OpenMP threads
        omp_set_num_threads(num_threads[j]);

        // Start measuring execution time
        double start_time = omp_get_wtime();

        // Parallel region
#pragma omp parallel
        {
            int seed = SEED + omp_get_thread_num();  // Ensure unique seed per thread
            int local_count = 0;  // Local count for each thread

#pragma omp for
            for (int i = 0; i < n; i++)
            {
                // Generate random (x, y) coordinates between 0 and 1
                double x = (double)rand_r(&seed) / RAND_MAX;
                double y = (double)rand_r(&seed) / RAND_MAX;

                // Check if the point falls inside the quarter-circle
                if (x * x + y * y <= 1)
                    local_count++;
            }

            // Atomic update to avoid race conditions
#pragma omp atomic
            count += local_count;
        }

        // Compute estimated PI value
        pi_values[j] = (double)count / n * 4;

        // Compute elapsed time
        times[j] = omp_get_wtime() - start_time;
    }

    // Print results in table format with adjusted width
    printf("| %-12d", n);
    for (int j = 0; j < num_threads_size; j++)
    {
        if (result)
            printf(" | %-10.6f", pi_values[j]);  // Print estimated PI value
        else
            printf(" | %-9.6fs", times[j]);    // Print execution time
    }
    printf(" |\n");
}
