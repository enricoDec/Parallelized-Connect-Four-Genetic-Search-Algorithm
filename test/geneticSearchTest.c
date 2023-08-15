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
    Game_context game = initGame(6, 6, 1);
    Individual individual = createRandomIndividual(game);
    setRemainingMovesToEmpty(&individual, 0);
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
    Game_context game = initGame(6, 6, 1);
    Individual individual = createRandomIndividual(game);
    for (int i = 0; i < 3; i++)
    {
        mu_assert(individual.moves[i] >= 0 && individual.moves[i] < game.boardCols, "individual.moves[i] should be between 0 and game.boardCols");
    }
}

MU_TEST(test_getBestIndividual)
{
    Game_context game = initGame(6, 6, 1);
    Individual population[1000];
    for (int i = 0; i < 1000; i++)
    {
        population[i] = createRandomIndividual(game);
    }
    Individual best = getBestIndividual(population);
    for (int i = 0; i < 1000; i++)
    {
        mu_assert(best.fitness >= population[i].fitness, "best.fitness should be greater than or equal to population[i].fitness");
    }
}

MU_TEST(test_evaluateFitness)
{
    Game_context game = initGame(6, 6, 1);
    Individual individual = createRandomIndividual(game);
    evaluateFitness(&individual);
}

MU_TEST(test_reinsertion)
{
    Game_context game = initGame(6, 6, 1);
    Individual *oldPopulation = malloc(1000 * sizeof(Individual));
    for (int i = 0; i < 1000; i++)
    {
        oldPopulation[i] = createRandomIndividual(game);
    }
    Individual *newPopulation = malloc(1000 * sizeof(Individual));
    for (int i = 0; i < 1000; i++)
    {
        newPopulation[i] = createRandomIndividual(game);
    }
    reinsertion(oldPopulation, newPopulation, game);
    Individual oldPopulationBestIndividual = getBestIndividual(oldPopulation);
    Individual newPopulationBestIndividual = getBestIndividual(newPopulation);
    mu_assert(oldPopulationBestIndividual.fitness == newPopulationBestIndividual.fitness, 
    "oldPopulationBestIndividual.fitness should be equal to newPopulationBestIndividual.fitness");
}

MU_TEST(test_mutate)
{
    Game_context game = initGame(6, 6, 1);
    Individual individual = createRandomIndividual(game);
    mutate(&individual, game.boardCols);
}

MU_TEST(test_crossover)
{
    Game_context game = initGame(6, 6, 1);
    Individual parent1 = createRandomIndividual(game);
    Individual parent2 = createRandomIndividual(game);
    Individual offspring = crossover(parent1, parent2, game);
    for (int i = 0; i < game.boardCols; i++)
    {
        mu_assert(offspring.moves[i] == parent1.moves[i] || offspring.moves[i] == parent2.moves[i], 
        "offspring.moves[i] should be equal to parent1.moves[i] or parent2.moves[i]");
    }
}

MU_TEST(test_tournamentSelection)
{
    Game_context game = initGame(6, 6, 1);
    Individual population[1000];
    for (int i = 0; i < 1000; i++)
    {
        population[i] = createRandomIndividual(game);
    }
    Individual parent = tournamentSelection(population);
    mu_assert(parent.fitness == getBestIndividual(population).fitness, 
    "parent.fitness should be equal to getBestIndividual(population).fitness");
}

MU_TEST(test_geneticSearch)
{
    Game_context game = initGame(6, 6, 1);
    geneticSearch(game);
}