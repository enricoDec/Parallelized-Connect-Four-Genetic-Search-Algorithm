#include "includes/benchmark.h"

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

struct timespec start, end;

void benchmark_start(void)
{
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
}

void benchmark_end(void)
{
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
}

void benchmark_print(void)
{
    double time_taken;
    time_taken = (end.tv_sec - start.tv_sec) * 1000.0;
    time_taken += (end.tv_nsec - start.tv_nsec) / 1e6;
    printf("Time taken: %fms\n", time_taken);
    // wirte to file for plotting
    FILE *fp;
    fp = fopen("benchmark.txt", "a");
    fprintf(fp, "%fms\n", time_taken);
    fclose(fp);
}