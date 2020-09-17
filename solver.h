//
// Created by rinat on 9/15/2020.
//

#ifndef ROADEF2020CHALLENGE_SOLVER_H
#define ROADEF2020CHALLENGE_SOLVER_H

#include "parser.h"

class AbstractSolver {
public:

    virtual vector<int> solve() = 0;

};

void write_result(string path, vector<int> schedule, const vector<Intervention> &interventions);

class RandomSolver : public AbstractSolver {
public:
    RandomSolver(const DataInstance &data, int computationTime);
    vector<int> solve() override;
    const DataInstance &data;
private:
    int computationTime = 0;
};


#endif //ROADEF2020CHALLENGE_SOLVER_H
