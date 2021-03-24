//
// Created by rinat on 12/13/2020.
//

#ifndef ROADEF2020CHALLENGE_GENETIC_SOLVER_H
#define ROADEF2020CHALLENGE_GENETIC_SOLVER_H

#include "parser.h"
#include "stochastic_walk_solver.h"

typedef vector<int> Individual;
typedef vector<Individual> populationVec;

class GeneticSolver: public StochasticWalkSolver{
public:
    GeneticSolver(const DataInstance &data, int population_size, float mutation_rate);
    populationVec getInitialPopulation();
    populationVec getNBest(populationVec&);
    void crossover(populationVec&);
    void mutate(populationVec&);
    vector<int> solve() override;
    int getParent(populationVec & population);

    vector<vector<int>> bad_interventions;
    vector<float> fitness_scores;
    unique_ptr<Checker> checker;
private:
    int population_size;
    float mutation_rate;
};


#endif //ROADEF2020CHALLENGE_GENETIC_SOLVER_H
