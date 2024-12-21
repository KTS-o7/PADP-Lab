// We need to write a program in C for the Sieve of Eratosthenes algorithm to find all prime numbers less than or equal to a given integer n.
// need two versions one that is non cache friendly and one that is cache friendly

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

static inline long mark(bool composite[], long i, long step, long limit)
{
    for (long j = i; j <= limit; j += step)
    {
        composite[j] = true;
    }
    return i;
}

long cache_unfriendly_sieve(long n)
{
    long count = 0;
    bool *composite = (bool *)malloc((n + 1) * sizeof(bool));
    memset(composite, 0, (n + 1) * sizeof(bool));
    long limit = (long)sqrt(n);

    // Mark composites up to sqrt(n)
    for (long i = 2; i <= limit; i++)
    {
        if (!composite[i])
        {
            mark(composite, i * i, i, n);
        }
    }

    // Count all primes up to n
    for (long i = 2; i <= n; i++)
    {
        if (!composite[i])
        {
            count++;
        }
    }

    free(composite);
    return count;
}

long cache_friendly_sieve(long n)
{
    long limit = (long)sqrt(n);
    long count = 0;

    // Mark composites up to √n
    bool *small_composite = calloc(limit + 1, sizeof(bool));
    for (long i = 2; i * i <= limit; i++)
    {
        if (!small_composite[i])
        {
            for (long j = i * i; j <= limit; j += i)
            {
                small_composite[j] = true;
            }
        }
    }

    // Collect small primes
    long *primes = malloc((limit + 1) * sizeof(long));
    long prime_count = 0;
    for (long i = 2; i <= limit; i++)
    {
        if (!small_composite[i])
        {
            primes[prime_count] = i;
            count++;
            prime_count++;
        }
    }
    free(small_composite);

    for (long window_start = limit + 1; window_start <= n; window_start += limit)
    {
        long window_end = window_start + limit - 1;
        if (window_end > n)
            window_end = n;
        long segment_size = window_end - window_start + 1;

        bool *segment = calloc(segment_size, sizeof(bool));
        for (long i = 0; i < prime_count; i++)
        {
            long p = primes[i];
            long start_idx = (window_start + p - 1) / p * p;
            for (long j = start_idx; j <= window_end; j += p)
            {
                segment[j - window_start] = true;
            }
        }

        // Count primes in this segment
        for (long i = 0; i < segment_size; i++)
        {
            if (!segment[i])
                count++;
        }
        free(segment);
    }

    free(primes);
    return count;
}

long parallel_sieve(long n)
{
    long count = 0;
    long limit = (long)sqrt(n);
    long n_factor = 0;
    long *factor = (long *)malloc((limit) * sizeof(long));
    bool *composite = (bool *)malloc((limit + 1) * sizeof(bool));
    memset(composite, 0, (limit + 1) * sizeof(bool));

    // Sequential part to find initial prime factors
    for (long i = 2; i <= limit; i++)
    {
        if (!composite[i])
        {
            count++;
            mark(composite, i * i, i, limit);
            factor[n_factor++] = i;
        }
    }

#pragma omp parallel
    {
        bool *local_composite = (bool *)malloc((limit + 1) * sizeof(bool));
        long *marker = (long *)malloc(n_factor * sizeof(long));

#pragma omp for reduction(+ : count)
        for (long window = limit + 1; window <= n; window += limit)
        {
            memset(local_composite, 0, (limit + 1) * sizeof(bool));

            long window_end = window + limit - 1;
            if (window_end > n)
                window_end = n;

            // Calculate starting points for marking
            for (long k = 0; k < n_factor; k++)
            {
                long f = factor[k];
                marker[k] = (window + f - 1) / f * f - window;
            }

            // Mark composites
            for (long k = 0; k < n_factor; k++)
            {
                mark(local_composite, marker[k], factor[k], window_end - window);
            }

            // Count primes in this window
            for (long i = 0; i <= window_end - window; i++)
            {
                if (!local_composite[i])
                    count++;
            }
        }

        free(local_composite);
        free(marker);
    }

    free(composite);
    free(factor);
    return count;
}

int main()
{
    long input[3] = {1000000, 10000000, 100000000};

    printf("\nSieve of Eratosthenes - Prime Number Counting\n");
    printf("============================================\n\n");

    printf("+---------------+----------------------------+----------------------------+----------------------------+\n");
    printf("|   Input Size  |  Cache Unfriendly (sec)   |   Cache Friendly (sec)    |     Parallel (sec)        |\n");
    printf("+---------------+----------------------------+----------------------------+----------------------------+\n");

    for (int i = 0; i < 3; i++)
    {
        long n = input[i];
        double start, end, time1, time2, time3;

        start = omp_get_wtime();
        long result1 = cache_unfriendly_sieve(n);
        end = omp_get_wtime();
        time1 = end - start;

        start = omp_get_wtime();
        long result2 = cache_friendly_sieve(n);
        end = omp_get_wtime();
        time2 = end - start;

        start = omp_get_wtime();
        long result3 = parallel_sieve(n);
        end = omp_get_wtime();
        time3 = end - start;

        printf("| %11ldM | %12ld (%.6f s)      | %12ld (%.6f s)      | %12ld (%.6f s)      |\n",
               n / 1000000, result1, time1, result2, time2, result3, time3);
    }

    printf("+---------------+----------------------------+----------------------------+----------------------------+\n");
    return 0;
}
