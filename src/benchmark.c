#include "includes/benchmark.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

struct timeval start, end;

void benchmark_start(void)
{
    gettimeofday(&start, NULL);
}

void benchmark_end(void)
{
    gettimeofday(&end, NULL);
}

void benchmark_print(void)
{
    double time_taken;
    time_taken = (end.tv_sec - start.tv_sec) * 1000.0;
    time_taken += (end.tv_usec - start.tv_usec) / 1000.0;
    printf("Time taken: %fms\n", time_taken);
    // write to file for plotting
    FILE *fp;
    fp = fopen("benchmark.txt", "a");
    fprintf(fp, "%fms\n", time_taken);
    fclose(fp);
}