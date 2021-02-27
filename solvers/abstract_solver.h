//
// Created by rinat on 10/4/2020.
//

#ifndef ROADEF2020CHALLENGE_ABSTRACT_SOLVER_H
#define ROADEF2020CHALLENGE_ABSTRACT_SOLVER_H

#include "parser.h"
#include "checker.h"

extern mutex solution_lock;
extern vector<int> best_solution;
extern bool exit_;

void write_result(string path, vector<int> schedule, const vector<Intervention> &interventions);

class AbstractSolver {
public:

    virtual vector<int> solve() = 0;

};

#endif //ROADEF2020CHALLENGE_ABSTRACT_SOLVER_H
