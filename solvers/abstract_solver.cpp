//
// Created by rinat on 12/13/2020.
//

#include "abstract_solver.h"
#include <fstream>
#include <unistd.h>

mutex solution_lock;

void update_solution(vector<int> &new_best_solution, Checker& checker) {
    solution_lock.lock();
    best_solution = new_best_solution;
    best_score = checker.checkAll(best_solution);
    solution_lock.unlock();
}

void write_result(string path, vector<int> schedule, const vector<Intervention> &interventions) {
    usleep(2000);
    ofstream out;
    out.open(path);

    for (int ind = 0; ind < schedule.size(); ind++) {
        out << interventions[ind].name << " " << schedule[ind] + 1 << endl;
    }
    out.close();
}
