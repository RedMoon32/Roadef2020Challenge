//
// Created by rinat on 9/29/2020.
//

#ifndef ROADEF2020CHALLENGE_PARTIALCHECKER_H
#define ROADEF2020CHALLENGE_PARTIALCHECKER_H

#include "checker.h"

class PartialChecker : public Checker {
public:
    PartialChecker(vector<int> schedule, const DataInstance &data);

    int checkResourceConstraint() override;

    int checkHorizon() override;

    vector <pair<int, int>> changed;
};


#endif //ROADEF2020CHALLENGE_PARTIALCHECKER_H
