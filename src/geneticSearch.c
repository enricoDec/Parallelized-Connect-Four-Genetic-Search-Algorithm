#include "includes/geneticSearch.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <omp.h>

#define POPULATION_SIZE 1000
#define TOURNAMENT_SIZE 2
#define CROSSOVER_RATE 0.8
#define MUTATION_RATE 0.1
#define MAX_GENERATIONS 1000
#define MAX_MOVES 4
#define BAD_FITNESS INT_MAX

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

void evaluateFitness(Individual *individual)
{
    for (int i = 0; i < MAX_MOVES; i++)
    {
        Game_context game = individual->game;
        bool isMoveValid = doMove(&game, individual->moves[i], PC);
        int result = checkWin(game);

        // if move is invalid, fitness = BAD_FITNESS
        if (!isMoveValid)
        {
            individual->fitness = BAD_FITNESS; // TODO: board could be full so dont penalize for that, since game over
            break;
        }

        // a draw should be better than a loss
        if (result == PLAYER)
        {
            individual->fitness = BAD_FITNESS;
            break;
        }
        else if (result == PC)
        {
            individual->fitness = i == 0 ? 0 : -i;
            break;
        }
        else if (result == 3) // draw
        {
            individual->fitness--;
            break;
        }
        else if (result == 0)
        {
            // if the move is valid but no end yet, decrease fitness reflecting distance to win
            individual->fitness--;
        }

        // Apply the player's move, try all the columns and choose one that leads to player win or if none wins choose a valid move
        if (!isBoardFull(game))
        {
            int playerWinMove = -1;
            int lastValidMove = -1; // TODO: we always take the last player move like this if no win move is found
            for (int j = 0; j < game.boardCols; j++)
            {
                bool isMoveValid = doMove(&game, j, PLAYER);
                int result = checkWin(game);
                undoMove(&game, j, PLAYER);
                if(result == PLAYER) {
                    playerWinMove = j;
                    break;
                } else if(isMoveValid) {
                    lastValidMove = j;
                }
            }
            if(playerWinMove != -1) {
                doMove(&game, playerWinMove, PLAYER);
            } else {
                // since board is not full, there must be a valid move
                doMove(&game, lastValidMove, PLAYER);
            }
        }
        result = checkWin(game);
        // Check if the game is finished
        if (result != 0)
        {
            // if the player wins, fitness = BAD_FITNESS
            if (result == PLAYER)
            {
                individual->fitness = BAD_FITNESS;
                break;
            }
            // if the PC wins, fitness = 0
            else if (result == PC)
            {
                individual->fitness = 0;
                break;
            }
            // if it's a draw, fitness = BAD_FITNESS / 2
            else if (result == 3)
            {
                individual->fitness--;
                break;
            }
        }
    }
}

Individual *tournamentSelection(Individual *population)
{
    Individual *parents = malloc(2 * sizeof(Individual));
    for (int i = 0; i < 2; i++)
    {
        // Randomly select individuals for the tournament
        Individual *tournament = malloc(TOURNAMENT_SIZE * sizeof(Individual));
        for (int j = 0; j < TOURNAMENT_SIZE; j++)
        {
            int index = rand() % POPULATION_SIZE;
            tournament[j] = population[index];
        }
        // Select the fittest individual from the tournament, so the one closer to fittness == 0 (fitness can be negative)
        Individual fittest = tournament[0];
        for (int j = 1; j < TOURNAMENT_SIZE; j++)
        {
            // TODO: this logic a bit flawed because it will always select the first individual if there are multiple with the same fitness
            if (tournament[j].fitness == bestFitness(tournament[j].fitness, fittest.fitness))
            {
                fittest = tournament[j];
            }
        }
        parents[i] = fittest;
    }
    return parents;
}

Individual crossover(Individual parent1, Individual parent2, int boardCols)
{
    Individual offspring;
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
    offspring.game = parent1.game;
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

void reinsertion(Individual *population, Individual offspring)
{
    // Find the worst individual in the population
    int worstIndex = 0;
    Individual worst = population[worstIndex];
    for (int i = 1; i < POPULATION_SIZE; i++)
    {
        if (population[i].fitness == worstFitness(population[i].fitness, worst.fitness))
        {
            worst = population[i];
            worstIndex = i;
        }
    }
    // Replace the worst individual with the offspring
    population[worstIndex] = offspring;
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

int geneticSearch(Game_context game)
{
    // Initialize the population
    game = copyGameContext(&game);
    Individual *population = malloc(POPULATION_SIZE * sizeof(Individual));
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        population[i] = createRandomIndividual(game);
    }
    // Evaluate the fitness of the initial population
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        evaluateFitness(&population[i]);
    }
    // Perform the genetic search algorithm
    int generation = 0;
    while (generation < MAX_GENERATIONS)
    {
        // Select parents using tournament selection
        Individual *parents = tournamentSelection(population);
        // Perform crossover to create offspring
        Individual offspring = crossover(parents[0], parents[1], game.boardCols);
        // Mutate the offspring
        mutate(&offspring, game.boardCols);
        // Evaluate the fitness of the offspring
        evaluateFitness(&offspring);
        // Perform reinsertion of the offspring into the population (replace the worst individual)
        // reinsertion(population, offspring);
        generation++;
    }
    // Find the best next move based on the fittest individual
    Individual bestIndividual = getBestIndividual(population);
    // Retrieve the best next move
    int bestMove = bestIndividual.moves[0];
    printf("Best move: %d with fitness: %d\n", bestMove, bestIndividual.fitness);
    printBoard(bestIndividual.game);
    // Cleanup
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        freeBoard(&population[i].game);
        free(population[i].moves);
    }
    free(population);
    freeBoard(&game);
    return bestMove;
}
