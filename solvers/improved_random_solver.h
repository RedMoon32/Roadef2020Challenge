//
// Created by rinat on 9/15/2020.
//

#ifndef ROADEF2020CHALLENGE_IMPROVED_RANDOM_SOLVER_H
#define ROADEF2020CHALLENGE_IMPROVED_RANDOM_SOLVER_H

#include "random_solver.h"
#include "parser.h"


class StochasticWalkSolver : public RandomSolver {
public:
    using RandomSolver::RandomSolver;

    void pregenerateBest();

    void improvePregenerated();

    vector<int> solve() override;
};


#endif //ROADEF2020CHALLENGE_RANDOM_SOLVER_H
