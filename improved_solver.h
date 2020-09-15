//
// Created by rinat on 9/15/2020.
//

#ifndef ROADEF2020CHALLENGE_ABSTRACT_SOLVER_H
#define ROADEF2020CHALLENGE_ABSTRACT_SOLVER_H

#include "solver.h"
#include "parser.h"

class ImprovedRandomSolver: public AbstractSolver{
public:
    ImprovedRandomSolver(const DataInstance &data, int computationTime);
    vector<int> solve() override;
    const DataInstance &data;
private:
    int computationTime = 0;
};


#endif //ROADEF2020CHALLENGE_SOLVER_H
