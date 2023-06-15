#include "includes/benchmark.h"

#include <stdio.h>
#include <stdlib.h>

static long long start_time;
static long long end_time;

char *millis_to_time(long long millis)
{
    // Convert milliseconds to seconds and millis as formatted string
    int seconds = (millis % 60000) / 1000;
    int milliseconds = millis % 1000;
    char *time = malloc(10 * sizeof(char));
    sprintf(time, "%d.%dms", seconds, milliseconds);
    return time;
}

void benchmark_start()
{
    start_time = get_time();
}

void benchmark_end()
{
    end_time = get_time();
}

void benchmark_print()
{
    long long elapsed_time = end_time - start_time;
    char *time = millis_to_time(elapsed_time);
    // write to file the time
    FILE *file = fopen("benchmark.txt", "a");
    fprintf(file, "%s\n", time);
    fclose(file);
    free(time);
}