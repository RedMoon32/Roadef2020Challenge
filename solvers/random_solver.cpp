//
// Created by rinat on 9/15/2020.
//

#include "random_solver.h"

vector<int> RandomSolver::solve() {
    vector<int> schedule(data.interventions.size());
    for (int j = 0; j < schedule.size(); j++) {
        int cur = rand() % data.interventions[j].tmax;
        schedule[j] = cur;
    }
    return schedule;
}

RandomSolver::RandomSolver(const DataInstance &data) : data(data) {

}

