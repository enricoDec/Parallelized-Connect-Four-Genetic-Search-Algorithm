#include "includes/geneticSearch.h"
// #include "includes/benchmark.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <omp.h>

#define BAD_FITNESS INT_MAX

int geneticSearch(Game_context game, GeneticSearchParameters geneticSearchParameters)
{
    int populationSize = geneticSearchParameters.populationSize;
    double crossoverRate = geneticSearchParameters.crossoverRate;
    double mutationRate = geneticSearchParameters.mutationRate;
    int maxGenerations = geneticSearchParameters.maxGenerations;
    int maxMoves = geneticSearchParameters.maxMoves;
    // benchmark_start();
    omp_set_num_threads(4);
    // Copy the game context to avoid modifying the original
    game = copyGameContext(&game);
    // Initialize the population
    Individual *population = malloc(populationSize * sizeof(Individual));
    #pragma omp parallel for
    for (int i = 0; i < populationSize; i++)
    {
        population[i] = createRandomIndividual(game, maxMoves);
    }
    // Evaluate the fitness of the initial population
    #pragma omp parallel for
    for (int i = 0; i < populationSize; i++)
    {
        evaluateFitness(&population[i], maxMoves);
    }
    // Perform the genetic search algorithm
    int generation = 0;
    while (generation < maxGenerations)
    {
        // Select parents using tournament selection
        Individual *parents = malloc(populationSize * sizeof(Individual));
        #pragma omp parallel for
        for (int i = 0; i < populationSize; i++)
        {
            parents[i] = tournamentSelection(population, populationSize);
        }
        // Perform crossover for each parent to create offspring
        Individual *offspring = malloc(populationSize * sizeof(Individual));
        #pragma omp parallel for
        for (int i = 0; i < populationSize; i++)
        {
            offspring[i] = crossover(parents[i], parents[(i + 1) % populationSize], game, crossoverRate);
        }
        // Mutate the offspring
        #pragma omp parallel for
        for (int i = 0; i < populationSize; i++)
        {
            mutate(&offspring[i], game.boardCols, mutationRate);
        }
        // Reinsert the best individual from the previous generation
        reinsertion(offspring, population, game, maxMoves);
        // Evaluate the fitness of the offspring
        #pragma omp parallel for
        for (int i = 0; i < populationSize; i++)
        {
            evaluateFitness(&offspring[i], maxMoves);
        }
        for (int i = 0; i < populationSize; i++)
        {
            // freeBoard(&population[i].game);
            // free(population[i].moves);
        }
        free(population);
        // Replace the population with the new one
        population = offspring;
        free(parents);
        generation++;
    }
    // Find the best next move based on the fittest individual
    Individual bestIndividual = getBestIndividual(population, populationSize);
    // Retrieve the best next move
    int bestMove = bestIndividual.moves[0];
    // printf("Best move: %d with fitness: %d\n", bestMove, bestIndividual.fitness);
    // printBoard(bestIndividual.game);
    //   Cleanup
    freeBoard(&game);
    free(population);
    // benchmark_end();
    // benchmark_print();
    return bestMove;
}

Individual tournamentSelection(Individual *population, int populationSize)
{
    int parent1Index = rand() % populationSize;
    int parent2Index = rand() % populationSize;
    // Pick the best individual as parent (the one closer to 0)
    Individual parent = bestFitness(population[parent1Index].fitness, population[parent2Index].fitness) == population[parent1Index].fitness ? population[parent1Index] : population[parent2Index];
    return parent;
}

Individual crossover(Individual parent1, Individual parent2, Game_context game, double crossoverRate)
{
    Individual offspring;
    int boardCols = game.boardCols;
    offspring.moves = malloc(boardCols * sizeof(int));
    for (int i = 0; i < boardCols; i++)
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

void mutate(Individual *individual, int boardCols, double mutationRate)
{
    for (int i = 0; i < boardCols; i++)
    {
        if ((double)rand() / RAND_MAX < mutationRate)
        {
            individual->moves[i] = rand() % boardCols;
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

    newPopulation[0] = best;
}

void evaluateFitness(Individual *individual, int maxMoves)
{
    Game_context game = individual->game;
    int movesToWin = 0;

    for (int i = 0; i < maxMoves; i++)
    {
        int move = individual->moves[i];
        bool isMoveValid = doMove(&game, move, PC);
        int result = checkWin(game);

        // If move is invalid or there are no more valid moves, terminate with the worst possible fitness
        if (!isMoveValid || isBoardFull(game))
        {
            individual->fitness = BAD_FITNESS;
            setRemainingMovesToEmpty(individual, i + 1, maxMoves);
            return;
        }

        // If the move results in a win, terminate with the corresponding fitness (moves to win)
        if (result == PC)
        {
            individual->fitness = movesToWin;
            setRemainingMovesToEmpty(individual, i + 1, maxMoves);
            return;
        }

        // Apply the player's move
        int playerMove = getRandomValidMove(&game); // TODO: this promotes individuals who randomly got a bad player move and won. Maybe we should use a different strategy. We should try to always pick a good move for the player (maybe run genSearch for next player move)
        // If there are no more valid moves for the player (-1), it means the game is a draw. Just ignore.
        if (playerMove != -1)
        {
            doMove(&game, playerMove, PLAYER);
        }

        // Update the fitness based on the distance to win
        movesToWin++;
    }

    // If all moves are exhausted, terminate with the worst possible fitness
    individual->fitness = BAD_FITNESS;
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
    // Create random moves
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