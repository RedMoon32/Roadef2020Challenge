//
// Created by rinat on 9/15/2020.
//

#ifndef ROADEF2020CHALLENGE_RANDOM_SOLVER_H
#define ROADEF2020CHALLENGE_RANDOM_SOLVER_H

#include "parser.h"
#include "abstract_solver.h"
#include <pthread.h>

class RandomSolver : public AbstractSolver {
public:
    RandomSolver(const DataInstance &data);
    vector<int> solve() override;
    vector<int> generateRandomSchedule();
    const DataInstance &data;
};


#endif //ROADEF2020CHALLENGE_RANDOM_SOLVER_H
