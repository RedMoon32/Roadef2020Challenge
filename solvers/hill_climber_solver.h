//
// Created by rinat on 2/27/2021.
//

#ifndef ROADEF2020CHALLENGE_HILL_CLIMBER_SOLVER_H
#define ROADEF2020CHALLENGE_HILL_CLIMBER_SOLVER_H

#include "stochastic_walk_solver.h"


class HillClimbing : public StochasticWalkSolver{
public:
    using StochasticWalkSolver::StochasticWalkSolver;

    void improvePregenerated();
};


#endif //ROADEF2020CHALLENGE_HILL_CLIMBER_SOLVER_H
