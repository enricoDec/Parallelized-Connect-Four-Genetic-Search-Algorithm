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
    int *moves;        /**< The moves of the individual */
    int fitness;       /**< The fitness of the individual where closest to 0 is the best */
    Game_context game; /**< Copy of the game context */
} Individual;

/**
 * @brief Perform the genetic search algorithm to find the best next move.
 * Mathematcially, the return value can potentially be an invalid move.
 * @param game The game context
 * @return The best next move
 */
int geneticSearch(Game_context game);

/**
 * @brief Perform tournament selection to select parents
 * @param population The population
 * @return The selected parents
 */
Individual tournamentSelection(Individual *population);

/**
 * @brief Perform crossover between two parents to create offspring
 * @param parent1 The first parent
 * @param parent2 The second parent
 * @param game The game context
 * @return The offspring
 */
Individual crossover(Individual parent1, Individual parent2, Game_context game);

/**
 * @brief Perform mutation on an individual
 * @param individual The individual
 * @param boardCols The number of columns in the board
 */
void mutate(Individual *individual, int boardCols);

/**
 * @brief Reinsert the best individual into the new generation
 * @param newPopulation The new population
 * @param oldPopulation The old population
 * @param game The game context
 */
void reinsertion(Individual *newPopulation, Individual *oldPopulation, Game_context game);

/**
 * @brief Evaluate the fitness of an individual
 * @param individual The individual
 */
void evaluateFitness(Individual *individual);

/**
 * @brief Find the best individual in the population
 * @param population The population
 * @return The best individual
 */
Individual getBestIndividual(Individual *population);

/**
 * @brief Create a random individual
 * @param game The game context
 * @return The random individual
 */
Individual createRandomIndividual(Game_context game);

/**
 * @brief Set the remaining moves to empty
 * @param individual The individual
 * @param startIndex The start index
 */
void setRemainingMovesToEmpty(Individual *individual, int startIndex);

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

#endif // GENETIC_SEARCH_H
