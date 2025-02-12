#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void matrix_multiply(int rows, int cols);

int main()
{
    // Array of matrix sizes to be tested
    int matrix_sizes[] = { 100, 400, 1600, 3200 };
    int num_sizes = sizeof(matrix_sizes) / sizeof(matrix_sizes[0]);

    // Print table header
    printf("\n");
    printf("+------------+------------+------------+------------+------------+\n");
    printf("| %10s | %10s | %10s | %10s | %10s |\n","MatrixSize", "1 Thread", "2 Thread", "4 Thread", "8 Thread" );
    printf("+------------+------------+------------+------------+------------+\n");

    // Iterate over different matrix sizes
    for (int i = 0; i < num_sizes; i++)
    {
        matrix_multiply(matrix_sizes[i], matrix_sizes[i]);
    }

    printf("+------------+------------+------------+------------+------------+\n");
    return 0;
}

/**
 * Function to perform matrix multiplication and measure execution time using OpenMP.
 * The function dynamically allocates memory for matrices, initializes them, performs 
 * multiplication with different numbers of threads, and prints the execution times.
 *
 * @param rows: Number of rows in the matrices.
 * @param cols: Number of columns in the matrices.
 */
void matrix_multiply(int rows, int cols)
{
    int i, j, k;

    // Dynamically allocate memory for three matrices: matrix1, matrix2, and result.
    int **matrix1 = (int **)malloc(rows * sizeof(int *));
    int **matrix2 = (int **)malloc(rows * sizeof(int *));
    int **result = (int **)malloc(rows * sizeof(int *));
    
    for (i = 0; i < rows; i++)
    {
        matrix1[i] = (int *)malloc(cols * sizeof(int));
        matrix2[i] = (int *)malloc(cols * sizeof(int));
        result[i] = (int *)malloc(cols * sizeof(int));
    }

    // Initialize matrices with random values
    // OpenMP is used to parallelize the nested loop using collapse(2), 
    // which combines both loops into a single iteration space.
    #pragma omp parallel for collapse(2)
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            matrix1[i][j] = rand() % 100;  // Assign random values
            matrix2[i][j] = rand() % 100;
            result[i][j] = 0;  // Initialize result matrix
        }
    }

    // Array containing different numbers of threads to be tested
    int num_threads[] = {1, 2, 4, 8};
    double times[4]; // Array to store execution times

    // Perform matrix multiplication with different thread counts
    for (int t = 0; t < 4; t++)
    {
        double start_time = omp_get_wtime();  // Start timing
        omp_set_num_threads(num_threads[t]);  // Set the number of threads

        // Parallelizing the outer loops using OpenMP
        // `collapse(2)` ensures that both row and column loops are parallelized together
        // `schedule(static)` ensures uniform workload distribution among threads
        #pragma omp parallel for collapse(2) schedule(static)
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < cols; j++)
            {
                int sum = 0;

                // The innermost loop performs the multiplication and summation
                // Using `#pragma omp simd reduction(+:sum)` enables SIMD vectorization
                // `reduction(+:sum)` ensures that different threads sum correctly
                #pragma omp simd reduction(+:sum)
                for (k = 0; k < cols; k++)
                {
                    sum += matrix1[i][k] * matrix2[k][j];
                }

                result[i][j] = sum;  // Store the computed value in the result matrix
            }
        }

        double end_time = omp_get_wtime();  // End timing
        times[t] = end_time - start_time;  // Store execution time
    }

    // Print execution times for different thread counts
    printf("| %10d | %10.6f | %10.6f | %10.6f | %10.6f |\n", 
            rows, times[0], times[1], times[2], times[3]);

    // Free dynamically allocated memory to prevent memory leaks
    for (i = 0; i < rows; i++)
    {
        free(matrix1[i]);
        free(matrix2[i]);
        free(result[i]);
    }
    free(matrix1);
    free(matrix2);
    free(result);
}
