//
// Created by rinat on 9/15/2020.
//

#ifndef ROADEF2020CHALLENGE_HILL_CLIMBING_H
#define ROADEF2020CHALLENGE_HILL_CLIMBING_H

#include "random_solver.h"
#include "parser.h"


class HillClimbingSASolver : public RandomSolver {
public:
    using RandomSolver::RandomSolver;

    HillClimbingSASolver(const DataInstance &data, float change_percent, float neigbors_percent, float initial_tempreature, bool simulated_annealing);

    void pregenerateBest();

    virtual void improvePregenerated();

    void checkForUpdate(double new_score, vector<int>& solution, int iter, Checker& checker);

    vector<int> solve() override;

    vector<int> bad_res;
private:
    float change_percent, neighbors_percent;
    bool simulated_annealing;
    float initial_tempreature;
    vector<int> prev_schedule;
};


#endif //ROADEF2020CHALLENGE_RANDOM_SOLVER_H
