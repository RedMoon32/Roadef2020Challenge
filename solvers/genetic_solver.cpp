//
// Created by rinat on 12/13/2020.
//

#include "genetic_solver.h"

#define ITERATION_NUMBER 100000
#define POP_COUNT_INITIAL 100000
#define POPULATION_SIZE 10

vector<int> GeneticSolver::solve() {
    vector<int> schedule(data.interventions.size());
    checker = make_unique<Checker>(data);
    auto population = getInitialPopulation();
    for (int i = 0; i < ITERATION_NUMBER; i ++){
        population = getNBest(population);
        crossover(population);
        mutate(population);
        cout << checker->checkAll(best_solution) << endl;
    }
    return population[0];
}

populationVec GeneticSolver::getInitialPopulation() {
    int n = data.interventions.size();
    populationVec population(POP_COUNT_INITIAL, vector<int>(n));
    for (int i = 0; i < POP_COUNT_INITIAL; i++){
        for (int j = 0; j < data.interventions.size(); j++) {
            int cur = rand() % data.interventions[j].tmax;
            population[i][j] = cur;
        }
    }
    return population;
}

populationVec GeneticSolver::getNBest(populationVec &population) {
    vector<int> scores;
    for (auto &schedule: population){
        scores.push_back(checker->checkAll(schedule));
    }
    vector<size_t> idx(population.size());
    stable_sort(idx.begin(), idx.end(),
         [&](size_t i1, size_t i2) {return scores[i1] < scores[i2];});

    populationVec sorted_population;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        sorted_population.push_back(population[i]);
    }
    best_solution = sorted_population[0];
    return sorted_population;
}

void GeneticSolver::crossover(populationVec & population) {
    int n = population.size();
    for (int j = 0; j < n; j++) {
        auto &first = population[j];
        auto &second = population[rand() % n];
        vector<int> schedule(first.size());
        for (int i = 0; i < first.size(); i++){
            schedule[i] = rand() % 10 == 0 ? first[i] : second[i];
        }
        population.push_back(schedule);
    }
}

void GeneticSolver::mutate(populationVec & population) {
    for (auto &individ: population) {
        for (int i = 0; i < 10; i++) {
            auto copy = individ;
            for (int j = 0; j < 3; j++) {
                int randindex = rand() % data.interventions.size();
                int cur = rand() % data.interventions[randindex].tmax;
                copy[randindex] = cur;
            }
            if (checker->checkAll(copy) < checker->checkAll(individ))
                individ = copy;
        }
    }
}

