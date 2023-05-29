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
    int fitness; /**< The fitness of the individual where closest to 0 is the best */
    Game_context game; /**< Copy of the game context */
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
 */
void evaluateFitness(Individual *individual);

/**
 * @brief Evaluate the fitness of a player move
 * @param game The game context
 * @return The fitness of the player move
 */
int evaluatePlayerMove(Game_context *game);

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
 * @param boardCols The number of columns in the board
 * @return The offspring
 */
Individual crossover(Individual parent1, Individual parent2, int boardCols);

/**
 * @brief Perform mutation on an individual
 * @param individual The individual
 * @param boardCols The number of columns in the board
 */
void mutate(Individual *individual, int boardCols);

/**
 * @brief Perform reinsertion of offspring into the population
 * @param population The population
 * @param offspring The offspring
 */
void reinsertion(Individual *population, Individual offspring);

/**
 * @brief Find the best fitness between two fitness values
 * @param fitnessA The first fitness value
 * @param fitnessB The second fitness value
 * @return The best fitness value
 */
int bestFitness(int fitnessA, int fitnessB);

/**
 * @brief Find the worst fitness between two fitness values
 * @param fitnessA The first fitness value
 * @param fitnessB The second fitness value
 * @return The worst fitness value
 */
int worstFitness(int fitnessA, int fitnessB);

/**
 * @brief Find the best individual in the population
 * @param population The population
 * @return The best individual
 */
Individual getBestIndividual(Individual *population);

/**
 * @brief Perform the genetic search algorithm to find the best next move.
 * Mathematcially, the return value can potentially be an invalid move.
 * @param game The game context
 * @return The best next move
 */
int geneticSearch(Game_context game);

#endif // GENETIC_SEARCH_H
