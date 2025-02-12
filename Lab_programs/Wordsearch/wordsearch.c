#include <stdio.h>
#include <omp.h>
#include <string.h>

#define COUNT 10
#define FILE_NAME "words.txt"

char search_words[21][20] = {
    "student",
    "apple", "banana", "cherry", "date", "elderberry", "fig", "grape", "honeydew", "kiwi", "lemon",
    "mango", "nectarine", "orange", "pear", "plum", "peach", "pineapple", "quince", "raspberry", "strawberry"};

long counts[COUNT];

int line_c = 0;

int is_alpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_equal(char *a, const char *key, int ignore_case)
{
    char b[20];
    strcpy(b, key);
    int len_a = strlen(a);
    int len_b = strlen(b);
    if (len_a != len_b)
        return 0;

    if (ignore_case != 0)
    {
        for (int i = 0; i < len_a; i++)
        {
            if (a[i] > 90)
            {
                a[i] = a[i] - 32;
            }
            if (b[i] > 90)
            {
                b[i] = b[i] - 32;
            }
        }
    }

    return strcmp(a, b) == 0;
}

void read_word(char *temp, FILE *fp)
{
    int i = 0;
    int ch;

    // Skip non-alphabetic characters
    while ((ch = fgetc(fp)) != EOF && !is_alpha(ch))
        ;

    if (ch == EOF)
    {
        temp[0] = '\0';
        return;
    }

    // Read word
    temp[i++] = ch;
    while ((ch = fgetc(fp)) != EOF && is_alpha(ch) && i < 39)
    {
        temp[i++] = ch;
    }
    temp[i] = '\0';
}

long determine_count(const char *filename, const char *key, int ignore_case)
{
    int key_index = 0, key_len = strlen(key);
    long word_count = 0;
    char c;
    FILE *fp = fopen(filename, "r");
    char temp[40];
    int i = 0;

    while (feof(fp) == 0)
    {
        read_word(temp, fp);
        if (is_equal(temp, key, ignore_case))
            word_count++;
        // printf("%s\n", temp);
    }
    fclose(fp);
    // printf("Word count: %ld\n", word_count);
    return word_count;
}

int main()
{
    int thread_counts[] = {1, 2, 4, 8};
    double execution_times[4];
    long final_counts[COUNT];

    printf("\nParallel Word Search Program\n");
    printf("============================\n\n");

    printf("Timing Analysis:\n");
    printf("+---------------+----------------+\n");
    printf("|  Thread Count |  Time (sec)    |\n");
    printf("+---------------+----------------+\n");

    for (int t = 0; t < 4; t++)
    {
        // Reset counts for each thread configuration
        for (int i = 0; i < COUNT; i++)
            counts[i] = 0;

        double start_time = omp_get_wtime();
        omp_set_num_threads(thread_counts[t]);

#pragma omp parallel for
        for (int i = 0; i < COUNT; i++)
        {
            counts[i] = determine_count(FILE_NAME, search_words[i], 1);
        }

        double end_time = omp_get_wtime();
        execution_times[t] = end_time - start_time;

        // Save final counts from last run
        if (t == 3)
        {
            for (int i = 0; i < COUNT; i++)
                final_counts[i] = counts[i];
        }

        printf("| %13d | %12.6f |\n", thread_counts[t], execution_times[t]);
    }
    printf("+---------------+----------------+\n\n");

    // Display word counts once
    printf("Word Frequency Counts:\n");
    printf("+---------------+----------------+\n");
    printf("|     Word      |     Count     |\n");
    printf("+---------------+----------------+\n");
    for (int i = 0; i < COUNT; i++)
    {
        printf("| %-13s | %12ld |\n", search_words[i], final_counts[i]);
    }
    printf("+---------------+----------------+\n");

    return 0;
}