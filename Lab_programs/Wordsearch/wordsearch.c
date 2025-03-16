#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <omp.h>

#define COUNT 10
#define FILE_NAME "words.txt"
#define MAX_WORD_LEN 100

char search_words[COUNT][20] = {"apple", "banana", "cherry", "date", "elderberry", "fig", "grape", "honeydew", "kiwi", "lemon"};

void to_lower(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }
}

long get_word_count(const char *filename, const char *search_word)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 0;
    }

    char word[MAX_WORD_LEN];
    char lower_search[MAX_WORD_LEN];
    long count = 0;

    // Convert search word to lowercase once
    strcpy(lower_search, search_word);
    to_lower(lower_search);

    // Read words directly
    while (fscanf(fp, "%99s", word) == 1)
    {
        to_lower(word);
        if (strcmp(word, lower_search) == 0)
        {
            count++;
        }
    }

    fclose(fp);
    return count;
}

int main()
{
    int thread_counts[] = {1, 2, 4, 8};
    long counts[COUNT];

    printf("\nParallel Word Search Program\n");
    printf("============================\n\n");

    for (int t = 0; t < 4; t++)
    {
        double start_time = omp_get_wtime();
        omp_set_num_threads(thread_counts[t]);

#pragma omp parallel for
        for (int i = 0; i < COUNT; i++)
        {
            counts[i] = get_word_count(FILE_NAME, search_words[i]);
        }

        double end_time = omp_get_wtime();
        printf("Threads: %d, Time: %.6f seconds\n",
               thread_counts[t], end_time - start_time);
    }
    for (int i = 0; i < COUNT; i++)
    {
        printf("Word: %s, Count: %ld\n", search_words[i], counts[i]);
    }

    return 0;
}