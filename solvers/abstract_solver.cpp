//
// Created by rinat on 12/13/2020.
//

#include "abstract_solver.h"
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
