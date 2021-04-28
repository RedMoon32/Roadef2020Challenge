//
// Created by rinat on 9/15/2020.
//

#ifndef ROADEF2020CHALLENGE_STOCHASTIC_WALK_SOLVER_H
#define ROADEF2020CHALLENGE_STOCHASTIC_WALK_SOLVER_H

#include "random_solver.h"
#include "parser.h"


class StochasticWalkSolver : public RandomSolver {
public:
    using RandomSolver::RandomSolver;

    StochasticWalkSolver(const DataInstance &data, float change_percent, float neigbors_percent, bool hill_climbing);

    void pregenerateBest();

    virtual void improvePregenerated();

    void checkForUpdate(double new_score, vector<int>& solution, Checker& checker);

    vector<int> solve() override;

    vector<int> bad_res;
private:
    float change_percent, neighbors_percent;
    bool hill_climbing;
};


#endif //ROADEF2020CHALLENGE_RANDOM_SOLVER_H
