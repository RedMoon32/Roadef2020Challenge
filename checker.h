//
// Created by rinat on 9/2/2020.
//

#ifndef ROADEF2020CHALLENGE_CHECKER_H
#define ROADEF2020CHALLENGE_CHECKER_H

#include "parser.h"

class AbstractChecker {
public:

    virtual int checkAll() = 0;

    virtual int checkResourceConstraint() = 0;

    virtual int checkHorizon() = 0;

    virtual int checkExclusions() = 0;

};

class Checker : public AbstractChecker {
public:
    Checker(vector<int> schedule, const DataInstance &data);

    int checkAll() override;

    int checkResourceConstraint() override;

    int checkHorizon() override;

    int checkExclusions() override;

    vector<int> schedule;
    vector<vector<int>> resource_consumption;
    const DataInstance &data;
};


#endif //ROADEF2020CHALLENGE_CHECKER_H
