// We need to write a program to convert a RGB image to a B/W image using OpenMP.
// We need to demonstrate the performance of different scheduling policies for various chunk sizes.
// We also need to analyse the scheduling patterns by assigning a single color value for an image for each thread.
// Tabulate the following
// Image sizes (Width x Height): 512 x 512, 1024 x 1024, 2048 x 2048, 4096 x 4096
// Schedule types: default, static, dynamic, guided

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <gd.h>
#include <error.h>
#include <string.h>

/**
 * Function: process_pixels
 * -------------------------
 * Converts an image's pixel colors to grayscale while assigning a unique tint
 * to each thread's portion of the image.
 *
 * Parameters:
 *    img - Pointer to the GD image structure.
 *    x   - Column index to process.
 *    h   - Height of the image (number of rows to process).
 */
void process_pixels(gdImagePtr img, int x, int h)
{
    int y, color, red, green, blue, tmp, tid;
    for (y = 0; y < h; y++)
    {
        tid = omp_get_thread_num(); // Get thread ID
        color = gdImageGetPixel(img, x, y);
        red = gdImageRed(img, color);
        green = gdImageGreen(img, color);
        blue = gdImageBlue(img, color);
        tmp = (red + green + blue) / 3; // Compute grayscale intensity

        // Assign unique color tint per thread
        switch (tid)
        {
        case 0:
            color = gdImageColorAllocate(img, tmp, 0, 0);
            break;
        case 1:
            color = gdImageColorAllocate(img, 0, tmp, 0);
            break;
        case 2:
            color = gdImageColorAllocate(img, 0, 0, tmp);
            break;
        default:
            color = gdImageColorAllocate(img, tmp, tmp, tmp);
        }
        gdImageSetPixel(img, x, y, color); // Set pixel color
    }
}

/**
 * Function: process_image
 * -------------------------
 * Reads an input PNG image, converts it to grayscale, applies a scheduling strategy,
 * and writes the output to a new PNG file. Uses OpenMP for parallel processing.
 *
 * Parameters:
 *    iname        - Input image file name (PNG format).
 *    oname        - Output image file name (PNG format).
 *    num_threads  - Number of OpenMP threads to use.
 *    schedule_type - Type of OpenMP scheduling policy ("static", "dynamic", "guided").
 *    chunk_size   - Size of chunks for OpenMP scheduling.
 *
 * Returns:
 *    Execution time (in seconds) for processing the image.
 */
double process_image(char *iname, char *oname, int num_threads, const char *schedule_type, int chunk_size)
{
    FILE *fp = NULL;
    gdImagePtr img;
    int x, h, w;

    // Open input image file
    if ((fp = fopen(iname, "r")) == NULL)
    {
        error(1, 0, "Error: %s not found", iname);
    }
    img = gdImageCreateFromPng(fp);
    fclose(fp);

    w = gdImageSX(img); // Get image width
    h = gdImageSY(img); // Get image height

    double start = omp_get_wtime(); // Start timing
    omp_set_num_threads(num_threads);

    // Apply parallel processing with different scheduling strategies
    if (strcmp(schedule_type, "static") == 0)
    {
#pragma omp parallel for schedule(static, chunk_size)
        for (x = 0; x < w; x++)
        {
            process_pixels(img, x, h);
        }
    }
    else if (strcmp(schedule_type, "dynamic") == 0)
    {
#pragma omp parallel for schedule(dynamic, chunk_size)
        for (x = 0; x < w; x++)
        {
            process_pixels(img, x, h);
        }
    }
    else if (strcmp(schedule_type, "guided") == 0)
    {
#pragma omp parallel for schedule(guided, chunk_size)
        for (x = 0; x < w; x++)
        {
            process_pixels(img, x, h);
        }
    }

    double end = omp_get_wtime(); // End timing

    // Save processed image to output file
    if ((fp = fopen(oname, "w")) == NULL)
    {
        error(1, 0, "Error: %s not found", oname);
    }
    gdImagePng(img, fp);
    fclose(fp);
    gdImageDestroy(img);

    return end - start;
}

/**
 * Function: main
 * --------------
 * Runs performance tests on various image sizes, scheduling strategies, and chunk sizes.
 *
 * Returns:
 *    0 on successful execution.
 */
int main()
{
    // Image sizes to test
    const int sizes[] = {512, 1024, 2048, 4096};
    // OpenMP scheduling policies to test
    const char *schedules[] = {"static", "dynamic", "guided"};
    // Chunk sizes to test
    const int chunk_sizes[] = {1, 10, 50, 100};
    // Number of OpenMP threads to use
    const int num_threads = 4;

    printf("\nPerformance Results (Time in seconds)\n");
    printf("=====================================\n");
    printf("Size\tSchedule\tChunk\tTime\n");

    char input_file[256], output_file[256];

    // Iterate over different image sizes
    for (int i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++)
    {
        sprintf(input_file, "input_%dx%d.png", sizes[i], sizes[i]);

        // Iterate over different scheduling strategies
        for (int j = 0; j < sizeof(schedules) / sizeof(schedules[0]); j++)
        {
            // Iterate over different chunk sizes
            for (int k = 0; k < sizeof(chunk_sizes) / sizeof(chunk_sizes[0]); k++)
            {
                sprintf(output_file, "output/output_%dx%d_%s_%d.png", sizes[i], sizes[i], schedules[j], chunk_sizes[k]);

                double time = process_image(input_file, output_file, num_threads, schedules[j], chunk_sizes[k]);

                printf("%dx%d\t%s\t	%d\t%.6f\n", sizes[i], sizes[i], schedules[j], chunk_sizes[k], time);
            }
        }
    }

    return 0;
}