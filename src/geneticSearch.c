#include "includes/geneticSearch.h"
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <omp.h>

#define BAD_FITNESS INT_MAX

int geneticSearch(Game_context game, GeneticSearchParameters geneticSearchParameters)
{
    int populationSize = geneticSearchParameters.populationSize;
    double crossoverRate = geneticSearchParameters.crossoverRate;
    double mutationRate = geneticSearchParameters.mutationRate;
    int maxGenerations = geneticSearchParameters.maxGenerations;
    int maxMoves = geneticSearchParameters.maxMoves;
    game = copyGameContext(&game);
    Individual *population = malloc(populationSize * sizeof(Individual));
    for (int i = 0; i < populationSize; i++)
    {
        population[i] = createRandomIndividual(game, maxMoves);
    }
    multithreadedEvaluateFitness(population, populationSize, maxMoves);
    // for (int i = 0; i < populationSize; i++)
    // {
    //     int fitness = evaluateFitness(&population[i], maxMoves);
    //     #pragma omp critical
    //     {
    //         population[i].fitness = fitness;
    //     }
    // }
    int generation = 0;
    while (generation < maxGenerations)
    {
        Individual *parents = malloc(populationSize * sizeof(Individual));
        for (int i = 0; i < populationSize; i++)
        {
            parents[i] = tournamentSelection(population, populationSize);
        }
        Individual *offspring = malloc(populationSize * sizeof(Individual));
        for (int i = 0; i < populationSize; i++)
        {
            offspring[i] = crossover(parents[i], parents[(i + 1) % populationSize], game, crossoverRate, maxMoves);
        }
        for (int i = 0; i < populationSize; i++)
        {
            mutate(&offspring[i], game.boardCols, mutationRate, maxMoves);
        }
        reinsertion(offspring, population, game, maxMoves);
        multithreadedEvaluateFitness(offspring, populationSize, maxMoves);
        // #pragma omp parallel for num_threads(4)
        // for (int i = 0; i < populationSize; i++)
        // {
        //     int fitness = evaluateFitness(&offspring[i], maxMoves);
        //     #pragma omp critical
        //     {
        //         offspring[i].fitness = fitness;
        //     }
        // }
        for (int i = 0; i < populationSize; i++)
        {
            freeBoard(&population[i].game);
            free(population[i].moves);
        }
        free(population);
        population = offspring;
        free(parents);
        generation++;
    }
    Individual bestIndividual = getBestIndividual(population, populationSize);
    int bestMove = bestIndividual.moves[0];
    freeBoard(&game);
    free(population);
    return bestMove;
}

Individual tournamentSelection(Individual *population, int populationSize)
{
    int parent1Index = rand() % populationSize;
    int parent2Index = rand() % populationSize;
    Individual parent = bestFitness(population[parent1Index].fitness, population[parent2Index].fitness) == population[parent1Index].fitness ? population[parent1Index] : population[parent2Index];
    return parent;
}

Individual crossover(Individual parent1, Individual parent2, Game_context game, double crossoverRate, int maxMoves)
{
    Individual offspring;
    offspring.moves = malloc(maxMoves * sizeof(int));
    for (int i = 0; i < maxMoves; i++)
    {
        if ((double)rand() / RAND_MAX < crossoverRate)
        {
            offspring.moves[i] = parent1.moves[i];
        }
        else
        {
            offspring.moves[i] = parent2.moves[i];
        }
    }
    offspring.game = copyGameContext(&game);
    offspring.fitness = BAD_FITNESS;
    return offspring;
}

void mutate(Individual *individual, int boardCols, double mutationRate, int maxMoves)
{
    for (int i = 0; i < maxMoves; i++)
    {
        if ((double)rand() / RAND_MAX < mutationRate)
        {
            individual->moves[i] = arc4random() % boardCols;
        }
    }
}

void reinsertion(Individual *newPopulation, Individual *oldPopulation, Game_context game, int maxMoves)
{
    Individual best = getBestIndividual(oldPopulation, maxMoves);
    Individual copyBest;
    copyBest.moves = malloc(maxMoves * sizeof(int));
    for (int i = 0; i < maxMoves; i++)
    {
        copyBest.moves[i] = best.moves[i];
    }
    copyBest.fitness = BAD_FITNESS;
    copyBest.game = copyGameContext(&game);

    newPopulation[0] = copyBest;
}

void evaluateFitnessSubset(Individual *population, int startIdx, int endIdx, int maxMoves) {
    for (int i = startIdx; i < endIdx; i++) {
        population[i].fitness = evaluateFitness(&population[i], maxMoves);
    }
}

void multithreadedEvaluateFitness(Individual *population, int populationSize, int maxMoves) {
    #pragma omp parallel
    {
        int numThreads = omp_get_num_threads();
        int chunkSize = populationSize / numThreads;
        int threadID = omp_get_thread_num();
        int startIdx = threadID * chunkSize;
        int endIdx = (threadID == numThreads - 1) ? populationSize : (threadID + 1) * chunkSize;
        evaluateFitnessSubset(population, startIdx, endIdx, maxMoves);
    }
}

int evaluateFitness(Individual *individual, int maxMoves)
{
    Game_context game = individual->game;
    int movesToWin = 0;
    int fitness = BAD_FITNESS;
    bool playerWin = false;

    for (int i = 0; i < maxMoves; i++)
    {
        int move = individual->moves[i];
        bool isMoveValid = doMove(&game, move, PC);
        int result;
        result = checkWin(game);
        if (result == PC)
        {
            return movesToWin; // PC won, return the move count
        }
        if (!isMoveValid || isBoardFull(game))
        {
            return BAD_FITNESS; // Invalid move or draw, return BAD_FITNESS
        }
        int opponentMove = getRandomValidMove(&game);
        if (opponentMove != -1)
        {
            doMove(&game, opponentMove, PLAYER);
        }
        // Check if the opponent's move resulted in a win
        result = checkWin(game);
        if (result == PLAYER)
        {
            playerWin = true; // Player won
            fitness = movesToWin; // Update fitness
        }
        else if (result == PC)
        {
            return movesToWin; // PC won, return the move count
        }
        movesToWin++;
    }
    if (playerWin)
    {
        return fitness; // Player win, return fitness
    }
    else
    {
        return maxMoves; // Draw or loss, return maxMoves (worst fitness)
    }
}

Individual getBestIndividual(Individual *population, int populationSize)
{
    Individual best = population[0];
    for (int i = 1; i < populationSize; i++)
    {
        if (population[i].fitness == bestFitness(population[i].fitness, best.fitness))
        {
            best = population[i];
        }
    }
    return best;
}

Individual createRandomIndividual(Game_context game, int maxMoves)
{
    Individual individual;
    individual.moves = malloc(maxMoves * sizeof(int));
    individual.fitness = BAD_FITNESS;
    individual.game = copyGameContext(&game);
    for (int i = 0; i < maxMoves; i++)
    {
        individual.moves[i] = rand() % game.boardCols;
    }
    return individual;
}

void setRemainingMovesToEmpty(Individual *individual, int startIndex, int maxMoves)
{
    if (startIndex < 0 || startIndex > maxMoves)
    {
        return;
    }
    for (int i = startIndex; i < maxMoves; i++)
    {
        individual->moves[i] = EMPTY;
    }
}

int bestFitness(int fitnessA, int fitnessB)
{
    int absA = abs(fitnessA);
    int absB = abs(fitnessB);

    if (absA < absB)
    {
        return fitnessA;
    }
    else
    {
        return fitnessB;
    }
}

int worstFitness(int fitnessA, int fitnessB)
{
    int absA = abs(fitnessA);
    int absB = abs(fitnessB);

    if (absA > absB)
    {
        return fitnessA;
    }
    else
    {
        return fitnessB;
    }
}

void printPopulation(Individual *population, int populationSize, int maxMoves)
{
    // find best 10 individuals in population and print them (moves and fitness)
    Individual bestIndividuals[10];
    for (int i = 0; i < 10; i++)
    {
        bestIndividuals[i] = population[i];
    }
    for (int i = 10; i < populationSize; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (population[i].fitness < bestIndividuals[j].fitness)
            {
                bestIndividuals[j] = population[i];
                break;
            }
        }
    }
    for (int i = 0; i < 10; i++)
    {
        printf("Individual %d:\n", i);
        for (int j = 0; j < maxMoves; j++)
        {
            printf("%d ", bestIndividuals[i].moves[j]);
        }
        printf("\n");
        printf("Fitness: %d\n", bestIndividuals[i].fitness);
    }
}