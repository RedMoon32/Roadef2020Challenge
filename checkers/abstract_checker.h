//
// Created by rinat on 10/8/2020.
//

#ifndef ROADEF2020CHALLENGE_ABSTRACT_CHECKER_H
#define ROADEF2020CHALLENGE_ABSTRACT_CHECKER_H

class AbstractChecker {
public:

    virtual int checkAll() = 0;

    virtual int checkResourceConstraint() = 0;

    virtual int checkHorizon() = 0;

    virtual int checkExclusions() = 0;

};

#endif //ROADEF2020CHALLENGE_ABSTRACT_CHECKER_H
