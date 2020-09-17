//
// Created by rinat on 9/15/2020.
//

#include "solver.h"
#include <fstream>

void write_result(string path, vector<int> schedule, const vector<Intervention> &interventions) {
    cout << "Writing result to file" << endl;
    ofstream out;
    out.open(path);
    for (int ind = 0; ind < schedule.size(); ind++) {
        out << interventions[ind].name << " " << schedule[ind] + 1 << endl;
    }
    out.close();
    cout << "Wrote result" << endl;
}

vector<int> RandomSolver::solve() {
    vector<int> schedule(data.interventions.size());
    for (int j = 0; j < schedule.size(); j++) {
        int cur = rand() % data.interventions[j].tmax;
        schedule[j] = cur;
    }
    return schedule;
}

RandomSolver::RandomSolver(const DataInstance &data, int computationTime) : data(data),
                                                                            computationTime(computationTime) {

}

