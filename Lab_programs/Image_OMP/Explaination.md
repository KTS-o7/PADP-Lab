# Image Processing using OpenMP

## Logic Explanation

The program implements parallel image processing using OpenMP. Here's a detailed breakdown:

1. Image Processing Overview

   - Converts RGB images to modified grayscale images
   - Tests different OpenMP scheduling policies
   - Handles multiple image sizes and chunk sizes
   - Visualizes thread distribution through color coding

2. Core Processing Tasks
   - Reads PNG image file using GD library
   - Processes pixels in parallel using OpenMP
   - Assigns different colors based on thread ID
   - Measures performance across different configurations

## OpenMP Directives and Scheduling

### Pragma Directives

```c
#pragma omp parallel for private(variables) schedule(type, chunk_size)
```

- `parallel for`: Creates parallel threads for loop execution
- `private`: Creates local copies of variables for each thread
- `schedule`: Defines how loop iterations are distributed

### Scheduling Types

1. **Static Scheduling**

   - Divides iterations equally among threads
   - Best for balanced workloads
   - Fixed distribution pattern
   - Example: With chunk_size=100, each thread gets 100 consecutive iterations

2. **Dynamic Scheduling**

   - Runtime load balancing
   - Threads request new chunks when finished
   - Good for uneven workloads
   - Higher overhead due to task distribution

3. **Guided Scheduling**
   - Starts with large chunks, decreases over time
   - Self-adjusting chunk size
   - Balances overhead and load distribution
   - Chunk size = remaining_iterations/number_of_threads

## Code Implementation

### Key Components

1. **Pixel Processing Function**

   ```c
   void process_pixels(gdImagePtr img, int x, int h)
   ```

   - Processes individual columns of pixels
   - Calculates grayscale value from RGB
   - Assigns colors based on thread ID

2. **Image Processing Function**
   ```c
   double process_image(char *iname, char *oname, int num_threads, const char *schedule_type, int chunk_size)
   ```
   - Handles image I/O operations
   - Implements parallel processing with OpenMP directives
   - Supports different scheduling policies

## Performance Analysis

- Tests various combinations:
  - Image sizes (512x512)
  - Schedule types (static, dynamic, guided)
  - Chunk sizes (1, 10, 50, 100)
  - Fixed 4 threads

## Compilation Instructions

```bash
gcc -fopenmp image_omp.c -o image_proc -lgd
./image_omp ./input_512x512.png  ./output.png
```

## Example Output Format

```
Performance Results (Time in seconds)
=====================================
Size    Schedule        Chunk   Time
512x512 static          1       0.007814
512x512 static          10      0.005296
512x512 static          50      0.004720
512x512 static          100     0.005632
512x512 dynamic         1       0.006901
512x512 dynamic         10      0.006104
512x512 dynamic         50      0.004669
512x512 dynamic         100     0.004690
512x512 guided          1       0.004686
512x512 guided          10      0.004267
512x512 guided          50      0.003198
512x512 guided          100     0.003128
```
