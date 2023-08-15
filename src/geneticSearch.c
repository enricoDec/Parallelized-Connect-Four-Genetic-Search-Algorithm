#include "includes/geneticSearch.h"
// #include "includes/benchmark.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <omp.h>

#define POPULATION_SIZE 1000
#define CROSSOVER_RATE 0.8
#define MUTATION_RATE 0.1
#define MAX_GENERATIONS 100
#define MAX_MOVES 3
#define BAD_FITNESS INT_MAX

int geneticSearch(Game_context game)
{
    // benchmark_start();
    omp_set_num_threads(4);
    // Copy the game context to avoid modifying the original
    game = copyGameContext(&game);
    // Initialize the population
    Individual *population = malloc(POPULATION_SIZE * sizeof(Individual));
    # pragma omp parallel for
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        population[i] = createRandomIndividual(game);
    }
    // Evaluate the fitness of the initial population
    #pragma omp parallel for
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        evaluateFitness(&population[i]);
    }
    // Perform the genetic search algorithm
    int generation = 0;
    while (generation < MAX_GENERATIONS)
    {
        // Select parents using tournament selection
        Individual *parents = malloc(POPULATION_SIZE * sizeof(Individual));
        #pragma omp parallel for
        for (int i = 0; i < POPULATION_SIZE; i++)
        {
            parents[i] = tournamentSelection(population);
        }
        // Perform crossover for each parent to create offspring
        Individual *offspring = malloc(POPULATION_SIZE * sizeof(Individual));
        #pragma omp parallel for
        for (int i = 0; i < POPULATION_SIZE; i++)
        {
            offspring[i] = crossover(parents[i], parents[(i + 1) % POPULATION_SIZE], game);
        }
        // Mutate the offspring
        #pragma omp parallel for
        for (int i = 0; i < POPULATION_SIZE; i++)
        {
            mutate(&offspring[i], game.boardCols);
        }
        // Reinsert the best individual from the previous generation
        reinsertion(offspring, population, game);
        // Evaluate the fitness of the offspring
        #pragma omp parallel for
        for (int i = 0; i < POPULATION_SIZE; i++)
        {
            evaluateFitness(&offspring[i]);
        }
        for (int i = 0; i < POPULATION_SIZE; i++)
        {
            //freeBoard(&population[i].game);
            //free(population[i].moves);
        }
        free(population);
        // Replace the population with the new one
        population = offspring;
        free(parents);
        generation++;
    }
    // Find the best next move based on the fittest individual
    Individual bestIndividual = getBestIndividual(population);
    // Retrieve the best next move
    int bestMove = bestIndividual.moves[0];
    //printf("Best move: %d with fitness: %d\n", bestMove, bestIndividual.fitness);
    //printBoard(bestIndividual.game);
    //  Cleanup
    freeBoard(&game);
    free(population);
    // benchmark_end();
    // benchmark_print();
    return bestMove;
}

Individual tournamentSelection(Individual *population)
{
    int parent1Index = rand() % POPULATION_SIZE;
    int parent2Index = rand() % POPULATION_SIZE;
    // Pick the best individual as parent (the one closer to 0)
    Individual parent = bestFitness(population[parent1Index].fitness, population[parent2Index].fitness) == population[parent1Index].fitness ? population[parent1Index] : population[parent2Index];
    return parent;
}

Individual crossover(Individual parent1, Individual parent2, Game_context game)
{
    Individual offspring;
    int boardCols = game.boardCols;
    offspring.moves = malloc(boardCols * sizeof(int));
    for (int i = 0; i < boardCols; i++)
    {
        if ((double)rand() / RAND_MAX < CROSSOVER_RATE)
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

void mutate(Individual *individual, int boardCols)
{
    for (int i = 0; i < boardCols; i++)
    {
        if ((double)rand() / RAND_MAX < MUTATION_RATE)
        {
            individual->moves[i] = rand() % boardCols;
        }
    }
}

void reinsertion(Individual *newPopulation, Individual *oldPopulation, Game_context game)
{
    Individual best = getBestIndividual(oldPopulation);
    Individual copyBest;
    copyBest.moves = malloc(MAX_MOVES * sizeof(int));
    for (int i = 0; i < MAX_MOVES; i++)
    {
        copyBest.moves[i] = best.moves[i];
    }
    copyBest.fitness = BAD_FITNESS;
    copyBest.game = copyGameContext(&game);

    newPopulation[0] = best;
}

void evaluateFitness(Individual *individual)
{
    Game_context game = individual->game;
    int movesToWin = 0;

    for (int i = 0; i < MAX_MOVES; i++)
    {
        int move = individual->moves[i];
        bool isMoveValid = doMove(&game, move, PC);
        int result = checkWin(game);

        // If move is invalid or there are no more valid moves, terminate with the worst possible fitness
        if (!isMoveValid || isBoardFull(game))
        {
            individual->fitness = BAD_FITNESS;
            setRemainingMovesToEmpty(individual, i + 1);
            return;
        }

        // If the move results in a win, terminate with the corresponding fitness (moves to win)
        if (result == PC)
        {
            individual->fitness = movesToWin;
            setRemainingMovesToEmpty(individual, i + 1);
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

Individual getBestIndividual(Individual *population)
{
    Individual best = population[0];
    for (int i = 1; i < POPULATION_SIZE; i++)
    {
        if (population[i].fitness == bestFitness(population[i].fitness, best.fitness))
        {
            best = population[i];
        }
    }
    return best;
}

Individual createRandomIndividual(Game_context game)
{
    Individual individual;
    individual.moves = malloc(MAX_MOVES * sizeof(int));
    individual.fitness = BAD_FITNESS;
    individual.game = copyGameContext(&game);
    // Create random moves
    for (int i = 0; i < MAX_MOVES; i++)
    {
        individual.moves[i] = rand() % game.boardCols;
    }
    return individual;
}

void setRemainingMovesToEmpty(Individual *individual, int startIndex)
{
    if (startIndex < 0 || startIndex > MAX_MOVES)
    {
        return;
    }
    for (int i = startIndex; i < MAX_MOVES; i++)
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