//
// Created by rinat on 2/27/2021.
//

#include "HillClimbing.h"
#include <limits>

vector<int> HillClimbing::solve() {
    vector<int> schedule(data.interventions.size());
    vector<int> &best = best_solution;
    Checker checker(data);
    cout.precision(10);

    cout << "Iterations done " << endl;
    return best;
}
