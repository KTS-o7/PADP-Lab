// This is a simple MPI program with 4 processes.
// The master process (rank 0) sends tasks to the worker processes.
// Each worker process calculates PI * its rank and sends the result back to the master.
// The master receives and prints the results.

#include <stdio.h>
#include <mpi.h>
#include <string.h>

#define BUFFER_SIZE 100 // Define buffer size for message communication

int main(int argc, char *argv[])
{
    int rank, size;
    char buffer[BUFFER_SIZE];
    MPI_Status status; // Structure to store status of MPI operations

    // Initialize the MPI environment
    // argc, argv: Command-line arguments (not used here but required by MPI_Init)
    MPI_Init(&argc, &argv);
    
    // Get the rank (ID) of the calling process in MPI_COMM_WORLD
    // MPI_COMM_WORLD: Default communicator containing all processes
    // &rank: Stores the rank of the calling process (0 to size-1)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Get the total number of processes in MPI_COMM_WORLD
    // MPI_COMM_WORLD: Default communicator
    // &size: Stores the number of processes in the communicator
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Simulate computation: Each process calculates PI * its rank
    double computation_result = rank * 3.14159;

    if (rank == 0) // Master process (rank 0)
    {
        printf("Master process 0 starting coordination...\n");
        for (int i = 1; i < size; i++)
        {
            // Format task message
            sprintf(buffer, "Task assignment for process %d: Calculate PI * %d", i, i);
            
            // Send message to process i
            // buffer: Message to send
            // BUFFER_SIZE: Size of the message
            // MPI_CHAR: Data type of the message
            // i: Destination process rank
            // 0: Message tag (identifier)
            // MPI_COMM_WORLD: Communicator
            MPI_Send(buffer, BUFFER_SIZE, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    }
    else // Worker processes (rank 1 to size-1)
    {
        // Receive message from master process (rank 0)
        // buffer: Storage for received message
        // BUFFER_SIZE: Maximum size of the message
        // MPI_CHAR: Data type of the message
        // 0: Source process rank (master process)
        // 0: Message tag
        // MPI_COMM_WORLD: Communicator
        // &status: Stores details about the received message
        MPI_Recv(buffer, BUFFER_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        
        printf("Process %d received task: %s\n", rank, buffer);
        fflush(stdout); // Flush output to ensure immediate display
        
        // Format result message
        sprintf(buffer, "Process %d completed calculation: Result = %.4f", rank, computation_result);
        
        // Send result back to master process (rank 0)
        MPI_Send(buffer, BUFFER_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) // Master process collects results
    {
        for (int i = 1; i < size; i++)
        {
            // Receive results from worker processes
            MPI_Recv(buffer, BUFFER_SIZE, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
            printf("Master process 0 received: %s\n", buffer);
        }
    }

    // Finalize the MPI environment (cleanup resources)
    MPI_Finalize();
    return 0;
}