//
// Created by rinat on 12/13/2020.
//

#include "abstract_solver.h"
#include <fstream>
#include <unistd.h>

mutex solution_lock;

void update_solution(vector<int> &new_best_solution) {
    solution_lock.lock();
    best_solution = new_best_solution;
    solution_lock.unlock();
}

void write_result(string path, vector<int> schedule, const vector<Intervention> &interventions) {
    exit_ = true;
    usleep(2000);

    cout << "Writing result to file" << endl;
    ofstream out;
    out.open(path);

    for (int ind = 0; ind < schedule.size(); ind++) {
        out << interventions[ind].name << " " << schedule[ind] + 1 << endl;
    }
    out.close();
    cout << "Wrote result" << endl;
}
