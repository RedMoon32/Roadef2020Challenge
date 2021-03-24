//
// Created by rinat on 9/15/2020.
//

#include "random_solver.h"

vector<int> best_solution;
bool exit_ = false;

RandomSolver::RandomSolver(const DataInstance &data) : data(data) {

}

vector<int> RandomSolver::solve() {
    return generateRandomSchedule();
}

vector<int> RandomSolver::generateRandomSchedule() {
    vector<int> schedule(data.interventions.size());
    for (int j = 0; j < schedule.size(); j++) {
        int cur = rand() % data.interventions[j].tmax;
        schedule[j] = cur;
    }
    return schedule;
}

RandomSolver::RandomSolver() {}



