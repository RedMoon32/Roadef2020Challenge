//
// Created by rinat on 2/27/2021.
//

#ifndef ROADEF2020CHALLENGE_HILLCLIMBING_H
#define ROADEF2020CHALLENGE_HILLCLIMBING_H

#include "random_solver.h"
#include "parser.h"


class HillClimbing : public RandomSolver{
public:
    using RandomSolver::RandomSolver;
    vector<int> solve() override;
};


#endif //ROADEF2020CHALLENGE_HILLCLIMBING_H
