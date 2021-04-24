//
// Created by rinat on 12/13/2020.
//

#include "genetic_solver.h"

#define ITERATION_NUMBER 100000
#define POP_COUNT_INITIAL 1000
#define POPULATION_SIZE 10

int getRandomIndividual(int population_size){
    return rand() % population_size;
}

vector<int> GeneticSolver::solve() {
    vector<int> schedule(data.interventions.size());
    checker = make_unique<Checker>(data);
    auto population = getInitialPopulation();
    for (int i = 0; i < ITERATION_NUMBER & !exit_; i ++){
        population = getNBest(population);
        crossover(population);
        mutate(population);
    }
    return population[0];
}

populationVec GeneticSolver::getInitialPopulation() {
    int n = data.interventions.size();
    populationVec population(POP_COUNT_INITIAL, vector<int>(n));
    for (int i = 0; i < POP_COUNT_INITIAL; i++){
        for (int j = 0; j < n; j++) {
            int cur = rand() % data.interventions[j].tmax;
            population[i][j] = cur;
        }
    }
    return population;
}

populationVec GeneticSolver::getNBest(populationVec &population) {

    bad_interventions = {};
    fitness_scores = {};
    vector<vector<int>> cur_bad_interventions = {};

    vector<int> scores;
    for (auto &schedule: population){
        scores.push_back(checker->checkAll(schedule));
        cur_bad_interventions.push_back(checker->wrong_resource_intervention);
    }
    vector<size_t> idx(population.size());
    iota(idx.begin(), idx.end(), 0);
    stable_sort(idx.begin(), idx.end(),
         [&](size_t i1, size_t i2) {return scores[i1] < scores[i2];});

    populationVec sorted_population = {};

    for (int i = 0; i < population_size; i++) {
        int ind = idx[i];
        sorted_population.push_back(population[ind]);
        bad_interventions.push_back(cur_bad_interventions[ind]);
        fitness_scores.push_back(scores[ind]);
    }

    if (scores[0] < best_score)
        update_solution(sorted_population[0], *checker);

    return sorted_population;
}

int GeneticSolver::getParent(populationVec &population){

    int i1 = getRandomIndividual(population_size),
        i2 = getRandomIndividual(population_size),
        i3 = getRandomIndividual(population_size),
        i4 = getRandomIndividual(population_size);

    float min_score = 999999999;
    int best_ind = -1;
    for (auto ind: {i1, i2, i3, i4}){
        if (fitness_scores[ind] <= min_score){
            min_score = fitness_scores[ind];
            best_ind = ind;
        }
    }
    return best_ind;
}

void GeneticSolver::crossover(populationVec & population) {

    for (int j = 0; j < population_size * 9 / 2; j++) {

        int parent_id1 = getParent(population);
        int parent_id2 = getParent(population);

        auto& parent1 = population[parent_id1];
        auto& parent2 = population[parent_id2];


        int mid_point = rand() % data.interventions.size();
        Individual child1, child2;

        for (int i = 0; i < mid_point; i++)
        {
            child1.push_back(parent1[i]);
            child2.push_back(parent2[i]);
        }

        for (int i = mid_point; i < data.interventions.size(); i++)
        {
            child1.push_back(parent2[i]);
            child2.push_back(parent1[i]);
        }

        if (rand() % 100 >= crossover_rate * 100) {
            child1 = parent1;
            child2 = parent2;
        }
        population.push_back(child1);
        population.push_back(child2);

        bad_interventions.push_back(bad_interventions[parent_id1]);
        bad_interventions.push_back(bad_interventions[parent_id2]);
    }

}

void GeneticSolver::mutate(populationVec & population) {
    int mutations_count = population.size() * data.interventions.size() * mutation_rate;
    for (int i = 0; i < mutations_count; i++) {
        int random_individ = rand() % population.size();
        int randindex = !bad_interventions[random_individ].empty() ?
                bad_interventions[random_individ][rand() % bad_interventions[random_individ].size()] : // if bad intervention exists
                rand() % data.interventions.size(); // otherwise just choose random intervention
        int new_time = rand() % data.interventions[randindex].tmax ;
        population[random_individ][randindex] = new_time;
    }
}

GeneticSolver::GeneticSolver(const DataInstance &data, int population_size, float mutation_rate, float crossover_rate) :
    population_size(population_size), mutation_rate(mutation_rate), crossover_rate(crossover_rate)
{
    this->data = data;
}

