#include "includes/geneticSearch.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define POPULATION_SIZE 500
#define TOURNAMENT_SIZE 2
#define CROSSOVER_RATE 0.8
#define MUTATION_RATE 0.1
#define MAX_GENERATIONS 500
#define MAX_MOVES 1
#define BAD_FITNESS INT_MAX

Individual createRandomIndividual(Game_context game)
{
    Individual individual;
    individual.moves = malloc(MAX_MOVES * sizeof(int));
    individual.fitness = BAD_FITNESS;
    // Create random moves
    for (int i = 0; i < MAX_MOVES; i++)
    {
        individual.moves[i] = rand() % game.boardCols;
    }
    return individual;
}

void evaluateFitness(Individual *individual, Game_context game)
{
    // Apply the moves of the individual to the game
    for (int i = 0; i < MAX_MOVES; i++)
    {
        bool isMoveValid = doMove(&game, individual->moves[i], PC);
        int result = checkWin(game);

        // if move is invalid, fitness = BAD_FITNESS
        if (!isMoveValid)
        {
            individual->fitness = BAD_FITNESS;
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

        // Apply the player's move (random)
        if (!isBoardFull(game))
        {
            int playerMove = rand() % game.boardCols;
            bool isValid = doMove(&game, playerMove, PLAYER);

            // if move is invalid, generate a new random move (will not loop forever because the board is not full)
            while (!isValid)
            {
                // undo the move
                undoMove(&game, playerMove, PLAYER);
                playerMove = rand() % game.boardCols;
                isValid = doMove(&game, playerMove, PLAYER);
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
        // Select the fittest individual from the tournament, so the one closer to 0 (fitness can be negative)
        Individual fittest = tournament[0];
        for (int j = 1; j < TOURNAMENT_SIZE; j++)
        {
            if (tournament[j].fitness < fittest.fitness)
            {
                fittest = tournament[j];
            }
        }
        parents[i] = fittest;
        free(tournament);
    }
    return parents;
}

Individual crossover(Individual parent1, Individual parent2, Game_context game)
{
    Individual offspring;
    offspring.moves = malloc(game.boardCols * sizeof(int));

    for (int i = 0; i < game.boardCols; i++)
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

    return offspring;
}

void mutate(Individual *individual, Game_context game)
{
    for (int i = 0; i < game.boardCols; i++)
    {
        if ((double)rand() / RAND_MAX < MUTATION_RATE)
        {
            individual->moves[i] = rand() % game.boardCols;
        }
    }
}

void reinsertion(Individual *population, Individual offspring)
{
    // Find the worst individual in the population
    Individual worst = population[0];
    int worstIndex = 0;
    for (int i = 1; i < POPULATION_SIZE; i++)
    {
        if (population[i].fitness > worst.fitness)
        {
            worst = population[i];
            worstIndex = i;
        }
    }
    // Replace the worst individual with the offspring
    population[worstIndex] = offspring;
}

int geneticSearch(Game_context game)
{
    // TODO: copy the game context to avoid modifying the original game in each function
    // each individual should have a copy of the game board
    // Initialize the population
    Individual *population = malloc(POPULATION_SIZE * sizeof(Individual));
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        population[i] = createRandomIndividual(game);
    }
    // Evaluate the fitness of the initial population
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        evaluateFitness(&population[i], game);
    }
    // Perform the genetic search algorithm
    int generation = 0;
    while (generation < MAX_GENERATIONS)
    {
        // Select parents using tournament selection
        Individual *parents = tournamentSelection(population);
        // Perform crossover to create offspring
        Individual offspring = crossover(parents[0], parents[1], game);
        // Mutate the offspring
        mutate(&offspring, game);
        // Evaluate the fitness of the offspring
        evaluateFitness(&offspring, game);
        // Perform reinsertion of the offspring into the population
        reinsertion(population, offspring);
        generation++;
    }
    // Find the best next move based on the fittest individual
    Individual bestIndividual = population[0];
    for (int i = 1; i < POPULATION_SIZE; i++)
    {
        // The fittest individual is the one closer to 0 (fitness can be negative)
        if (population[i].fitness < bestIndividual.fitness)
        {
            bestIndividual = population[i];
        }
    }
    // Retrieve the best next move
    int bestMove = bestIndividual.moves[0];
    // Cleanup
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        free(population[i].moves);
    }
    printf("Best move: %d with fitness: %d\n", bestMove, bestIndividual.fitness);
    printBoard(game);
    free(population);
    cleanupGame(&game);
    return bestMove;
}
