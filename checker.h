//
// Created by rinat on 9/2/2020.
//

#ifndef ROADEF2020CHALLENGE_CHECKER_H
#define ROADEF2020CHALLENGE_CHECKER_H

#include "parser.h"

class Checker {
public:
    Checker(const vector<int> schedule, const DataInstance& data);
    int checkAll();
    int checkResourceConstraint();
    int checkHorizon();
    int checkExclusion();
    const DataInstance& data;
    const vector<int> schedule;
};


#endif //ROADEF2020CHALLENGE_CHECKER_H
