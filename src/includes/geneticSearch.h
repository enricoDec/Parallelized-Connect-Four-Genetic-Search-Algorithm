#ifndef GENETIC_SEARCH_H
#define GENETIC_SEARCH_H

#include "game.h"

/**
 * @file geneticSearch.h
 * @brief This file contains the genetic search algorithm
 */

/**
 * @brief The individual in the population
 */
typedef struct
{
    int *moves;  /**< The moves of the individual */
    int fitness; /**< The fitness of the individual where 0 is the best */
    BoardState **board; /**< The board of the game */
} Individual;

/**
 * @brief Create a random individual
 * @param game The game context
 * @return The random individual
 */
Individual createRandomIndividual(Game_context game);

/**
 * @brief Evaluate the fitness of an individual
 * @param individual The individual
 * @param game The game context
 */
void evaluateFitness(Individual *individual, Game_context game);

/**
 * @brief Perform tournament selection to select parents
 * @param population The population
 * @return The selected parents
 */
Individual *tournamentSelection(Individual *population);

/**
 * @brief Perform crossover between two parents to create offspring
 * @param parent1 The first parent
 * @param parent2 The second parent
 * @return The offspring
 */
Individual crossover(Individual parent1, Individual parent2, Game_context game);

/**
 * @brief Perform mutation on an individual
 * @param individual The individual
 * @param game The game context
 */
void mutate(Individual *individual, Game_context game);

/**
 * @brief Perform reinsertion of offspring into the population
 * @param population The population
 * @param offspring The offspring
 */
void reinsertion(Individual *population, Individual offspring);

/**
 * @brief Perform the genetic search algorithm to find the best next move.
 * Mathematcially, the return value can potentially be an invalid move.
 * @param game The game context
 * @return The best next move
 */
int geneticSearch(Game_context game);

#endif // GENETIC_SEARCH_H
