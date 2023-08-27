#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/argparse.h"
#include "includes/game.h"
#include "includes/gameUI.h"
#include "includes/geneticSearch.h"
#include "includes/benchmark.h"

static const char *const usages[] = {
    "connect4 [options] [[--] args]",
    "connect4 [options]",
    NULL,
};

int main(int argc, const char **argv)
{
    int rows = 7;
    int cols = 6;
    int populationSize = 1000;
    double crossoverRate = 0.8;
    double mutationRate = 0.01;
    int maxGenerations = 100;
    int maxMoves = 3;
    bool benchmark = false;
    bool version = false;
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("Basic options"),
        OPT_BOOLEAN('b', "benchmark", &benchmark, "benchmark", NULL, 0, 0),
        OPT_BOOLEAN('v', "version", &version, "version", NULL, 0, 0),
        OPT_GROUP("Genetic search options"),
        OPT_INTEGER('r', "rows", &rows, "number of rows", NULL, 0, 0),
        OPT_INTEGER('c', "cols", &cols, "number of cols", NULL, 0, 0),
        OPT_INTEGER('m', "maxMoves", &maxMoves, "max moves", NULL, 0, 0),
        OPT_INTEGER('p', "populationSize", &populationSize, "population size", NULL, 0, 0),
        OPT_FLOAT('x', "crossoverRate", &crossoverRate, "crossover rate", NULL, 0, 0),
        OPT_FLOAT('u', "mutationRate", &mutationRate, "mutation rate", NULL, 0, 0),
        OPT_INTEGER('g', "maxGenerations", &maxGenerations, "max generations", NULL, 0, 0),
        OPT_END(),
    };
    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\n Connect 4", "\n Connect 4");
    argparse_parse(&argparse, argc, argv);
    if (version)
    {
        printf("Connect 4 version 1.0.0\n");
        return 0;
    }
    if (rows < 1)
    {
        printf("Number of rows must be higher then 0!\n");
        return 1;
    }
    if (cols < 1)
    {
        printf("Number of cols must be higher then 0!\n");
        return 1;
    }
    if (maxMoves < 1)
    {
        printf("Number of moves must be higher then 0!\n");
        return 1;
    }
    if (populationSize < 1)
    {
        printf("Population size must be higher then 0!\n");
        return 1;
    }
    if (crossoverRate < 0 || crossoverRate > 1)
    {
        printf("Crossover rate must be between 0 and 1!\n");
        return 1;
    }
    if (mutationRate < 0 || mutationRate > 1)
    {
        printf("Mutation rate must be between 0 and 1!\n");
        return 1;
    }
    if (maxGenerations < 1)
    {
        printf("Max generations must be higher then 0!\n");
        return 1;
    }
    StartingPlayer startingPlayer = RANDOM;
    GeneticSearchParameters geneticSearchParameters =
        {populationSize,
         crossoverRate,
         mutationRate,
         maxGenerations,
         maxMoves};
    if (benchmark)
    {
        printf("Benchmarking...\n");
        int numGames = 1000;
        for (int i = 0; i < numGames; i++)
        {
            double avgGenSearchTime = 0;
            int turns = 0;
            Game_context game = initGame(rows, cols, COMPUTER);
            while (checkWin(game) == 0)
            {
                turns++;
                // do pc move
                benchmark_start();
                int pcMove = geneticSearch(game, geneticSearchParameters);
                benchmark_end();
                double timeTaken = get_time_taken();
                printf("(%d/%d) - Time taken: %fms\n", i, numGames, timeTaken);
                avgGenSearchTime += timeTaken;
                bool isValid = doMove(&game, pcMove, PC);
                if (!isValid)
                {
                    printf("PC took invalid move!\n");
                    break;
                }
                if (checkWin(game) != 0)
                {
                    avgGenSearchTime /= turns;
                    break;
                }
                // do user move (random)
                int playerMove = getRandomValidMove(&game);
                if (playerMove == -1)
                {
                    printf("No valid moves available!\n");
                    break;
                }
                isValid = doMove(&game, pcMove, PLAYER);
                if (!isValid)
                {
                    printf("User took invalid move!\n");
                    break;
                }
                if (checkWin(game) != 0)
                {
                    avgGenSearchTime /= turns;
                    break;
                }
            }
            int pcWon = checkWin(game) == COMPUTER;
            char *message = NULL;
            int uuid = rand();
            int threads = 1;
            asprintf(&message, "%d,%d,%d,%d,%d,%f,%f,%d,%d,%f,%d",
                     uuid,
                     threads,
                     rows,
                     cols,
                     populationSize,
                     crossoverRate,
                     mutationRate,
                     maxGenerations,
                     maxMoves,
                     avgGenSearchTime,
                     pcWon);
            benchmark_printCol(message);
            freeBoard(&game);
            free(message);
        }
    }
    else
    {
        initUI(800, 450);
        Game_context game = initGame(rows, cols, startingPlayer);
        loop(&game);
        freeBoard(&game);
    }
    return 0;
}
