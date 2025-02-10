#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SEED 3655942  // Seed for random number generation
#define ROOT 0        // Define root process (rank 0) for collecting results

int main(int argc, char *argv[])
{
    // Initialize MPI environment
    MPI_Init(&argc, &argv);

    int rank, size;
    
    // Get the rank (ID) of the current process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Get the total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Number of random points for Monte Carlo simulation
    int n = 100000; 

    double t; // Variable to store execution time

    // Start the timer in the root process
    if (rank == ROOT)
    {
        t = MPI_Wtime();
    }

    int count = 0;      // Local count of points inside the circle
    int red_count = 0;  // Reduced count (sum of counts from all processes)

    // Set a unique random seed for each process
    srand(SEED + rank);

    // Each process generates (n / size) random points
    for (int i = 0; i < n / size; i++)
    {
        double x = (double)rand() / RAND_MAX; // Random x coordinate
        double y = (double)rand() / RAND_MAX; // Random y coordinate
        double z = x * x + y * y;            // Compute distance from origin

        // If the point lies inside the unit circle, increase local count
        if (z <= 1.0)
            count++;
    }

    // Reduce all local counts to obtain the global count at the root process
    MPI_Reduce(&count, &red_count, 1, MPI_INT, MPI_SUM, ROOT, MPI_COMM_WORLD);

    /*
     * MPI_Reduce Parameters:
     * - &count:        Local count to send
     * - &red_count:    Variable to store the reduced sum at the root
     * - 1:            Number of elements to reduce
     * - MPI_INT:      Data type of elements
     * - MPI_SUM:      Reduction operation (sum of all counts)
     * - ROOT:         Rank 0 collects the result
     * - MPI_COMM_WORLD: MPI communicator (all processes)
     */

    // Root process calculates and prints the final result
    if (rank == ROOT)
    {
        t = MPI_Wtime() - t; // Compute total execution time

        // Estimate the value of π using Monte Carlo formula
        double pi = (double)red_count / n * 4;

        // Display results
        printf("Time taken: %f seconds\n", t);
        printf("Number of MPI processes: %d\n", size);
        printf("Total number of trials: %d\n", n);
        printf("Estimated value of Pi: %f\n", pi);
    }

    // Finalize the MPI environment
    MPI_Finalize();
    return 0;
}
