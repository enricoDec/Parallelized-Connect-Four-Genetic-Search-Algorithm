#include "minunit.h"
#include "../src/includes/geneticSearch.h"
#include <stdlib.h>

MU_TEST(test_worstFitness)
{
    mu_assert(worstFitness(0, 2) == 2, "worstFitness(0,2) should be 2");
    mu_assert(worstFitness(0, -1) == -1, "worstFitness(0,-1) should be -1");
    mu_assert(worstFitness(0, 0) == 0, "worstFitness(0,0) should be 0");
    mu_assert(worstFitness(0, 1) == 1, "worstFitness(0,1) should be 1");
    mu_assert(worstFitness(0, 3) == 3, "worstFitness(0,3) should be 3");
}

MU_TEST(test_bestFitness)
{
    mu_assert(bestFitness(0, 2) == 0, "bestFitness(0,2) should be 0");
    mu_assert(bestFitness(0, -1) == 0, "bestFitness(0,-1) should be 0");
    mu_assert(bestFitness(0, 0) == 0, "bestFitness(0,0) should be 0");
    mu_assert(bestFitness(0, 1) == 0, "bestFitness(0,1) should be 0");
    mu_assert(bestFitness(0, 3) == 0, "bestFitness(0,3) should be 0");
}

MU_TEST(test_setRemainingMovesToEmpty)
{
    int maxMoves = 3;
    Game_context game = initGame(6, 6, 1);
    Individual individual = createRandomIndividual(game, maxMoves);
    setRemainingMovesToEmpty(&individual, 0, maxMoves);
    for (int i = 0; i < 3; i++)
    {
        if (i < 3)
        {
            mu_assert(individual.moves[i] >= 0 && individual.moves[i] < game.boardCols, "individual.moves[i] should be between 0 and game.boardCols");
        }
        else
        {
            mu_assert(individual.moves[i] == 0, "individual.moves[i] should be EMPTY");
        }
    }
}

MU_TEST(test_createRandomIndividual)
{
    int maxMoves = 3;
    Game_context game = initGame(6, 6, 1);
    Individual individual = createRandomIndividual(game, maxMoves);
    for (int i = 0; i < 3; i++)
    {
        mu_assert(individual.moves[i] >= 0 && individual.moves[i] < game.boardCols, "individual.moves[i] should be between 0 and game.boardCols");
    }
}

MU_TEST(test_getBestIndividual)
{
    int maxMoves = 3;
    int populationSize = 1000;
    Game_context game = initGame(6, 6, 1);
    Individual population[populationSize];
    for (int i = 0; i < populationSize; i++)
    {
        population[i] = createRandomIndividual(game, maxMoves);
    }
    Individual best = getBestIndividual(population, populationSize);
    for (int i = 0; i < populationSize; i++)
    {
        mu_assert(best.fitness >= population[i].fitness, "best.fitness should be greater than or equal to population[i].fitness");
    }
}

MU_TEST(test_evaluateFitness)
{
    int maxMoves = 3;
    Game_context game = initGame(6, 6, 1);
    Individual individual = createRandomIndividual(game, maxMoves);
    evaluateFitness(&individual, maxMoves);
}

MU_TEST(test_reinsertion)
{
    int maxMoves = 3;
    int populationSize = 1000;
    Game_context game = initGame(6, 6, 1);
    Individual *oldPopulation = malloc(populationSize * sizeof(Individual));
    for (int i = 0; i < populationSize; i++)
    {
        oldPopulation[i] = createRandomIndividual(game, maxMoves);
    }
    Individual *newPopulation = malloc(populationSize * sizeof(Individual));
    for (int i = 0; i < populationSize; i++)
    {
        newPopulation[i] = createRandomIndividual(game, maxMoves);
    }
    reinsertion(oldPopulation, newPopulation, game, maxMoves);
    Individual oldPopulationBestIndividual = getBestIndividual(oldPopulation, populationSize);
    Individual newPopulationBestIndividual = getBestIndividual(newPopulation, populationSize);
    mu_assert(oldPopulationBestIndividual.fitness == newPopulationBestIndividual.fitness,
              "oldPopulationBestIndividual.fitness should be equal to newPopulationBestIndividual.fitness");
}

MU_TEST(test_mutate)
{
    int maxMoves = 3;
    double mutationRate = 0.1;
    Game_context game = initGame(6, 6, 1);
    Individual individual = createRandomIndividual(game, maxMoves);
    mutate(&individual, game.boardCols, mutationRate, maxMoves);
}

MU_TEST(test_crossover)
{
    int maxMoves = 3;
    double crossoverRate = 0.8;
    Game_context game = initGame(6, 6, 1);
    Individual parent1 = createRandomIndividual(game, maxMoves);
    Individual parent2 = createRandomIndividual(game, maxMoves);
    Individual offspring = crossover(parent1, parent2, game, crossoverRate, maxMoves);
    for (int i = 0; i < game.boardCols; i++)
    {
        mu_assert(offspring.moves[i] == parent1.moves[i] || offspring.moves[i] == parent2.moves[i],
                  "offspring.moves[i] should be equal to parent1.moves[i] or parent2.moves[i]");
    }
}

MU_TEST(test_tournamentSelection)
{
    int maxMoves = 3;
    int populationSize = 1000;
    Game_context game = initGame(6, 6, 1);
    Individual population[populationSize];
    for (int i = 0; i < populationSize; i++)
    {
        population[i] = createRandomIndividual(game, maxMoves);
    }
    Individual parent = tournamentSelection(population, populationSize);
    mu_assert(parent.fitness == getBestIndividual(population, populationSize).fitness,
              "parent.fitness should be equal to getBestIndividual(population).fitness");
}

MU_TEST(test_geneticSearch)
{
    Game_context game = initGame(6, 6, 1);
    GeneticSearchParameters geneticSearchParameters;
    geneticSearchParameters.populationSize = 1000;
    geneticSearchParameters.crossoverRate = 0.8;
    geneticSearchParameters.mutationRate = 0.1;
    geneticSearchParameters.maxGenerations = 100;
    geneticSearchParameters.maxMoves = 3;
    geneticSearch(game, geneticSearchParameters);
}