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

double get_time_taken(void)
{
    double time_taken;
    time_taken = (end.tv_sec - start.tv_sec) * 1000.0;
    time_taken += (end.tv_usec - start.tv_usec) / 1000.0;
    return time_taken;
}

void benchmark_printCol(char *message)
{
    FILE *fp;
    fp = fopen("benchmark.csv", "a");
    if (fp == NULL)
    {
        perror("Error opening file");
        return;
    }
    if (ftell(fp) == 0)
    {
        fprintf(fp, "UUID,threads,Rows,Columns,Population size,crossover rate,mutation rate,max number of generations,max number of moves (genes),average search time (turn), game status\n");
    }
    if (message != NULL)
    {
        fprintf(fp, "%s\n", message);
    }
    fclose(fp);
    free(message);
}
